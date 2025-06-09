#include "peripherals/i2c.h"

I2C_HandleTypeDef hi2c2;

void I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00201D2B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

HAL_StatusTypeDef I2C_Write(uint16_t devAddress, uint8_t *pData, uint16_t size)
{
  return HAL_I2C_Master_Transmit(&hi2c2, devAddress, pData, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef I2C_Read(uint16_t devAddress, uint8_t *pData, uint16_t size)
{
  return HAL_I2C_Master_Receive(&hi2c2, devAddress, pData, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef I2C_WriteRegister(uint16_t devAddress, uint8_t reg, uint8_t *pData, uint16_t size)
{
  return HAL_I2C_Mem_Write(&hi2c2, devAddress, reg, I2C_MEMADD_SIZE_8BIT, pData, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef I2C_ReadRegister(uint16_t devAddress, uint8_t reg, uint8_t *pData, uint16_t size)
{
  return HAL_I2C_Mem_Read(&hi2c2, devAddress, reg, I2C_MEMADD_SIZE_8BIT, pData, size, HAL_MAX_DELAY);
}