#ifndef BUZZER_SLEEP_H_
#define BUZZER_SLEEP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"

#define BUZZER_PIN GPIO_NUM_14 //! BUZZER_PIN
#define SLEEP_PIN GPIO_NUM_6   //! SLEEP_PIN

void buzzer_sleep_init();

#endif