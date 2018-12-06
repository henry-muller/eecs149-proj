#ifndef FLEX_SENSOR_HANDLER_H
#define FLEX_SENSOR_HANDLER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

#include "buckler.h"

/*#define NUMBER_OF_SENSORS 5

#define SENSOR_0_INPUT_PIN NRF_SAADC_INPUT_AIN3
#define SENSOR_1_INPUT_PIN NRF_SAADC_INPUT_AIN4
#define SENSOR_2_INPUT_PIN NRF_SAADC_INPUT_AIN5
#define SENSOR_3_INPUT_PIN NRF_SAADC_INPUT_AIN6
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN7

#define SENSOR_0_ADC_CHANNEL 1
#define SENSOR_1_ADC_CHANNEL 2
#define SENSOR_2_ADC_CHANNEL 3
#define SENSOR_3_ADC_CHANNEL 4
#define SENSOR_4_ADC_CHANNEL 5*/

#define NUMBER_OF_SENSORS 10

#define SENSOR_01_INPUT_PIN NRF_SAADC_INPUT_AIN2 //left pink and ring
#define SENSOR_23_INPUT_PIN NRF_SAADC_INPUT_AIN3 //left middle and pointer
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN1 //left thumb
#define SENSOR_5_INPUT_PIN NRF_SAADC_INPUT_AIN6 //right thumb
#define SENSOR_67_INPUT_PIN NRF_SAADC_INPUT_AIN4 // right pointer and middle
#define SENSOR_89_INPUT_PIN NRF_SAADC_INPUT_AIN5 // right ring and pinky

#define SENSOR_01_ADC_CHANNEL 2 //left pink and ring
#define SENSOR_23_ADC_CHANNEL 3 //left middle and pointer
#define SENSOR_4_ADC_CHANNEL 1 //left thumb
#define SENSOR_5_ADC_CHANNEL 6 //right thumb
#define SENSOR_67_ADC_CHANNEL 4 // right pointer and middle
#define SENSOR_89_ADC_CHANNEL 5 // right ring and pinky

#define S01_SEL NRF_GPIO_PIN_MAP(0,6) //pin 8
#define S23_SEL NRF_GPIO_PIN_MAP(0,7) //pin 9
#define S67_SEL NRF_GPIO_PIN_MAP(0,8) //pin 10
#define S89_SEL NRF_GPIO_PIN_MAP(0,11) //pin 11

#define ADC_SCALING_FACTOR 1137.778 // See page 358 of nRF52832 Product Specification for details
// ADC_OUTPUT =  [V(P) – V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
// ADC_OUTPUT = V * 1137.778

#define THRESHOLD_SLACK_FACTOR 0.98

void update_flex_sensor_thresholds();

bool is_sensor_flexed(int sensor_number);

void initialize_flex_sensors();

#endif /* FLEX_SENSOR_HANDLER_H */
