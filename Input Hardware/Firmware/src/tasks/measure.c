#include "tasks/measure.h"

#include "peripherals/GPIO.h"
#include "peripherals/SPI.h"

#include "FreeRTOS.h"
#include "task.h"

uint32_t data;
uint32_t zero = 0U;

void measure( void *pvParameters )
{
    //measure_task_params *params = (measure_task_params *) pvParameters;

    while (1) {
        

        data ^= 1 << 0;
        HAL_SPI_Transmit(&hspi1, (uint8_t *) &data, 4, 1000);
        HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_SET);
        for (uint8_t j = 0; j < 55; j ++) {
            __ASM("nop");
        }
        HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_RESET);
        vTaskDelay(5/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}