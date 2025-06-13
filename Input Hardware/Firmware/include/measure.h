#ifndef MEASURE_H
#define MEASURE_H

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "arm_math.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc4;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

typedef struct {
    int16_t ADC_buffer[256];
    uint16_t IC_buffer[2];
} measure_sample;

typedef struct {
	measure_sample samples[4];
} measure_group;

typedef struct {
    uint8_t frequency;      // (f/4), KHz
    uint8_t amplitude;      // (v/8), mV
} measure_cell;

void measure(void *pvParameters);

void measure_collect(measure_group *buffer, uint8_t cell_x, uint8_t cell_y);
void row_to_mux(uint8_t row);

#endif
