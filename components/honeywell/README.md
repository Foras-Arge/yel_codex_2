#include "F_control.h"
#include "F_nextion.h"
#include "F_lzeroMaster.h"
#include "F_utility.h"
#include <math.h>


#define pi 3.1416
extern int dataf[];
extern int datap[];

extern uint32_t CNT_highState;
extern uint32_t CNT_lowState;
extern uint32_t riseTimeMatchVal;
extern UART_HandleTypeDef huart_lcd;

uint32_t inspT,expT;
double brNum,brOrt;
double med,mid,medOrt,midOrt;

double U,motorU;
double ksent;
int windup=10000;
 
uint8_t FLAG_ref_change;
 
double acpap_inc;
int acpap_tick;
int asv_timer=5;

double asv_brt;
double asv_minvol;
breath_t breath;
uint8_t new_state=0;
double leak_new;

int f_arr[10];
int p_arr[10];

int nefes_fmax=0;
int nefes_fmin=2000;
int flow_delta_ort,flow_delta;
uint8_t delta_start=1;
uint8_t apne,last_apne,last_apne2;

const uint8_t tidal_arr_size=5;
int tidal_arr[tidal_arr_size];
int leak_arr[tidal_arr_size];
uint8_t possible_apne_arr[tidal_arr_size];

uint8_t FLAG_rampdone=0;

int val; //delete

int flow_base;
int pressure_base;

int rawFlow,flowNoFilter,flowWithFilter;
int rawPressure,pressureNoFilter,pressureWithFilter;

int pressure;
int flow;
int ref;

double Kp_filt=3,Kf_filt=600,K2=270,K22=270,inspirium_treshold=1.2,expirium_treshold=0.8;
double K_riseTime=1;
double fot_freq=1;
double Kp=0.005,Ki=0.0001,Kd=0,dt=1;
double Kad=5;

double mean;
double diff;
int treshCpap=2;
uint8_t cpapNefes,cpapNefesOld;
uint32_t apneTick;
uint8_t cpapH;

int basinc;
int akis;
int nef=0,nefes=0,nefesold,dp,df;



void F_sendBar(UART_HandleTypeDef *huart,double prs)
{
	double sendval;
		if(FLAG_deviceMode==1||FLAG_deviceMode==2)
			sendval=0.5*prs;
		else if(FLAG_deviceMode==3||FLAG_deviceMode==5)
			sendval=0.4*prs;
		else if(FLAG_deviceMode==4||FLAG_deviceMode==6||FLAG_deviceMode==7)
			sendval=prs*2/7;
		
		if(sendval>100)
			sendval=100;
		else if(sendval<0)
			sendval=0;
		F_nextionValSend(huart,PRS_bar,sendval);
		if(pressure>0)
		F_nextionValSend(huart,PRS_str,prs);
}


double median(double a[60]){
	
	   for (uint8_t i=0; i<59; i++){
      for (uint8_t j=i+1; j<59; j++){
         if (a[i] > a[j]){
            double t = a[i];
            a[i] = a[j];
            a[j] = t;
         }
      }
   }
		 
	 return (a[29]+a[30])/2;
}


double F_abs(double x)
{
	if (x<0)
		return -x;
	else 
		return x;
}

int F_riseTime(int delta)	
{
	int retval=0;
	
	if(riseTimeMatchVal>CNT_highState)
	retval=delta*(riseTimeMatchVal-CNT_highState)*(riseTimeMatchVal-CNT_highState)/(riseTimeMatchVal*riseTimeMatchVal);

	return retval;
}

int F_fot(uint8_t x)
{
	
	int a;
if(x)
{
a= 10*sin(fot_freq*pi*0.02*CNT_highState);
	return a;
}
else return 0;
}



void F_updateSensor(int pressure, int flow) //return leak value
{
	int i;
	int size=10;
	for(i=0;i<size-1;i++)
	{
		f_arr[size-1-i]=f_arr[size-2-i];
		p_arr[size-1-i]=p_arr[size-2-i];
	}
	f_arr[0]=flow;
	p_arr[0]=pressure;
	leak_new=(double)leak_new*(1-0.003)+(double)f_arr[0]*0.003;
}


