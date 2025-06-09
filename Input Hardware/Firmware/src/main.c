#include "FreeRTOS.h"
#include "task.h"

#include "peripherals/GPIO.h"
#include "peripherals/ADC.h"
#include "peripherals/DMA.h"
#include "peripherals/clock.h"
#include "peripherals/SPI.h"
#include "peripherals/comparator.h"

#include "tasks/blink.h"
#include "tasks/measure.h"

#include <stdlib.h>

static TaskHandle_t measure_task;

int main(void)
{
    HAL_Init();
    
    Clock_Init();

    GPIO_TX_Init();
    GPIO_MUX_Init();
    GPIO_TP_Init();

    DMA_Init();
    ADC_Init();
    COMP6_Init();

    HAL_COMP_Start(&hcomp6);

    SPI_TX_Init();

    HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);

    if (HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED) != HAL_OK) {
        Error_Handler();
    }

    BaseType_t res = xTaskCreate(measure, "Blink Task", 4096, NULL, 2, &measure_task);
    configASSERT(res == pdPASS);

    vTaskStartScheduler();
    
    Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1);
}

void BusFault_Handler(void) {
    while (1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
}