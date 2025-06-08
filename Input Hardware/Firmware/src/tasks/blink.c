#include "tasks/blink.h"

#include "peripherals/GPIO.h"
#include "peripherals/SPI.h"

#include "FreeRTOS.h"
#include "task.h"


void blink( void *pvParameters )
{
    while (1) {
        HAL_GPIO_TogglePin(TP908_GPIO_Port, TP908_Pin);
        vTaskDelay(5/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}