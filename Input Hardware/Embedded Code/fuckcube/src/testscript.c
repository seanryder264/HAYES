#include "stm32f3xx_hal.h"

#define LED_PIN                                GPIO_PIN_7
#define LED_GPIO_PORT                          GPIOA
#define LED_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

#define TX_SLEEP_PIN                           GPIO_PIN_8
#define TX_SLEEP_PORT                          GPIOA

#define TX_RCLK_PIN                            GPIO_PIN_6
#define TX_RCLK_PORT                           GPIOB

#define TX_CLK_PIN                             GPIO_PIN_3

#define TX_MOSI_PIN                            GPIO_PIN_5

SPI_HandleTypeDef hspi1;

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

uint8_t data = 0b11111111;
HAL_StatusTypeDef status;

int main(void)
{
  HAL_Init();
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = TX_SLEEP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TX_RCLK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TX_CLK_PIN|TX_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  MX_SPI1_Init();

  HAL_GPIO_WritePin(TX_SLEEP_PORT, TX_SLEEP_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TX_RCLK_PORT, TX_RCLK_PIN, GPIO_PIN_SET);

  while (1)
  {
    //status = HAL_SPI_Transmit(&hspi1, &data, 1, 1000);
    //HAL_Delay(100);

    HAL_GPIO_WritePin(GPIOB, TX_MOSI_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, TX_MOSI_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TX_RCLK_PORT, TX_RCLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TX_RCLK_PORT, TX_RCLK_PIN, GPIO_PIN_RESET);

    for (uint8_t i = 0; i < 32; i ++) {
        HAL_Delay(1);
        HAL_GPIO_WritePin(TX_SLEEP_PORT, TX_SLEEP_PIN, GPIO_PIN_SET);
        for (uint8_t j = 0; j < 20; j ++) {
            __ASM("nop");
        }
        HAL_GPIO_WritePin(TX_SLEEP_PORT, TX_SLEEP_PIN, GPIO_PIN_RESET);
        HAL_Delay(10);
        //HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_SET);
        //HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(TX_RCLK_PORT, TX_RCLK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(TX_RCLK_PORT, TX_RCLK_PIN, GPIO_PIN_RESET);
    }


    for (uint8_t i = 0; i < 32; i ++) {
      HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB, TX_CLK_PIN, GPIO_PIN_RESET);
    }
  }
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1) {}
}


void MemManage_Handler(void)
{
  while (1) {}
}

void BusFault_Handler(void)
{
  while (1) {}
}

void UsageFault_Handler(void)
{
  while (1) {}
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

