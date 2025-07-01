
#include "honeywell.h"

void app_main()
{
    honeywell_init();
    while (1)
    {
        printf("Pressure: %d cmH2O, Flow: %d L/min\n", sensor_data.pressure, sensor_data.flow);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
