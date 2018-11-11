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
#include "adc.h"

#include "buckler.h"

// #include "instrument_player.h"

#define NUMBER_OF_SENSORS 5

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

#define ADC_SCALING_FACTOR 1137.778 // See page 358 of nRF52832 Product Specification for details
// ADC_OUTPUT =  [V(P) – V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
// ADC_OUTPUT = V * 1137.778

// static nrf_saadc_value_t flex_sensor_readings[NUMBER_OF_SENSORS];
static nrf_saadc_value_t flex_sensor_thresholds[NUMBER_OF_SENSORS];
// static bool is_flexed[NUMBER_OF_SENSORS];

// static void saadc_callback(nrfx_saadc_evt_t const * p_event) {} // don't care about SAADC callbacks

// static ret_code_t initialize_adc() {
//     nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
//     saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
//     ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
//     return error_code;
// }

// static ret_code_t initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel, nrf_saadc_channel_config_t channel_config) {
//     channel_config.pin_p = pin;
//     ret_code_t error_code = nrfx_saadc_channel_init(channel, &channel_config);
//     return error_code;
// }

// // Sample a particular analog channel in blocking mode
// static nrf_saadc_value_t sample_value(uint8_t channel) {
//     nrf_saadc_value_t val;
//     ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
//     APP_ERROR_CHECK(error_code);
//     return val;
// }

/*
void update_flex_sensor_readings() {
    flex_sensor_readings[0] = sample_value(SENSOR_0_ADC_CHANNEL);
    flex_sensor_readings[1] = sample_value(SENSOR_1_ADC_CHANNEL);
    flex_sensor_readings[2] = sample_value(SENSOR_2_ADC_CHANNEL);
    flex_sensor_readings[3] = sample_value(SENSOR_3_ADC_CHANNEL);
    flex_sensor_readings[4] = sample_value(SENSOR_4_ADC_CHANNEL);
} */

static nrf_saadc_value_t get_sensor_value(int sensor) {
    nrf_saadc_value_t result;
    switch(sensor) {
        case 0:
            result = sample_value(SENSOR_0_ADC_CHANNEL);
        case 1:
            result = sample_value(SENSOR_1_ADC_CHANNEL);
        case 2:
            result = sample_value(SENSOR_2_ADC_CHANNEL);
        case 3:
            result = sample_value(SENSOR_3_ADC_CHANNEL);
        case 4:
            result = sample_value(SENSOR_4_ADC_CHANNEL);
    }
    return result;
}

static bool is_above_threshold(int sensor_number, nrf_saadc_value_t sensor_value) {
    return sensor_value >= 0.8 * flex_sensor_thresholds[sensor_number];
}

// static note_index_t user_input_to_note_index(int user_input) {
//     note_index_t result;
//     if (user_input < 4) {
//         result = (note_index_t) user_input;
//     } else if (user_input > 5) {
//         result = (note_index_t) (user_input - 2);
//     }
//     return result;
// }

/*
void update_flex_sensor_handler() {
    int i;
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        is_flexed[i] = is_above_threshold(i);
    }
}
*/

void update_sensor_thresholds() {
    int i;
    int j;
    nrf_saadc_value_t sums[NUMBER_OF_SENSORS];
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        sums[i] = (nrf_saadc_value_t) 0;
    }
    int32_t count = 10; // number of samples to take
    printf("Preparing to calibrate sensors. Please bend all fingers.\n");
    nrf_delay_ms(2000);
    printf("Calibrating...\n");
    for (i = 0; i < count; i++) {
        for (j = 0; j < NUMBER_OF_SENSORS; j++) {
            sums[j] += get_sensor_value(j);
            nrf_delay_ms(10);
        }
    }
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        flex_sensor_thresholds[i] = (nrf_saadc_value_t) (sums[i]/count);
    }
}

bool is_sensor_flexed(int sensor_number) {
    return is_above_threshold(sensor_number, get_sensor_value(sensor_number));
}

void initialize_flex_sensors() {
    ret_code_t error_code = NRF_SUCCESS;
    error_code = initialize_adc();
    APP_ERROR_CHECK(error_code);

    // initialize analog inputs
    nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
    channel_config.gain = NRF_SAADC_GAIN1_6; // GAIN = 1.6
    channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // REFERENCE = 0.6

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
}
