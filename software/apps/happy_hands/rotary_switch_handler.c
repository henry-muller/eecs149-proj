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

#define ROTARY_SWITCH_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define ROTARY_SWITCH_ADC_CHANNEL 0
// #define ROTARY_SWITCH_1_FLOOR 3200
// #define ROTARY_SWITCH_2_FLOOR 2925
// #define ROTARY_SWITCH_3_FLOOR 2630
// #define ROTARY_SWITCH_4_FLOOR 2338
// #define ROTARY_SWITCH_5_FLOOR 2047
// #define ROTARY_SWITCH_6_FLOOR 1755
// #define ROTARY_SWITCH_7_FLOOR 1457
// #define ROTARY_SWITCH_8_FLOOR 1160
// #define ROTARY_SWITCH_9_FLOOR 865
// #define ROTARY_SWITCH_10_FLOOR 570
// #define ROTARY_SWITCH_11_FLOOR 277
// #define ROTARY_SWITCH_12_FLOOR -19

#define ROTARY_SWITCH_1_FLOOR 3470
#define ROTARY_SWITCH_2_FLOOR 3250
#define ROTARY_SWITCH_3_FLOOR 2935
#define ROTARY_SWITCH_4_FLOOR 2600
#define ROTARY_SWITCH_5_FLOOR 2280
#define ROTARY_SWITCH_6_FLOOR 1960
#define ROTARY_SWITCH_7_FLOOR 1635
#define ROTARY_SWITCH_8_FLOOR 1305
#define ROTARY_SWITCH_9_FLOOR 975
#define ROTARY_SWITCH_10_FLOOR 655
#define ROTARY_SWITCH_11_FLOOR 325
#define ROTARY_SWITCH_12_FLOOR -5

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
