// testing flex sensors
//
//

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

#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1

// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
    // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value (uint8_t channel) {
    nrf_saadc_value_t val;
    ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
    APP_ERROR_CHECK(error_code);
    return val;
}

void update_sensor_readings();

int main() {
    ret_code_t error_code = NRF_SUCCESS;

    // initialize RTT library
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // initialize analog to digital converter
    nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
    saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
    error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
    APP_ERROR_CHECK(error_code);

    // initialize analog inputs
    // configure with 0 as input pin for now
    nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
    channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
    channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

     // initialize ADC channel 0 with pin AIN0
    channel_config.pin_p = NRF_SAADC_INPUT_AIN0;
    error_code = nrfx_saadc_channel_init(ADC_CHANNEL_0, &channel_config);
    APP_ERROR_CHECK(error_code);

    // initialize ADC channel 1 with pin AIN1
    channel_config.pin_p = NRF_SAADC_INPUT_AIN1;
    error_code = nrfx_saadc_channel_init(ADC_CHANNEL_1, &channel_config);
    APP_ERROR_CHECK(error_code);

    // float resistor = 47000.0;
    // float straight_resistance = 14000.0;
    // float bend_resistance = 27000.0;
    // float VCC = 5.0;

    while (1) {
        printf("Sampling...\n");
        nrf_saadc_value_t sample_0 = sample_value(ADC_CHANNEL_0);
        nrf_saadc_value_t sample_1 = sample_value(ADC_CHANNEL_1);
        // float flexV = sample * VCC / 1023.0;
        // float flexR = resistor * (VCC / (flexV - 1.0));
        printf("sample_0: %d\n", sample_0);
        printf("sample_1: %d\n", sample_1);
        // printf("resistance: %f\n", flexR);
        // float angle = (flexR - straight_resistance) / 144.4;
        // printf("angle: %f\n", angle);
        nrf_delay_ms(1000);
    }

}