void F_clearApneArray()
{
	int i;
	for(i=0;i<tidal_arr_size;i++)
	{
		tidal_arr[i]=0;
		leak_arr[i]=0;
		possible_apne_arr[i]=0;
	}

}

void cpapPrsUpdate(double prs)
{
	int i;
	int arr_size=10;
	for(i=0;i<arr_size-1;i++)
	{
	p_arr[arr_size-1-i]=p_arr[arr_size-2-i];
	}
	p_arr[0]=prs;
}

void cpapApneControl()
{
	mean= mean*0.995+0.005*p_arr[0];
	diff=p_arr[0]-p_arr[1];
	
	if((p_arr[0]-mean+treshCpap)*(p_arr[1]-mean+treshCpap)<0&&diff<-0.09)
	{
		cpapNefes=1;
		expT=HAL_GetTick()-apneTick;
		apneTick=HAL_GetTick();
		medOrt=0.2*medOrt+0.8*diff;
	}
	else if((p_arr[0]-mean-treshCpap)*(p_arr[1]-mean-treshCpap)<0&&diff>0.09)
	{
		cpapNefes=0;
		inspT=HAL_GetTick()-apneTick;
		brNum=6000/(inspT+expT);
		brOrt=0.5*brOrt+0.5*brNum;
		apneTick=HAL_GetTick();
		midOrt=0.2*midOrt+0.8*diff;
	}
	if(HAL_GetTick()-apneTick>12000)
	{
		cpapH=1;
	}
	else
	{
		cpapH=0;
	}
}

//void F_updateTidal(uint8_t d) //returns apnea value
//{
//	int i;
//	int mean_tidal=0;
//	int mean_leak=0;
//	if(FLAG_deviceMode==7 && asv_timer>0)
//	{
//		asv_minvol=asv_minvol*0.9+0.1*tidal_arr[0];
//		asv_brt=asv_brt*0.9+0.1*breath.bcnt;
//	}
//	for(i=0;i<tidal_arr_size;i++)
//	{
//		mean_tidal+=tidal_arr[i];
//		mean_leak+=leak_arr[i];
//	}
//	mean_tidal/=tidal_arr_size;
//	mean_leak/=tidal_arr_size;
//	for(i=0;i<tidal_arr_size-1;i++)
//		{
//			possible_apne_arr[tidal_arr_size-1-i]=possible_apne_arr[tidal_arr_size-2-i];
//		}
//	if(d==3)
//	{
//		possible_apne_arr[0]=3;
//	}
//	else
//	{
//		if(breath.ort_tidal> mean_tidal*0.5)
//		{
//			for(i=0;i<tidal_arr_size-1;i++)
//			{
//				tidal_arr[tidal_arr_size-1-i]=tidal_arr[tidal_arr_size-2-i];
//				leak_arr[tidal_arr_size-1-i]=leak_arr[tidal_arr_size-2-i];
//			}
//			leak_arr[0]=breath.ort_leak;
//			tidal_arr[0]=breath.ort_tidal;
//			possible_apne_arr[0]=0;
//		}
//		else
//		{
//			if(breath.ort_tidal< mean_tidal*0.2)
//				possible_apne_arr[0]=1;
//			else
//				possible_apne_arr[0]=2;
//		}
//	}
//	
//	if(possible_apne_arr[0]==0 || possible_apne_arr[1]==0 || possible_apne_arr[2]==0)
//	{
//	breath.apne_type=0;
//	}
//	else if(possible_apne_arr[0]==2 && possible_apne_arr[1]==2 && possible_apne_arr[2]==2)
//	{
//	breath.apne_type=2;
//	}
//	else
//	{
//	breath.apne_type=1;
//	}
//}

