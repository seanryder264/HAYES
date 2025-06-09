#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

void I2C2_Init(void);
HAL_StatusTypeDef I2C_Write(uint16_t devAddress, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef I2C_Read(uint16_t devAddress, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef I2C_WriteRegister(uint16_t devAddress, uint8_t reg, uint8_t *pData, uint16_t size);
HAL_StatusTypeDef I2C_ReadRegister(uint16_t devAddress, uint8_t reg, uint8_t *pData, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif 