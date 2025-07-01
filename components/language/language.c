#include "language.h"
#include "esp_log.h"
#include "u_nextion.h"

void set_language(language_t language)
{
    switch (language)
    {
    case 0: //! ENGLISH
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_EN_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_EN_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_EN_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_EN_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_EN_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_EN_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_EN_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_EN_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_EN_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_EN_HOME_T1);
        nextion_set_text(L_HOME_T2, L_EN_HOME_T2);
        nextion_set_text(L_HOME_T3, L_EN_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_EN_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_EN_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_EN_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_EN_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_EN_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_EN_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_EN_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_EN_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_EN_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_EN_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_EN_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_EN_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_EN_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_EN_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_EN_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_EN_DATE_T0);
        nextion_set_text(L_DATE_T1, L_EN_DATE_T1);
        nextion_set_text(L_DATE_T3, L_EN_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_EN_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_EN_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_EN_APAP_T1);
        nextion_set_text(L_APAP_T8, L_EN_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_EN_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_EN_INFO_T1);
        nextion_set_text(L_INFO_T2, L_EN_INFO_T2);
        nextion_set_text(L_INFO_T3, L_EN_INFO_T3);
        nextion_set_text(L_INFO_T5, L_EN_INFO_T5);
        nextion_set_text(L_INFO_T6, L_EN_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_EN_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_EN_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_EN_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_EN_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_EN_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_EN_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_EN_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_EN_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_EN_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_EN_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_EN_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_EN_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_EN_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_EN_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_EN_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_EN_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_EN_PASSWORD_T1);
        break;

    case 1: //! TURKISH
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_TR_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_TR_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_TR_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_TR_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_TR_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_TR_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_TR_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_TR_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_TR_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_TR_HOME_T1);
        nextion_set_text(L_HOME_T2, L_TR_HOME_T2);
        nextion_set_text(L_HOME_T3, L_TR_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_TR_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_TR_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_TR_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_TR_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_TR_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_TR_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_TR_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_TR_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_TR_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_TR_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_TR_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_TR_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_TR_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_TR_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_TR_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_TR_DATE_T0);
        nextion_set_text(L_DATE_T1, L_TR_DATE_T1);
        nextion_set_text(L_DATE_T3, L_TR_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_TR_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_TR_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_TR_APAP_T1);
        nextion_set_text(L_APAP_T8, L_TR_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_TR_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_TR_INFO_T1);
        nextion_set_text(L_INFO_T2, L_TR_INFO_T2);
        nextion_set_text(L_INFO_T3, L_TR_INFO_T3);
        nextion_set_text(L_INFO_T5, L_TR_INFO_T5);
        nextion_set_text(L_INFO_T6, L_TR_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_TR_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_TR_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_TR_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_TR_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_TR_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_TR_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_TR_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_TR_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_TR_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_TR_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_TR_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_TR_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_TR_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_TR_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_TR_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_TR_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_TR_PASSWORD_T1);
        break;

    case 2: //! FRENCH
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_FR_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_FR_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_FR_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_FR_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_FR_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_FR_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_FR_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_FR_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_FR_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_FR_HOME_T1);
        nextion_set_text(L_HOME_T2, L_FR_HOME_T2);
        nextion_set_text(L_HOME_T3, L_FR_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_FR_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_FR_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_FR_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_FR_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_FR_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_FR_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_FR_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_FR_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_FR_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_FR_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_FR_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_FR_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_FR_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_FR_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_FR_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_FR_DATE_T0);
        nextion_set_text(L_DATE_T1, L_FR_DATE_T1);
        nextion_set_text(L_DATE_T3, L_FR_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_FR_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_FR_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_FR_APAP_T1);
        nextion_set_text(L_APAP_T8, L_FR_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_FR_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_FR_INFO_T1);
        nextion_set_text(L_INFO_T2, L_FR_INFO_T2);
        nextion_set_text(L_INFO_T3, L_FR_INFO_T3);
        nextion_set_text(L_INFO_T5, L_FR_INFO_T5);
        nextion_set_text(L_INFO_T6, L_FR_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_FR_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_FR_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_FR_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_FR_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_FR_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_FR_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_FR_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_FR_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_FR_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_FR_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_FR_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_FR_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_FR_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_FR_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_FR_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_FR_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_FR_PASSWORD_T1);
        break;

    case 3: //! GERMAN
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_DE_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_DE_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_DE_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_DE_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_DE_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_DE_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_DE_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_DE_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_DE_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_DE_HOME_T1);
        nextion_set_text(L_HOME_T2, L_DE_HOME_T2);
        nextion_set_text(L_HOME_T3, L_DE_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_DE_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_DE_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_DE_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_DE_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_DE_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_DE_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_DE_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_DE_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_DE_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_DE_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_DE_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_DE_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_DE_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_DE_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_DE_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_DE_DATE_T0);
        nextion_set_text(L_DATE_T1, L_DE_DATE_T1);
        nextion_set_text(L_DATE_T3, L_DE_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_DE_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_DE_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_DE_APAP_T1);
        nextion_set_text(L_APAP_T8, L_DE_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_DE_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_DE_INFO_T1);
        nextion_set_text(L_INFO_T2, L_DE_INFO_T2);
        nextion_set_text(L_INFO_T3, L_DE_INFO_T3);
        nextion_set_text(L_INFO_T5, L_DE_INFO_T5);
        nextion_set_text(L_INFO_T6, L_DE_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_DE_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_DE_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_DE_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_DE_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_DE_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_DE_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_DE_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_DE_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_DE_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_DE_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_DE_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_DE_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_DE_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_DE_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_DE_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_DE_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_DE_PASSWORD_T1);

        break;

    case 4: //! SPANISH
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_ES_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_ES_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_ES_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_ES_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_ES_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_ES_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_ES_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_ES_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_ES_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_ES_HOME_T1);
        nextion_set_text(L_HOME_T2, L_ES_HOME_T2);
        nextion_set_text(L_HOME_T3, L_ES_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_ES_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_ES_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_ES_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_ES_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_ES_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_ES_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_ES_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_ES_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_ES_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_ES_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_ES_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_ES_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_ES_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_ES_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_ES_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_ES_DATE_T0);
        nextion_set_text(L_DATE_T1, L_ES_DATE_T1);
        nextion_set_text(L_DATE_T3, L_ES_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_ES_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_ES_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_ES_APAP_T1);
        nextion_set_text(L_APAP_T8, L_ES_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_ES_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_ES_INFO_T1);
        nextion_set_text(L_INFO_T2, L_ES_INFO_T2);
        nextion_set_text(L_INFO_T3, L_ES_INFO_T3);
        nextion_set_text(L_INFO_T5, L_ES_INFO_T5);
        nextion_set_text(L_INFO_T6, L_ES_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_ES_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_ES_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_ES_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_ES_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_ES_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_ES_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_ES_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_ES_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_ES_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_ES_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_ES_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_ES_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_ES_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_ES_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_ES_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_ES_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_ES_PASSWORD_T1);
        break;

    case 5: //! ITALIAN
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_IT_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_IT_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_IT_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_IT_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_IT_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_IT_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_IT_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_IT_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_IT_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_IT_HOME_T1);
        nextion_set_text(L_HOME_T2, L_IT_HOME_T2);
        nextion_set_text(L_HOME_T3, L_IT_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_IT_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_IT_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_IT_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_IT_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_IT_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_IT_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_IT_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_IT_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_IT_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_IT_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_IT_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_IT_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_IT_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_IT_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_IT_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_IT_DATE_T0);
        nextion_set_text(L_DATE_T1, L_IT_DATE_T1);
        nextion_set_text(L_DATE_T3, L_IT_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_IT_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_IT_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_IT_APAP_T1);
        nextion_set_text(L_APAP_T8, L_IT_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_IT_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_IT_INFO_T1);
        nextion_set_text(L_INFO_T2, L_IT_INFO_T2);
        nextion_set_text(L_INFO_T3, L_IT_INFO_T3);
        nextion_set_text(L_INFO_T5, L_IT_INFO_T5);
        nextion_set_text(L_INFO_T6, L_IT_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_IT_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_IT_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_IT_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_IT_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_IT_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_IT_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_IT_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_IT_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_IT_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_IT_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_IT_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_IT_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_IT_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_IT_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_IT_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_IT_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_IT_PASSWORD_T1);

        break;

    case 6: //! PORTUGUESE
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_PT_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_PT_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_PT_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_PT_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_PT_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_PT_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_PT_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_PT_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_PT_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_PT_HOME_T1);
        nextion_set_text(L_HOME_T2, L_PT_HOME_T2);
        nextion_set_text(L_HOME_T3, L_PT_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_PT_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_PT_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_PT_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_PT_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_PT_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_PT_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_PT_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_PT_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_PT_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_PT_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_PT_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_PT_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_PT_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_PT_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_PT_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_PT_DATE_T0);
        nextion_set_text(L_DATE_T1, L_PT_DATE_T1);
        nextion_set_text(L_DATE_T3, L_PT_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_PT_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_PT_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_PT_APAP_T1);
        nextion_set_text(L_APAP_T8, L_PT_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_PT_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_PT_INFO_T1);
        nextion_set_text(L_INFO_T2, L_PT_INFO_T2);
        nextion_set_text(L_INFO_T3, L_PT_INFO_T3);
        nextion_set_text(L_INFO_T5, L_PT_INFO_T5);
        nextion_set_text(L_INFO_T6, L_PT_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_PT_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_PT_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_PT_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_PT_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_PT_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_PT_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_PT_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_PT_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_PT_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_PT_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_PT_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_PT_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_PT_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_PT_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_PT_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_PT_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_PT_PASSWORD_T1);
        break;

    case 7: //! RUSSIAN
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_RU_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_RU_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_RU_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_RU_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_RU_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_RU_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_RU_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_RU_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_RU_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_RU_HOME_T1);
        nextion_set_text(L_HOME_T2, L_RU_HOME_T2);
        nextion_set_text(L_HOME_T3, L_RU_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_RU_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_RU_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_RU_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_RU_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_RU_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_RU_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_RU_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_RU_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_RU_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_RU_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_RU_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_RU_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_RU_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_RU_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_RU_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_RU_DATE_T0);
        nextion_set_text(L_DATE_T1, L_RU_DATE_T1);
        nextion_set_text(L_DATE_T3, L_RU_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_RU_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_RU_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_RU_APAP_T1);
        nextion_set_text(L_APAP_T8, L_RU_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_RU_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_RU_INFO_T1);
        nextion_set_text(L_INFO_T2, L_RU_INFO_T2);
        nextion_set_text(L_INFO_T3, L_RU_INFO_T3);
        nextion_set_text(L_INFO_T5, L_RU_INFO_T5);
        nextion_set_text(L_INFO_T6, L_RU_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_RU_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_RU_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_RU_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_RU_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_RU_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_RU_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_RU_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_RU_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_RU_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_RU_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_RU_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_RU_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_RU_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_RU_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_RU_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_RU_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_RU_PASSWORD_T1);

        break;

    case 8: //! ARABIC
        // PAGE0
        nextion_set_text(L_PAGE0_T0, L_AR_PAGE0_T0);
        nextion_set_text(L_PAGE0_B_MIN_VAL, L_AR_PAGE0_B_MIN_VAL);
        nextion_set_text(L_PAGE0_B_PRESS_VAL, L_AR_PAGE0_B_PRESS_VAL);
        nextion_set_text(L_PAGE0_ON_VAL, L_AR_PAGE0_ON_VAL);
        nextion_set_text(L_PAGE0_OFF_VAL, L_AR_PAGE0_OFF_VAL);
        nextion_set_text(L_PAGE0_L_DURATION, L_AR_PAGE0_L_DURATION);
        nextion_set_text(L_PAGE0_L_PRESS, L_AR_PAGE0_L_PRESS);
        nextion_set_text(L_PAGE0_L_LEVEL, L_AR_PAGE0_L_LEVEL);
        nextion_set_text(L_PAGE0_L_OKEY, L_AR_PAGE0_L_OKEY);

        // HOME
        nextion_set_text(L_HOME_T1, L_AR_HOME_T1);
        nextion_set_text(L_HOME_T2, L_AR_HOME_T2);
        nextion_set_text(L_HOME_T3, L_AR_HOME_T3);

        // HOME_INFO
        nextion_set_text(L_HOME_INFO_T1, L_AR_HOME_INFO_T1);
        nextion_set_text(L_HOME_INFO_T2, L_AR_HOME_INFO_T2);

        // SETTINGS_1
        nextion_set_text(L_SETTINGS_1_T1, L_AR_SETTINGS_1_T1);
        nextion_set_text(L_SETTINGS_1_T2, L_AR_SETTINGS_1_T2);

        // SETTINGS_2
        nextion_set_text(L_SETTINGS_2_T1, L_AR_SETTINGS_2_T1);
        nextion_set_text(L_SETTINGS_2_T2, L_AR_SETTINGS_2_T2);

        // SETTINGS_3
        nextion_set_text(L_SETTINGS_3_T1, L_AR_SETTINGS_3_T1);
        nextion_set_text(L_SETTINGS_3_T2, L_AR_SETTINGS_3_T2);

        // SETTINGS_4
        nextion_set_text(L_SETTINGS_4_T1, L_AR_SETTINGS_4_T1);
        nextion_set_text(L_SETTINGS_4_T2, L_AR_SETTINGS_4_T2);

        // DEVICE_SET_1
        nextion_set_text(L_DEVICE_SET_1_T1, L_AR_DEVICE_SET_1_T1);
        nextion_set_text(L_DEVICE_SET_1_T3, L_AR_DEVICE_SET_1_T3);
        nextion_set_text(L_DEVICE_SET_1_T4, L_AR_DEVICE_SET_1_T4);

        // DEVICE_SET_2
        nextion_set_text(L_DEVICE_SET_2_T1, L_AR_DEVICE_SET_2_T1);
        nextion_set_text(L_DEVICE_SET_2_T4, L_AR_DEVICE_SET_2_T4);

        // DATE
        nextion_set_text(L_DATE_T0, L_AR_DATE_T0);
        nextion_set_text(L_DATE_T1, L_AR_DATE_T1);
        nextion_set_text(L_DATE_T3, L_AR_DATE_T3);

        // HOUR
        nextion_set_text(L_HOUR_T3, L_AR_HOUR_T3);
        nextion_set_text(L_HOUR_T4, L_AR_HOUR_T4);

        // APAP
        nextion_set_text(L_APAP_T1, L_AR_APAP_T1);
        nextion_set_text(L_APAP_T8, L_AR_APAP_T8);

        // START
        nextion_set_text(L_START_T0, L_AR_START_T0);

        // INFO
        nextion_set_text(L_INFO_T1, L_AR_INFO_T1);
        nextion_set_text(L_INFO_T2, L_AR_INFO_T2);
        nextion_set_text(L_INFO_T3, L_AR_INFO_T3);
        nextion_set_text(L_INFO_T5, L_AR_INFO_T5);
        nextion_set_text(L_INFO_T6, L_AR_INFO_T6);
        // nextion_set_text(L_INFO_T7, L_AR_INFO_T7);
        // nextion_set_text(L_INFO_T8, L_AR_INFO_T8);
        // nextion_set_text(L_INFO_T9, L_AR_INFO_T9);
        // nextion_set_text(L_INFO_T11, L_AR_INFO_T11);
        // nextion_set_text(L_INFO_T12, L_AR_INFO_T12);

        // RAPOR_1
        nextion_set_text(L_RAPOR_1_B0, L_AR_RAPOR_1_B0);
        nextion_set_text(L_RAPOR_1_B1, L_AR_RAPOR_1_B1);
        nextion_set_text(L_RAPOR_1_B3, L_AR_RAPOR_1_B3);
        nextion_set_text(L_RAPOR_1_B10, L_AR_RAPOR_1_B10);
        nextion_set_text(L_RAPOR_1_B11, L_AR_RAPOR_1_B11);

        // COMFORT
        nextion_set_text(L_COMFORT_T0, L_AR_COMFORT_T0);
        nextion_set_text(L_COMFORT_T1, L_AR_COMFORT_T1);
        nextion_set_text(L_COMFORT_T2, L_AR_COMFORT_T2);

        // MASK
        nextion_set_text(L_MASK_T0, L_AR_MASK_T0);

        // ALARM
        nextion_set_text(L_ALARM_T0, L_AR_ALARM_T0);

        // PASSWORD
        nextion_set_text(L_PASSWORD_T0, L_AR_PASSWORD_T0);
        nextion_set_text(L_PASSWORD_T1, L_AR_PASSWORD_T1);
        break;
    }
}