void F_nefes2(UART_HandleTypeDef *huart,breath_t *br_ptr)
{
	static uint16_t CNT_nobreath=0;
	int leak_new2;
	uint32_t NoBreathTime=12*f_tick;
//	 new_state=br_ptr->state;
	if(breath.state==1 && (f_arr[0]-f_arr[8])*br_ptr->e<-expirium_treshold*e_trig && f_arr[0]<leak_new*0.9)
	{
		new_state=0;
		if(br_ptr->apne_type==3)
		br_ptr->apne_type=0;
	}
	else if(breath.state==0 && (f_arr[0]-f_arr[8])*br_ptr->e>inspirium_treshold*i_trig && f_arr[0]>leak_new*1.1)
	{
		new_state=1;
		if(!FLAG_plot)
			F_nextionValSend(&huart_lcd,"click t999,",1);
		if(br_ptr->apne_type==3)
		br_ptr->apne_type=0;
	}
	if(CNT_nobreath>NoBreathTime)
	{
		br_ptr->apne_type=3;
//	if(br_ptr->state)
//		new_state=0;
//	else
//		new_state=1;
	}else
	{
	CNT_nobreath++;
	}	
	if(new_state!=br_ptr->state)
	{
		CNT_nobreath=0;
		if(new_state==1)
		{
			flow_delta=nefes_fmax-nefes_fmin;
			nefes_fmax=0;
			nefes_fmin=1000;
			if(delta_start||(flow_delta_ort*0.7<flow_delta && flow_delta<flow_delta_ort*1.5))
			{
				flow_delta_ort=flow_delta_ort*0.9+flow_delta*0.1;
				apne=0;
			}
			else
			{
				flow_delta_ort = flow_delta_ort*0.99+flow_delta*0.01;
				if(flow_delta<flow_delta_ort*0.2)
					apne=1;
				else if(flow_delta<flow_delta_ort*0.5)
					apne=2;
			}
			if(br_ptr->apne_type!=3)
			{
				if(last_apne2!=0 &&last_apne!=0 && apne!=0)
					br_ptr->apne_type=apne;
				else
					br_ptr->apne_type=0;
			}
			last_apne2=last_apne;
			last_apne=apne;
			
			br_ptr->high_tidal=br_ptr->high_tt*100/60/f_tick;
			br_ptr->high_tt=0;
			br_ptr->bcnt=60*f_tick/br_ptr->leak_cnt;
			br_ptr->leak_cnt=0;
			
			br_ptr->ort_tidal=(br_ptr->high_tidal-br_ptr->low_tidal)/2; //tidal
			if(br_ptr->ort_tidal<0)
			{
				br_ptr->ort_tidal=0;
			}else if(br_ptr->ort_tidal>1801)
			{
				br_ptr->ort_tidal=1801;
			}
			
			
//			F_updateTidal(cent_ind);
			if(leak_new<0)
				leak_new2=0;
			else
				leak_new2=leak_new;
			F_nextionValSend(huart,STR_leak,(br_ptr->bcnt<<8)|(leak_new2/10)|(br_ptr->ort_tidal<<16));			
		}
		else
		{
			br_ptr->low_tidal=br_ptr->low_tt/60;
			br_ptr->low_tt=0;
		}
		CNT_highState=0;
		CNT_lowState=0;
		
		FLAG_ref_change=1;
		
		br_ptr->state=new_state;
		
	}
	else
	{
		if(new_state==1)
		{
			if(nefes_fmax<f_arr[0])
				nefes_fmax=f_arr[0];
			br_ptr->high_tt+=f_arr[0]-leak_new;
		}
		else
		{
			if(nefes_fmin>f_arr[0])
				nefes_fmin=f_arr[0];
			br_ptr->low_tt+=f_arr[0]-leak_new;
		}
		br_ptr->leak_cnt++;
	}
}



