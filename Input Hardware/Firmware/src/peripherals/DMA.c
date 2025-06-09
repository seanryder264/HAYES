#include "peripherals/DMA.h"
#include "stm32f3xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

void DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA2_Channel2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);

}

void DMA2_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc4);
    portYIELD_FROM_ISR(pdTRUE);
}