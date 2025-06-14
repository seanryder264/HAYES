/*
 * measure.c
 *
 *  Created on: Jun 9, 2025
 *      Author: yuki
 */

#include "measure.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "arm_math.h"
#include "math.h"

#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "stdio.h"

#define TIM_TO_ADC_RAITO    28  // 1.5 + 12.5 ADC time, /2 ADC clock
#define IC_PSC              8   // Prescaler on input capture channels
#define CLK_KHZ             72000

#define F_C_0 610
#define F_C_1 563
#define F_C_2 516
#define F_C_3 487
#define F_C_4 452
#define F_C_5 411
#define F_C_6 367
#define F_C_7 333
#define F_C_8 300

#define F_0_0 650
#define F_0_1 585
#define F_1_2 539
#define F_2_3 501
#define F_3_4 469
#define F_4_5 431
#define F_5_6 389
#define F_6_7 350
#define F_7_8 315
#define F_8_8 280

#define NAF_C (2 * PI * TIM_TO_ADC_RAITO) / (CLK_KHZ)

#define SIZE_X 32 //32
#define SIZE_Y 8 //8

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

extern COMP_HandleTypeDef hcomp3;
extern COMP_HandleTypeDef hcomp4;
extern COMP_HandleTypeDef hcomp5;
extern COMP_HandleTypeDef hcomp6;

static TaskHandle_t xTaskToNotify = NULL;

void measure(void *pvParameters)
{
    //measure_task_params *params = (measure_task_params *) pvParameters;

    xTaskToNotify = xTaskGetCurrentTaskHandle();
    static measure_cell grid[SIZE_X * SIZE_Y * 4];
    static measure_group measurements;

    uint16_t freq_khz;
    uint32_t diff;
    uint8_t ident;

    float amplitude;
    float norm_ang_freq = NAF_C * 610;
    float I;
    float Q;

    char buffer[50];

    int16_t *ADC;
    uint16_t *IC;

    uint16_t cell_index;
    uint32_t notify_bits;

    xTaskNotifyGive(xTaskToNotify);

    while (1) {
        for (uint8_t cell_x = 0; cell_x < SIZE_X; cell_x ++){
            for (uint8_t cell_y = 0; cell_y < SIZE_Y; cell_y ++) {

                measure_collect(&measurements, cell_x, cell_y);

                xTaskNotifyWait(
                    EV_ALL,
                    0,
                    &notify_bits,
                    portMAX_DELAY
                );

                if ((notify_bits & EV_ALL) == EV_ALL) {
                    HAL_GPIO_WritePin(TP906_GPIO_Port, TP906_Pin, SET);
                    for (uint8_t channel = 0; channel < 4; channel ++) {
                        I = 0;
                        Q = 0;
                        amplitude = 0;
        
                        cell_index = cell_x + (cell_y * SIZE_X) + (channel * 256);
                        ADC = measurements.samples[channel].ADC_buffer;
                        IC  = measurements.samples[channel].IC_buffer; 
    
        
                        if (IC[1] == IC[0]) {
                            freq_khz = 0;
                        }else if (IC[1] > IC[0]) {
                            diff = (IC[1] - IC[0]);
                            freq_khz = (CLK_KHZ * IC_PSC / diff);
                        } else {
                            diff = (0x10000 + IC[1] - IC[0]);
                            freq_khz = (CLK_KHZ * IC_PSC / diff);
                        }
        
                        norm_ang_freq = freq_khz * NAF_C;
                        for (uint16_t sample = 0; sample < N_SAMPLES; sample ++) {
                            I += arm_cos_f32(norm_ang_freq * sample) * ADC[sample];
                            Q += arm_sin_f32(norm_ang_freq * sample) * ADC[sample];
                        }
                        amplitude = sqrtf(I*I + Q*Q);
    
                        if (amplitude < 150000) {
                            ident = 255;
                        } else if ((freq_khz >= F_8_8) & (freq_khz < F_7_8)) {
                            ident = 8;
                        } else if ((freq_khz >= F_7_8) & (freq_khz < F_6_7)) {
                            ident = 7;
                        } else if ((freq_khz >= F_6_7) & (freq_khz < F_5_6)) {
                            ident = 6;
                        } else if ((freq_khz >= F_5_6) & (freq_khz < F_4_5)) {
                            ident = 5;
                        } else if ((freq_khz >= F_4_5) & (freq_khz < F_3_4)) {
                            ident = 4;
                        } else if ((freq_khz >= F_3_4) & (freq_khz < F_2_3)) {
                            ident = 3;
                        } else if ((freq_khz >= F_2_3) & (freq_khz < F_1_2)) {
                            ident = 2;
                        } else if ((freq_khz >= F_1_2) & (freq_khz < F_0_1)) {
                            ident = 1;
                        } else if ((freq_khz >= F_0_1) & (freq_khz < F_0_0)) {
                            ident = 0;
                        } else {
                            ident = 255;
                        }
    
                        //float scaled = (amplitude * 255.0f) / 1500000.0f;
                        //if (scaled < 0.0f)      scaled = 0.0f;
                        //else if (scaled > 255.0f) scaled = 255.0f;
                        //grid[cell_index].amplitude = (uint8_t)(scaled);
    
                        grid[cell_index].amplitude = (uint8_t)((amplitude * 255)/2500000);
                        grid[cell_index].frequency = ident;
    
                    }
                    HAL_GPIO_WritePin(TP906_GPIO_Port, TP906_Pin, RESET);
                }
            }
        }

        uint8_t max = 0;
        uint16_t max_index = 0;
        for (uint16_t i = 0; i < 1024; i ++) {
            if (grid[i].amplitude > max && grid[i].frequency != 255) {
                max_index = i;
                max = grid[i].amplitude;
            }
        }

        ssd1306_Fill(White);
        sprintf(buffer, "Pos: %u %u", max_index % 32, max_index / 32);
        ssd1306_SetCursor(6, 20);
        ssd1306_WriteString(buffer, Font_6x8, Black);
        sprintf(buffer, "ID: %u  %u", grid[max_index].frequency, grid[max_index].amplitude);
        ssd1306_SetCursor(6, 32);
        ssd1306_WriteString(buffer, Font_6x8, Black);
        ssd1306_UpdateScreen();
    }
    vTaskDelete(NULL);
}

