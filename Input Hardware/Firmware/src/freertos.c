/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_cdc_if.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

extern char *usbdata;
extern SPI_HandleTypeDef hspi3;

uint8_t led_status = 0b01111111;

int32_t encoder1_count = 0;
int32_t encoder2_count = 0;
uint8_t last_a_encoder1 = 0;
uint8_t last_a_encoder2 = 0;

uint8_t button1_state = 0;
uint8_t button2_state = 0;
uint8_t button3_state = 0;
uint8_t encoder1_button_state = 0;
uint8_t encoder2_button_state = 0;

uint8_t dpad1_up = 0;
uint8_t dpad1_down = 0;
uint8_t dpad1_left = 0;
uint8_t dpad1_right = 0;
uint8_t dpad1_button = 0;
uint8_t dpad2_up = 0;
uint8_t dpad2_down = 0;
uint8_t dpad2_left = 0;
uint8_t dpad2_right = 0;
uint8_t dpad2_button = 0;

uint8_t a1 = 0;
uint8_t a2 = 0;
uint8_t b1 = 0;
uint8_t b2 = 0;

/* USER CODE END Variables */
osThreadId USB_TransmitHandle;
osThreadId LED_WriteHandle;
osThreadId UI_ReadHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartUSB_Transmit(void const * argument);
void StartLED_Write(void const * argument);
void StartUI_Read(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  
  /* Create the thread(s) */
  /* definition and creation of USB_Transmit */
  osThreadDef(USB_Transmit, StartUSB_Transmit, osPriorityNormal, 0, 128);
  USB_TransmitHandle = osThreadCreate(osThread(USB_Transmit), NULL);

  /* definition and creation of LED_Write */
  osThreadDef(LED_Write, StartLED_Write, osPriorityLow, 0, 128);
  LED_WriteHandle = osThreadCreate(osThread(LED_Write), NULL);

  /* definition and creation of UI_Read */
  osThreadDef(UI_Read, StartUI_Read, osPriorityLow, 0, 128);
  UI_ReadHandle = osThreadCreate(osThread(UI_Read), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartUSB_Transmit */
/**
  * @brief  Function implementing the USB_Transmit thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUSB_Transmit */
void StartUSB_Transmit(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartUSB_Transmit */
  /* Infinite loop */
  for(;;) {
    osDelay(1);
	  if (CDC_Transmit_FS((uint8_t *) usbdata, strlen(usbdata)) != USBD_OK) {
		  //ssd1306_Fill(White);
      //ssd1306_SetCursor(6, 20);
      //ssd1306_WriteString("COM not established", Font_6x8, Black);
      //ssd1306_UpdateScreen();
	  } else {
		  //ssd1306_Fill(White);
      //ssd1306_SetCursor(6, 20);
      //ssd1306_WriteString("COM established", Font_6x8, Black);
      //ssd1306_UpdateScreen();
	  }
  }
  /* USER CODE END StartUSB_Transmit */
}

/* USER CODE BEGIN Header_StartLED_Write */
/**
* @brief Function implementing the LED_Write thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLED_Write */
void StartLED_Write(void const * argument)
{
  /* USER CODE BEGIN StartLED_Write */
  /* Infinite loop */
  for(;;)
  {
    uint8_t current_leds = led_status & 0b11110000;
    HAL_SPI_Transmit(&hspi3, &current_leds, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOC, UI_RCLK_Pin, GPIO_PIN_SET);
      osDelay(1);
    HAL_GPIO_WritePin(GPIOC, UI_RCLK_Pin, GPIO_PIN_RESET);
  }
  /* USER CODE END StartLED_Write */
}

/* USER CODE BEGIN Header_StartUI_Read */
/**
* @brief Function implementing the UI_Read thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUI_Read */
void StartUI_Read(void const * argument)
{
  /* USER CODE BEGIN StartUI_Read */
  HAL_GPIO_WritePin(GPIOC, OFFSET_COM_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOF, INPUT_COM_Pin, GPIO_PIN_SET);

  /* Infinite loop */
  for(;;)
  {
    /* --- Rotary Encoder 1 --- */
    HAL_GPIO_WritePin(GPIOC, OFFSET_COM_Pin, GPIO_PIN_RESET);
    osDelay(1);
    a1 = HAL_GPIO_ReadPin(GPIOC, SCALE_A_Pin);
    b1 = HAL_GPIO_ReadPin(GPIOC, SCALE_B_Pin);
    encoder1_button_state = HAL_GPIO_ReadPin(GPIOC, SCALE_PUSH_Pin);

    if(a1 != last_a_encoder1) {
      encoder1_count += (b1 != a1) ? 1 : -1;
    }
    last_a_encoder1 = a1;

    /* --- D-Pad 2 --- */
    dpad2_up = HAL_GPIO_ReadPin(GPIOC, OFFSET_UP_Pin);
    dpad2_down = HAL_GPIO_ReadPin(GPIOB, OFFSET_DOWN_Pin);
    dpad2_left = HAL_GPIO_ReadPin(GPIOB, OFFSET_LEFT_Pin);
    dpad2_right = HAL_GPIO_ReadPin(GPIOC, OFFSET_RIGHT_Pin);
    dpad2_button = HAL_GPIO_ReadPin(GPIOC, OFFSET_CENTER_Pin);

    HAL_GPIO_WritePin(GPIOC, OFFSET_COM_Pin, GPIO_PIN_SET);
    osDelay(1);

    /* --- Rotary Encoder 2 --- */
    HAL_GPIO_WritePin(GPIOF, INPUT_COM_Pin, GPIO_PIN_RESET);
    osDelay(1);
    a2 = HAL_GPIO_ReadPin(GPIOC, SCALE_A_Pin);
    b2 = HAL_GPIO_ReadPin(GPIOC, SCALE_B_Pin);
    encoder2_button_state = HAL_GPIO_ReadPin(GPIOC, SCALE_PUSH_Pin);

    if(a2 != last_a_encoder2) {
      encoder2_count += (b2 != a2) ? 1 : -1;
    }
    last_a_encoder2 = a2;

    /* --- D-Pad 1 --- */
    dpad1_up = HAL_GPIO_ReadPin(GPIOC, OFFSET_UP_Pin);
    dpad1_down = HAL_GPIO_ReadPin(GPIOB, OFFSET_DOWN_Pin);
    dpad1_left = HAL_GPIO_ReadPin(GPIOB, OFFSET_LEFT_Pin);
    dpad1_right = HAL_GPIO_ReadPin(GPIOC, OFFSET_RIGHT_Pin);
    dpad1_button = HAL_GPIO_ReadPin(GPIOC, OFFSET_CENTER_Pin);

    HAL_GPIO_WritePin(GPIOF, INPUT_COM_Pin, GPIO_PIN_SET);
    osDelay(1);

    /* --- Standalone Buttons --- */
    button1_state = HAL_GPIO_ReadPin(GPIOA, PUSH_1_Pin);
    button2_state = HAL_GPIO_ReadPin(GPIOB, PUSH_2_Pin);
    button3_state = HAL_GPIO_ReadPin(GPIOB, PUSH_3_Pin);
  }
  /* USER CODE END StartUI_Read */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