double FS_acpap_inc(int maxval)
{
	
	static uint8_t last_type = 0;
	
	if(breath.apne_type==0 && (last_type==1||last_type==2))
	{
		acpap_inc+=5;
		acpap_tick=0;
	}
	else
	{
	if(acpap_tick<60*f_tick)
		acpap_tick++;
	else{
		acpap_tick=0;
		acpap_inc-=2;
		}
	}
	
	last_type = breath.apne_type;
	
	if(acpap_inc<0)
		return acpap_inc=0;
	if(acpap_inc>maxval)
		return maxval;
	else
		return acpap_inc;		
}

double FS_avaps_inc()
{
	static double retval=0;
	if(FLAG_ref_change)
		{
		if(breath.ort_tidal<tidal_volume*0.9)
		{
			retval+=5;
		}
		else if(breath.ort_tidal>tidal_volume*1.1)
		{
			retval-=5;
		}
		if(retval<0)
		{
			retval=0;
		}
		else if(retval>PRS_ipap_max-PRS_ipap_min)
		{
			retval=PRS_ipap_max-PRS_ipap_min;
		}
		FLAG_ref_change=0;
	}
	return retval;
}

double FS_asv_refSet(double epap,double min_sup, double max_sup)
{
	static double retval=0;
	static double ratio=1;
	
	if(FLAG_ref_change)
	{
	FLAG_ref_change=0;
	
		if(asv_timer>0)
			return retval=min_sup;
		else
		{
			ratio=asv_minvol/(breath.ort_tidal+1);
			if(ratio>1.2)
				ratio=1.2;
			else if(ratio<0.8)
				ratio=0.8;			
			retval = retval*ratio;
			
			if(retval<min_sup)
				return retval=min_sup;
			else if(retval>max_sup)
				return retval=max_sup;
			else
				return retval;
		}
	}
	return retval;
}

double F_SettRef(void)
{
	double retval;
	if(FLAG_deviceMode==1)
	{
		retval=F_ramp(PRS_cpap);
		ksent=1;
	}
	else if(FLAG_deviceMode==2)
	{
		retval=F_ramp(PRS_acpap_min)+FS_acpap_inc(PRS_acpap_max-PRS_acpap_min);
		ksent=1;
	}
	else if(FLAG_deviceMode==3)
	{
		if(breath.state)
			retval=F_ramp(PRS_bpap_epap)+PRS_bpap_ipap-PRS_bpap_epap-F_riseTime(PRS_bpap_ipap-PRS_bpap_epap);
		else
			retval=F_ramp(PRS_bpap_epap);
		ksent=0.8;
	}else if(FLAG_deviceMode==5)
	{
		if(breath.state)
			retval=F_ramp(PRS_abpap_min)+PRS_abpap_ps+FS_acpap_inc(PRS_abpap_max-PRS_abpap_min)-F_riseTime(PRS_abpap_ps);
		else
			retval=F_ramp(PRS_abpap_min)+FS_acpap_inc(PRS_abpap_max-PRS_abpap_min);
		ksent=0.8;
	}else if(FLAG_deviceMode==4)
	{
		if(breath.state)
			retval=F_ramp(PRS_st_epap)+PRS_st_ipap-PRS_st_epap-F_riseTime(PRS_st_ipap-PRS_st_epap);
		else
			retval=F_ramp(PRS_st_epap);
		ksent=4.0/7;
	}else if(FLAG_deviceMode==6)
	{
		if(breath.state)
			retval=F_ramp(PRS_bpap_epap)+PRS_ipap_min-PRS_bpap_epap+FS_avaps_inc()-F_riseTime(PRS_ipap_min-PRS_avaps_epap);
		else
			retval= F_ramp(PRS_avaps_epap);
		ksent=4.0/7;
		
	}else if(FLAG_deviceMode==7)
	{
		retval= F_ramp(PRS_asv_epap)+ breath.state*(FS_asv_refSet(PRS_asv_epap,PRS_ps_min,PRS_ps_max)-F_riseTime(PRS_ps_min));
		ksent=4.0/7;
	}
	return retval;
}

