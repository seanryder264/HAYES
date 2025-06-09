/*
 * measure.c
 *
 *  Created on: Jun 9, 2025
 *      Author: yuki
 */

#include "measure.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static TaskHandle_t xTaskToNotify = NULL;

void measure(void *pvParameters)
{
    //measure_task_params *params = (measure_task_params *) pvParameters;

    xTaskToNotify = xTaskGetCurrentTaskHandle();
    //measure_grid grid;
    static measure_sample buffer;


    xTaskNotifyGive(xTaskToNotify);
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);

    while (1) {
        for (uint8_t index = 0; index < 1; index ++) {
            //ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
            measure_start_collection(&buffer, index);


            vTaskDelay(1/portTICK_PERIOD_MS);
        }
    }
    vTaskDelete(NULL);
}

void measure_start_collection(measure_sample *buffer, uint8_t index)
{
    uint8_t col = index / 8;
    //uint8_t row = index % 8;
    HAL_StatusTypeDef res;

    uint32_t frame = 1U << col;

    HAL_SPI_Transmit(&hspi1, (uint8_t *) &frame, 4, 1000);
    HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_SET);
    for (uint8_t j = 0; j < 40; j ++) {
        __ASM("nop");
    }

    HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_RESET);
    res = HAL_ADC_Start_DMA(&hadc4, (uint32_t *) buffer->RX1_buffer, 512);
    if (res == HAL_OK)
    	HAL_GPIO_TogglePin(TP908_GPIO_Port, TP908_Pin);

    //HAL_TIM_OnePulse_Start_IT(&htim3, TIM_CHANNEL_1);
    TIM3->CR1 |= TIM_CR1_CEN;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    HAL_ADC_Stop_DMA(&hadc4);
    vTaskNotifyGiveFromISR(xTaskToNotify, NULL);
    //HAL_GPIO_TogglePin(TP908_GPIO_Port, TP908_Pin);
}

