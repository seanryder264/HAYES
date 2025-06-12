/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    comp.h
  * @brief   This file contains all the function prototypes for
  *          the comp.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMP_H__
#define __COMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern COMP_HandleTypeDef hcomp3;
extern COMP_HandleTypeDef hcomp4;
extern COMP_HandleTypeDef hcomp5;
extern COMP_HandleTypeDef hcomp6;

void MX_COMP3_Init(void);
void MX_COMP4_Init(void);
void MX_COMP5_Init(void);
void MX_COMP6_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __COMP_H__ */

