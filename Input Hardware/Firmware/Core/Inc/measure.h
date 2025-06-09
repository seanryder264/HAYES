#ifndef MEASURE_H
#define MEASURE_H

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc4;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

typedef struct {
    uint8_t index;
    uint16_t RX1_buffer[512];
    uint16_t RX2_buffer[512];
    uint16_t RX3_buffer[512];
    uint16_t RX4_buffer[512];
} measure_sample;

typedef struct {
    uint8_t frequency;      // (f/4), KHz
    uint8_t amplitude;      // (v/8), mV
} measure_cell;

typedef struct {
    measure_cell cells [32 * 32];
} measure_grid;

void measure(void *pvParameters);

void measure_start_collection(measure_sample *buffer, uint8_t index);

#endif
