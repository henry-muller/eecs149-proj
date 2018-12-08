// #include <stdbool.h>
// #include <stdint.h>
#include <stdio.h>

// #include "app_error.h"
// #include "nrf.h"
// #include "nrf_delay.h"
// #include "nrf_gpio.h"
// #include "nrf_log.h"
// #include "nrf_log_ctrl.h"
// #include "nrf_log_default_backends.h"
// #include "nrf_pwr_mgmt.h"
// #include "nrf_serial.h"
// #include "nrfx_gpiote.h"
#include "nrfx_saadc.h"
// #include "buckler.h"

#include "adc.h"

#define ROTARY_SWITCH_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define ROTARY_SWITCH_ADC_CHANNEL 0
#define ROTARY_SWITCH_1_FLOOR 3210
#define ROTARY_SWITCH_2_FLOOR 2920
#define ROTARY_SWITCH_3_FLOOR 2630
#define ROTARY_SWITCH_4_FLOOR 2330
#define ROTARY_SWITCH_5_FLOOR 2039
#define ROTARY_SWITCH_6_FLOOR 1740
#define ROTARY_SWITCH_7_FLOOR 1450
#define ROTARY_SWITCH_8_FLOOR 1155
#define ROTARY_SWITCH_9_FLOOR 859
#define ROTARY_SWITCH_10_FLOOR 565
#define ROTARY_SWITCH_11_FLOOR 270
#define ROTARY_SWITCH_12_FLOOR -23

static int get_instantaneous_rotary_switch_position() {
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

int get_rotary_switch_position() {
    int sum = 0;
    int count = 10;
    int i;
    for (i = 0; i < count; i++) {
        sum += get_instantaneous_rotary_switch_position();
    }
    return (int) (sum/count);

}

void initialize_rotary_switch() {
    initialize_adc_channel(ROTARY_SWITCH_INPUT_PIN, ROTARY_SWITCH_ADC_CHANNEL);
}
