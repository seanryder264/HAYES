#include "Tasks/Blink.h"
#include "Peripherals/GPIO.h"
#include "FreeRTOS.h"
#include "task.h"

void Blink( void *pvParameters )
{
    while (1) {
        HAL_GPIO_TogglePin(TP908_GPIO_Port, TP908_Pin);
        vTaskDelay(5/portTICK_PERIOD_MS);
    }
}