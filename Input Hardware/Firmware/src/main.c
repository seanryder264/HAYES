#include "FreeRTOS.h"
#include "task.h"

#include "peripherals/GPIO.h"
#include "peripherals/ADC.h"
#include "peripherals/clock.h"
#include "peripherals/SPI.h"

#include "tasks/blink.h"

#include <stdlib.h>


int main(void)
{
    HAL_Init();
    
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /* System interrupt init*/
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    Clock_Init();

    GPIO_TX_Init();
    GPIO_MUX_Init();

    SPI_TX_Init();

    GPIO_TP_Init();

    xTaskCreate(blink, "Blink Task", 128, NULL, 0, NULL);

    vTaskStartScheduler();
    
    Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1);
}