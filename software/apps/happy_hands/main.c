// testing flex sensors

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

#define SENSOR_0_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define SENSOR_1_INPUT_PIN NRF_SAADC_INPUT_AIN1
#define SENSOR_2_INPUT_PIN NRF_SAADC_INPUT_AIN2
#define SENSOR_3_INPUT_PIN NRF_SAADC_INPUT_AIN3
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN4

#define SENSOR_0_ADC_CHANNEL 0
#define SENSOR_1_ADC_CHANNEL 1
#define SENSOR_2_ADC_CHANNEL 2
#define SENSOR_3_ADC_CHANNEL 3
#define SENSOR_4_ADC_CHANNEL 4

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

//raw ADC readings for each finger's flex sensor
typedef struct {
    nrf_saadc_value_t sensor_0;
    nrf_saadc_value_t sensor_1;
    nrf_saadc_value_t sensor_2;
    nrf_saadc_value_t sensor_3;
    nrf_saadc_value_t sensor_4;
} flex_sensor_readings;

void update_flex_sensor_readings(flex_sensor_readings* readings) {
    readings->sensor_0 = sample_value(SENSOR_0_ADC_CHANNEL);
    readings->sensor_1 = sample_value(SENSOR_1_ADC_CHANNEL);
    readings->sensor_2 = sample_value(SENSOR_2_ADC_CHANNEL);
    readings->sensor_3 = sample_value(SENSOR_3_ADC_CHANNEL);
    readings->sensor_4 = sample_value(SENSOR_4_ADC_CHANNEL);
}

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

ret_code_t initialize_adc() {
    nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
    saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
    ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
    return error_code;
}

ret_code_t initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel, nrf_saadc_channel_config_t channel_config) {
    // initialize ADC channel 0 with pin AIN0
    channel_config.pin_p = pin;
    ret_code_t error_code = nrfx_saadc_channel_init(channel, &channel_config);
    return error_code;
}

int main() {
    ret_code_t error_code = NRF_SUCCESS;

    // initialize RTT library
    error_code = initialize_rtt();
    APP_ERROR_CHECK(error_code);

    // initialize ADC
    error_code = initialize_adc();
    APP_ERROR_CHECK(error_code);

    // initialize analog inputs
    nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
    channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
    channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

    error_code = initialize_adc_channel(SENSOR_0_INPUT_PIN, SENSOR_0_ADC_CHANNEL, channel_config);
    APP_ERROR_CHECK(error_code);
    error_code = initialize_adc_channel(SENSOR_1_INPUT_PIN, SENSOR_1_ADC_CHANNEL, channel_config);
    APP_ERROR_CHECK(error_code);
    error_code = initialize_adc_channel(SENSOR_2_INPUT_PIN, SENSOR_2_ADC_CHANNEL, channel_config);
    APP_ERROR_CHECK(error_code);
    error_code = initialize_adc_channel(SENSOR_3_INPUT_PIN, SENSOR_3_ADC_CHANNEL, channel_config);
    APP_ERROR_CHECK(error_code);
    error_code = initialize_adc_channel(SENSOR_4_INPUT_PIN, SENSOR_4_ADC_CHANNEL, channel_config);
    APP_ERROR_CHECK(error_code);

    // float resistor = 47000.0;
    // float straight_resistance = 14000.0;
    // float bend_resistance = 27000.0;
    // float VCC = 5.0;
    
    flex_sensor_readings readings;

    while (1) {
        printf("Sampling...\n");
        update_flex_sensor_readings(&readings);

        // float flexV = sample * VCC / 1023.0;
        // float flexR = resistor * (VCC / (flexV - 1.0));
        printf("sample_0: %d\n", readings.sensor_0);
        printf("sample_1: %d\n", readings.sensor_1);
        // printf("resistance: %f\n", flexR);
        // float angle = (flexR - straight_resistance) / 144.4;
        // printf("angle: %f\n", angle);
        nrf_delay_ms(1000);
    }

}
