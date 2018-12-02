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

int get_accelerometer_adc() {
    nrf_saadc_value_t adc_value = sample_adc_value(ACCELEROMETER_ADC_CHANNEL);
    return adc_value;
}

volume_command_t adc_value_to_volume_command(nrf_saadc_value_t adc_value) { // Input: ADC value. Output: UP / DOWN / HOLD
    return NULL;
}

void initialize_accelerometer() {
    initialize_adc_channel(ACCELEROMETER_INPUT_PIN, ACCELEROMETER_ADC_CHANNEL);
}
