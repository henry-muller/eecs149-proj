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

#define ACCELEROMETER_INPUT_PIN NRF_SAADC_INPUT_AIN7
#define ACCELEROMETER_ADC_CHANNEL 7

//high > 1800
//middle btwn [1400, 1800]
//low < 1400
nrf_saadc_value_t get_accelerometer_adc() {
    nrf_saadc_value_t adc_value = sample_adc_value(ACCELEROMETER_ADC_CHANNEL);
    return adc_value;
    // if (adc_value >= ROTARY_SWITCH_1_FLOOR) {
    //     return 1;
    // } else if (adc_value >= ROTARY_SWITCH_2_FLOOR) {
    //     return 2;
    // } else if (adc_value >= ROTARY_SWITCH_3_FLOOR) {
    //     return 3;
    // } else if (adc_value >= ROTARY_SWITCH_4_FLOOR) {
    //     return 4;
    // } else if (adc_value >= ROTARY_SWITCH_5_FLOOR) {
    //     return 5;
    // } else if (adc_value >= ROTARY_SWITCH_6_FLOOR) {
    //     return 6;
    // } else if (adc_value >= ROTARY_SWITCH_7_FLOOR) {
    //     return 7;
    // } else if (adc_value >= ROTARY_SWITCH_8_FLOOR) {
    //     return 8;
    // } else if (adc_value >= ROTARY_SWITCH_9_FLOOR) {
    //     return 9;
    // } else if (adc_value >= ROTARY_SWITCH_10_FLOOR) {
    //     return 10;
    // } else if (adc_value >= ROTARY_SWITCH_11_FLOOR) {
    //     return 11;
    // } else {
    //     return 12;
    // }
}

int get_volume_control() {
    nrf_saadc_value_t val = get_accelerometer_adc();
    if (val < 1400) {
        return -1;
    } else if (val > 1400 && val < 1800) {
        return 0;
    } else {
        return 1;
    }
}
void initialize_accelerometer() {
    initialize_adc_channel(ACCELEROMETER_INPUT_PIN, ACCELEROMETER_ADC_CHANNEL);
}