double F_ramp(double peep)
{
	int val;
	if(FLAG_rampdone==1)
		return peep;
	
	double retval;
	double prs_tick;
	
		prs_tick=(peep-RAMP_pressure)/(RAMP_time*5*60*f_tick);
		retval=RAMP_pressure+prs_tick*RAMP_passedTime;
	if(RAMP_passedTime%(f_tick*30)==0)
	{
	val=RAMP_passedTime/(60*f_tick);
	F_nextionValSend(&huart_lcd,STR_RAMP_remains,5*RAMP_time-val);
	}
	return retval;
}

int F_SetRef(int Pset,int Prampa, int Tr,uint8_t nefes,uint32_t ipra)
{
	int retval;
	// Rampa durumunda rampali basinci return et
	if(RAMP_passedTime!=Tr*6000)
	{
		retval= Prampa+((double)(Pset-Prampa)/(Tr*6000))*RAMP_passedTime;
		return retval;
	}
	/// rampa yoksa ipra göre basinci return et
	else
	{
		retval = 0;
	}
	return retval;
}


void F_SetPwm(TIM_HandleTypeDef *htim,double dutyCycle)
{
	dutyCycle=dutyCycle*(3600-1);
	htim->Instance->CCR1=(uint32_t)dutyCycle;
}




int F_measure_I2C(I2C_HandleTypeDef *channel,uint8_t times)
{
	int retval=0;
	int measurement;
	uint8_t out[4];
	for(int i=0; i <times;i++)
				{
					HAL_I2C_Master_Receive(channel,(0x38<<1)|0x01, out, 4, 200);
					measurement=(((out[0]&0x3f) <<8)|out[1]);
					
					retval+=measurement;		
				}
				
		retval = retval/times;
		return retval;
}


void F_get_base(I2C_HandleTypeDef *pchannel,I2C_HandleTypeDef *fchannel)
{
 pressure_base = F_measure_I2C(pchannel,50);

	flow_base=F_measure_I2C(fchannel,50);
	if(pressure_base<8250 && pressure_base>8100 && flow_base<8250 && flow_base>8100)
		return;
	else
	{
	FLAG_error=error_base;
	}
}




int F_pressureConvert(double data)
{
  return 0.095321*data;
}

int F_pressureFilter(int pp)
{
	static double yy;
	static double ep;
	
	ep=(double)(yy-pp)/(pp);
	ep=F_abs(ep*Kp_filt);
	ep=ep*(K2-ref)/K2;
	
	if (ep > 0.95)
	{
		ep=0.95;
	}
	else if(ep<0.05)
	{
		ep=0.05;
	}
	yy=ep*yy+(1-ep)*pp;
	return (int)yy;
}

int F_flowFilter(int ff)
{
	static int fout;
	double ef;
	/*
		ef=(double)(fout-ff)/(ff);
		ef=F_abs(ef*Kf_filt);
		ef=ef*(K22-ref)/K22;
	*/
		ef=(double) F_abs(fout-ff)/Kf_filt;
		if (ef > 0.95)
		{
			ef=0.95;
		}else if(ef<0.05)
		{
			ef=0.05;
		}
		fout=(1-ef)*fout+ef*ff;
		breath.e=ef;
		
		return fout;
}

int F_flowConvert(double data)
{

	
	// constants from matlab flow_analysis.m
	double p1[] =
	{
	//0.00014654,-0.017239,0.82578,0.01481
		2.5419e-06, -0.0017567, 0.4532, 2.5134
	};
	double p2[] = 
	{
	4.1106e-07,-0.0005016,0.24597,5.7692
	};
	double p3[] =
	{
		-9.0825e-06,0.063717,25.757
	};
	double flow;
	int sign,t=4;
	
	if(data<0)
	{
		sign=-1;
		data=-1*data;
	}
	else
	{
		sign=1;
	}
	if(data<=20)
		t=0;
	else if(data<=450)
		t=1;
	else
		t=2;
	if(t==0)
		flow = sign*(p1[0]*data*data*data+p1[1]*data*data+p1[2]*data+p1[3]);
	else if(t==1)
		flow = sign*(p2[0]*data*data*data+p2[1]*data*data+p2[2]*data+p2[3]);
	else if(t==2)
		flow = sign*(p3[0]*data*data+p3[1]*data+p3[2]);
	else
		{
			FLAG_error = error_flowSensor;
		}
	
	return flow*11;
}

