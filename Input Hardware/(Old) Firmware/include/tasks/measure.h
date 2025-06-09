#ifndef MEASURE_H
#define MEASURE_H

#include "FreeRTOS.h"
#include "queue.h"

#include "stm32f3xx_hal.h"
#include "main.h"
#include "hardware.h"

typedef struct {
   uint8_t index;
} measure_task_params;

typedef struct {
    uint8_t frequency;      // (f/4), KHz
    uint8_t amplitude;      // (v/8), mV
} measure_cell;

typedef struct {
    measure_cell cells [GRID_COLS * GRID_COLS];     
} measure_grid;


#endif
