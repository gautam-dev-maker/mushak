#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum {
    IR_0 = 0,   // Leftmost IR sensor
    IR_1,
    IR_2,
    IR_3,
    IR_MAX
} IR_SENSOR;

HAL_StatusTypeDef adc_init();

uint32_t get_ir_readings(IR_SENSOR sensor);

uint32_t get_battery_reading();

#ifdef __cplusplus
}
#endif

#endif

