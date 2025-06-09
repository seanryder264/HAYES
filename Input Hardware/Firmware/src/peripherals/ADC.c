#include "peripherals/ADC.h"
#include "peripherals/DMA.h"
#include "stm32f3xx_hal.h"
#include "main.h"

void ADC_Init() 
{
    ADC_ChannelConfTypeDef sConfig = {0};
    
    hadc4.Instance = ADC4;
    hadc4.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc4.Init.Resolution = ADC_RESOLUTION_12B;
    hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc4.Init.ContinuousConvMode = ENABLE;
    hadc4.Init.DiscontinuousConvMode = DISABLE;
    hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc4.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc4.Init.NbrOfConversion = 1;
    hadc4.Init.DMAContinuousRequests = DISABLE;
    hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc4.Init.LowPowerAutoWait = DISABLE;
    hadc4.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_ADC_Init(&hadc4) != HAL_OK)
    {
      Error_Handler();
    }
  
    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_3;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK)
    {
      Error_Handler();
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hadc->Instance==ADC4)
    {
        /* Peripheral clock enable */
        __HAL_RCC_ADC34_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ADC4 GPIO Configuration
        PB12     ------> ADC4_IN3
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* ADC4 DMA Init */
        /* ADC4 Init */
        hdma_adc4.Instance = DMA2_Channel2;
        hdma_adc4.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc4.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc4.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc4.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_adc4.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_adc4.Init.Mode = DMA_NORMAL;
        hdma_adc4.Init.Priority = DMA_PRIORITY_MEDIUM;
        if (HAL_DMA_Init(&hdma_adc4) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc4);

    }

}