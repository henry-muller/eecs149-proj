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

#include "accelerometer_handler.h"

#define ADC_SCALING_FACTOR 1137.778 // See page 358 of nRF52832 Product Specification for details
// ADC_OUTPUT =  [V(P) – V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
// ADC_OUTPUT = V * 1137.778

static nrf_saadc_channel_config_t adc_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);

static void saadc_callback(nrfx_saadc_evt_t const * p_event) {
    // NRFX_IRQ_DISABLE(SAADC_IRQn);
    //printf("IRQ\n");
    if (p_event->type == NRFX_SAADC_EVT_LIMIT && p_event->data.limit.channel == ACCELEROMETER_ADC_CHANNEL) {
        printf("Limit Interrupt\n");
        handle_accelerometer_interrupt();
    }
    // NRFX_IRQ_ENABLE(SAADC_IRQn);
   //printf("Done with IRQ\n");
}

static bool _is_adc_initialized = false;

bool is_adc_initialized() {
    return _is_adc_initialized;
}

void initialize_adc() {
    nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
    saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
    ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
    APP_ERROR_CHECK(error_code);
    _is_adc_initialized = true;
}

void initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel) {
    adc_channel_config.pin_p = pin;
    ret_code_t error_code = nrfx_saadc_channel_init(channel, &adc_channel_config);
    APP_ERROR_CHECK(error_code);
}

void initialize_adc_channel_with_limits(nrf_saadc_input_t pin, uint8_t channel, int16_t limit_low, int16_t limit_high) {
    initialize_adc_channel(pin, channel);
    nrfx_saadc_limits_set(channel, limit_low, limit_high);
}

// Sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_adc_value(uint8_t channel) {
    nrf_saadc_value_t val;
    ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
    APP_ERROR_CHECK(error_code);
    return val;
}