void measure_collect(measure_group *measurements, uint8_t cell_x, uint8_t cell_y)
{
    uint32_t frame = 1U << cell_x;

    ///TODO: bounds checking
    row_to_mux(cell_y);

    taskENTER_CRITICAL();
    HAL_SPI_Transmit(&hspi1, (uint8_t *) &frame, 4, 1000);
    __ASM("nop");
    HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TX_RCLK_GPIO_Port, TX_RCLK_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_SET);

    for (uint8_t j = 0; j < 60; j ++) {
        __ASM("nop");
    }

    HAL_GPIO_WritePin(TX_SLEEP_GPIO_Port, TX_SLEEP_Pin, GPIO_PIN_RESET);

    for (uint8_t j = 0; j < 40; j ++) {
        __ASM("nop");
    }

    /*
    RX1 - ADC4 - COMP6 - TIM16
    RX2 - ADC3 - COMP4 - TIM4
    RX3 - ADC2 - COMP5 - TIM17
    RX4 - ADC1 - COMP3 - TIM15
    */

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) measurements->samples[3].ADC_buffer, N_SAMPLES);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *) measurements->samples[2].ADC_buffer, N_SAMPLES);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *) measurements->samples[1].ADC_buffer, N_SAMPLES);
    HAL_ADC_Start_DMA(&hadc4, (uint32_t *) measurements->samples[0].ADC_buffer, N_SAMPLES);

    HAL_TIM_IC_Start_DMA(&htim16, TIM_CHANNEL_1, (uint32_t*) measurements->samples[0].IC_buffer, 2);
    HAL_TIM_IC_Start_DMA(&htim4 , TIM_CHANNEL_2, (uint32_t*) measurements->samples[1].IC_buffer, 2);
    HAL_TIM_IC_Start_DMA(&htim17, TIM_CHANNEL_1, (uint32_t*) measurements->samples[2].IC_buffer, 2);
    HAL_TIM_IC_Start_DMA(&htim15, TIM_CHANNEL_1, (uint32_t*) measurements->samples[3].IC_buffer, 2);

    taskEXIT_CRITICAL();
}

void row_to_mux(uint8_t row)
{
    switch (row)
    {
    case 0:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_RESET);
        break;
    case 3:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_RESET);
        break;
    case 4:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_SET);
        break;
    case 5:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_SET);
        break;
    case 6:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_SET);
        break;
    case 7:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_SET);
        break;
    default:
        HAL_GPIO_WritePin(MUX_A_GPIO_Port, MUX_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_B_GPIO_Port, MUX_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_C_GPIO_Port, MUX_C_Pin, GPIO_PIN_RESET);
        break;
    }
    __ASM("nop");
    __ASM("nop");
    __ASM("nop");
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    BaseType_t woken = pdFALSE;

    if (hadc->Instance == ADC1) {
        HAL_ADC_Stop_DMA(&hadc1);
        xTaskNotifyFromISR(xTaskToNotify, EV_ADC0, eSetBits, &woken);
    } else if (hadc->Instance == ADC2) {
        HAL_ADC_Stop_DMA(&hadc2);
        xTaskNotifyFromISR(xTaskToNotify, EV_ADC1, eSetBits, &woken);
    } else if (hadc->Instance == ADC3) {
        HAL_ADC_Stop_DMA(&hadc3);
        xTaskNotifyFromISR(xTaskToNotify, EV_ADC2, eSetBits, &woken);
    } else if (hadc->Instance == ADC4) {
        HAL_ADC_Stop_DMA(&hadc4);
        xTaskNotifyFromISR(xTaskToNotify, EV_ADC3, eSetBits, &woken);
    }


    portYIELD_FROM_ISR(woken);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    BaseType_t woken = pdFALSE;
    if (htim->Instance == TIM16) {
        HAL_TIM_IC_Stop_DMA(&htim16, TIM_CHANNEL_1);
        xTaskNotifyFromISR(xTaskToNotify, EV_IC0, eSetBits, &woken);
    } else if (htim->Instance == TIM4) {
        HAL_TIM_IC_Stop_DMA(&htim4, TIM_CHANNEL_2);
        xTaskNotifyFromISR(xTaskToNotify, EV_IC1, eSetBits, &woken);
    } else if (htim->Instance == TIM15) {
        HAL_TIM_IC_Stop_DMA(&htim15, TIM_CHANNEL_1);
        xTaskNotifyFromISR(xTaskToNotify, EV_IC2, eSetBits, &woken);
    } else if (htim->Instance == TIM17) {
        HAL_TIM_IC_Stop_DMA(&htim17, TIM_CHANNEL_1);
        xTaskNotifyFromISR(xTaskToNotify, EV_IC3, eSetBits, &woken);
    }
    // …TIM17→EV_IC2, TIM15→EV_IC3
    portYIELD_FROM_ISR(woken);
}
