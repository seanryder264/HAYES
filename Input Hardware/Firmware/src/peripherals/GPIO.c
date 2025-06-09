#include "peripherals/GPIO.h"
#include "peripherals/ADC.h"
#include "peripherals/DMA.h"

#include "stm32f3xx_hal.h"

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(INPUT_COM_GPIO_Port, INPUT_COM_Pin, GPIO_PIN_RESET);
    
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, TP908_Pin|TX_SLEEP_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, TP909_Pin|PUSH_2_Pin|TX_RCLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, OUTPUT_COM_Pin|TP906_Pin|TP907_Pin|MUX_A_Pin
                            |MUX_B_Pin|MUX_C_Pin|MUX_INHIBIT_Pin|UI_RCLK_Pin, GPIO_PIN_RESET);

    GPIO_TX_Init();
    GPIO_MUX_Init();
    
   /*Configure GPIO pins : OFFSET_CENTER_Pin OFFSET_RIGHT_Pin OFFSET_UP_Pin SCALE_B_Pin
                            SCALE_A_Pin SCALE_PUSH_Pin */
   GPIO_InitStruct.Pin = OFFSET_CENTER_Pin|OFFSET_RIGHT_Pin|OFFSET_UP_Pin|SCALE_B_Pin
                           |SCALE_A_Pin|SCALE_PUSH_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
 
   /*Configure GPIO pin : INPUT_COM_Pin */
   GPIO_InitStruct.Pin = INPUT_COM_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(INPUT_COM_GPIO_Port, &GPIO_InitStruct);
 
   /*Configure GPIO pins : OUTPUT_COM_Pin TP906_Pin TP907_Pin MUX_A_Pin
                            MUX_B_Pin MUX_C_Pin MUX_INHIBIT_Pin UI_RCLK_Pin */
   GPIO_InitStruct.Pin = OUTPUT_COM_Pin|TP906_Pin|TP907_Pin|UI_RCLK_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
 
   /*Configure GPIO pins : TP908_Pin TX_SLEEP_Pin */
   GPIO_InitStruct.Pin = TP908_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 
   /*Configure GPIO pins : TP909_Pin PUSH_2_Pin TX_RCLK_Pin */
   GPIO_InitStruct.Pin = TP909_Pin|PUSH_2_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
   /*Configure GPIO pin : PUSH_1_Pin */
   GPIO_InitStruct.Pin = PUSH_1_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(PUSH_1_GPIO_Port, &GPIO_InitStruct);
 
   /*Configure GPIO pins : PUSH_3_Pin OFFSET_UPB8_Pin OFFSET_DOWN_Pin */
   GPIO_InitStruct.Pin = PUSH_3_Pin|OFFSET_UPB8_Pin|OFFSET_DOWN_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
 
}

void GPIO_TX_Init(void)
{
    /* GPIO Ports Clock Enable */

    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }

    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    if (__HAL_RCC_SPI1_IS_CLK_DISABLED()) {
        __HAL_RCC_SPI1_CLK_ENABLE();
    }

    /*Configure GPIO TX Drivers*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TX_RCLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TX_SLEEP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TX_CLK_Pin|TX_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Set Default Output Values*/

    HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_RESET);
}

void GPIO_MUX_Init(void)
{
    /* GPIO Ports Clock Enable */

    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    /*Configure GPIO Muxes*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = MUX_A_Pin | MUX_B_Pin | MUX_C_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(MUX_C_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MUX_INHIBIT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MUX_INHIBIT_GPIO_Port, &GPIO_InitStruct);

    /*Set Default Output Values*/

    HAL_GPIO_WritePin(GPIOC, MUX_A_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, MUX_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, MUX_C_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MUX_INHIBIT_GPIO_Port, MUX_INHIBIT_Pin, GPIO_PIN_RESET);
}

void GPIO_TP_Init(void)
{
    /* GPIO Ports Clock Enable */

    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }

    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    /*Configure GPIO Muxes*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = TP908_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TP909_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TP906_Pin|TP907_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Set Default Output Values*/

    HAL_GPIO_WritePin(GPIOA, TP908_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, TP909_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOC, TP906_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, TP907_Pin, GPIO_PIN_RESET);
}