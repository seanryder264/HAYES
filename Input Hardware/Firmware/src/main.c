#include "Peripherals/GPIO.h"
#include "Peripherals/ADC.h"
#include "Peripherals/Clock.h"

int main(void)
{
    HAL_Init();

    Clock_Init();

    GPIO_TX_Init();
    GPIO_MUX_Init();

    while(1) {

    }
}

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