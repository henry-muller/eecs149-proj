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

// static int count = 0;
// static nrf_saadc_value_t last_value;

nrf_saadc_value_t get_accelerometer_adc() {
    // if (count == 100) {
    return sample_adc_value(ACCELEROMETER_ADC_CHANNEL);
    //     last_value = adc_value;
    //     count = 0;
    // } else {
    //     count++;
    // }
    // return last_value;
}

void initialize_accelerometer() {
    initialize_adc_channel_with_limits(ACCELEROMETER_INPUT_PIN, ACCELEROMETER_ADC_CHANNEL, 1400, 1800);
}
