#ifndef MEASURE_H
#define MEASURE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"
#include "main.h"
#include "hardware.h"

typedef struct {
    QueueHandle_t output;
} measure_task_params;

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
    measure_cell cells [GRID_COLS * GRID_COLS];     
} measure_grid;

void measure(void *pvParameters);

void measure_start_collection(measure_sample *buffer, uint8_t index);

#endif