int F_get_pressure(I2C_HandleTypeDef *ptr)
{
	rawPressure = F_measure_I2C(ptr,5);
	pressureNoFilter = F_pressureConvert(rawPressure-pressure_base);
	pressureWithFilter = F_pressureFilter(pressureNoFilter);
	return pressureWithFilter;
}

int F_get_flow(I2C_HandleTypeDef *ptr,int p)
{
	rawFlow = F_measure_I2C(ptr,4);
	flowNoFilter = F_flowConvert(rawFlow-flow_base);
	flowWithFilter = F_flowFilter(flowNoFilter);
	return flowWithFilter;
}


void F_measure(TIM_HandleTypeDef *htim,I2C_HandleTypeDef *pptr,I2C_HandleTypeDef *fptr,double cont)
{
		if(FLAG_startStop==0)
	{
		
		int adet=50;
		F_SetPwm(htim,cont/100);
		
		pressure = F_measure_I2C(pptr,adet);
				
		basinc= F_pressureConvert(pressure-pressure_base);
		
		//basinc=0.095321*(pressure-8199)-1.9036;
						
		flow = F_measure_I2C(fptr,adet);
		flow=F_flowConvert(flow-flow_base);
				
	}
	
}


double F_pid_Control(int r,int y)
{
	
	double error=0;
	static double int_error=0,old_error=0;
	double u,bias;
	
	bias=-4.3086e-06*r*r+0.0033225*r+0.1789;

	error=r-y;
	int_error+=error;
	
	if(int_error>windup)
		int_error=windup;
	else if(int_error<-windup)
		int_error=-windup;
		
	u=Kp*error+Ki*int_error+Kd*(error-old_error)+bias;
	
	old_error=error;

	
//	else if(u<0.08)
//		u=0.08;
	if(u<=0.08){
		u=0.08;
		HAL_GPIO_WritePin(GPIOG,BRAKE,GPIO_PIN_SET);
	}else{
		if(u>0.99)
			u=0.99;
		HAL_GPIO_WritePin(GPIOG,BRAKE,GPIO_PIN_RESET);
	}
	return u;
}



void F_AutoStart(I2C_HandleTypeDef *hpress,UART_HandleTypeDef *lcd)
{
	
		if(FLAG_startStop==0 && U==0)
	{
		if(runAnyway)
		{
			FLAG_startInit=1;
			F_nextionValSend(lcd,"main.ss.val=",1);
			if(FLAG_language==0)
			F_nextionValSend(lcd,pageStr,2);
			else if(FLAG_language==1)
			F_nextionValSend(lcd,pageStr,52);
			else if(FLAG_language==2)
			F_nextionValSend(lcd,pageStr,66);
			
			HAL_Delay(10);
			
			runAnyway=0;
			return;                                                                                                                                             
		}
		
		rawPressure = F_measure_I2C(hpress,4);
		if(rawPressure < 7500 && rawPressure > 4000)
		{
			FLAG_startInit=1;
			
		//ekran acilisi yapilacak
		
		F_nextionValSend(lcd,"main.ss.val=",1);
		if(FLAG_language==0)
		F_nextionValSend(lcd,pageStr,2);
		else if(FLAG_language==1)
		F_nextionValSend(lcd,pageStr,52);
		else if(FLAG_language==2)
		F_nextionValSend(lcd,pageStr,66);
		
		HAL_Delay(10);
		}
	}
}
void F_MaskOff(UART_HandleTypeDef *lcd,UART_HandleTypeDef *lzero)
{
	
		FLAG_stopInit=1;
		F_nextionValSend(lcd,"maskoff.val=",1);
		F_ringAlarm(lzero,3);
}
