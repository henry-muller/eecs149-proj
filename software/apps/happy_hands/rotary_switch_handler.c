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
#include "adc.h"

#define ROTARY_SWITCH_INPUT_PIN NRF_SAADC_INPUT_AIN2
#define ROTARY_SWITCH_ADC_CHANNEL 0
#define ROTARY_SWITCH_1_FLOOR 
#define ROTARY_SWITCH_2_FLOOR 9999
#define ROTARY_SWITCH_3_FLOOR 9999
#define ROTARY_SWITCH_4_FLOOR 9999
#define ROTARY_SWITCH_5_FLOOR 9999
#define ROTARY_SWITCH_6_FLOOR 9999
#define ROTARY_SWITCH_7_FLOOR 9999
#define ROTARY_SWITCH_8_FLOOR 9999
#define ROTARY_SWITCH_9_FLOOR 9999
#define ROTARY_SWITCH_10_FLOOR 9999
#define ROTARY_SWITCH_11_FLOOR 9999
#define ROTARY_SWITCH_12_FLOOR 

int get_rotary_switch_position() {
    nrf_saadc_value_t adc_value = sample_adc_value(ROTARY_SWITCH_ADC_CHANNEL);
    if (adc_value >= ROTARY_SWITCH_1_FLOOR) {
        return 1;
    } else if (adc_value >= ROTARY_SWITCH_2_FLOOR) {
        return 2;
    } else if (adc_value >= ROTARY_SWITCH_3_FLOOR) {
        return 3;
    } else if (adc_value >= ROTARY_SWITCH_4_FLOOR) {
        return 4;
    } else if (adc_value >= ROTARY_SWITCH_5_FLOOR) {
        return 5;
    } else if (adc_value >= ROTARY_SWITCH_6_FLOOR) {
        return 6;
    } else if (adc_value >= ROTARY_SWITCH_7_FLOOR) {
        return 7;
    } else if (adc_value >= ROTARY_SWITCH_8_FLOOR) {
        return 8;
    } else if (adc_value >= ROTARY_SWITCH_9_FLOOR) {
        return 9;
    } else if (adc_value >= ROTARY_SWITCH_10_FLOOR) {
        return 10;
    } else if (adc_value >= ROTARY_SWITCH_11_FLOOR) {
        return 11;
    } else {
        return 12;
    }
}

void initialize_rotary_switch() {
    initialize_adc_channel(ROTARY_SWITCH_INPUT_PIN, ROTARY_SWITCH_ADC_CHANNEL);
}
