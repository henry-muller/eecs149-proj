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
#include "nrf_drv_pwm.h"
//#include "nrf_drv_clock.h"
//#include "nrfx_pwm.h"

#include "buckler.h"
// #include "pin_assignments.h"
#include "pwm_instrument.h"

/* //define flex sensor readout pins
#define SENSOR_0_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define SENSOR_1_INPUT_PIN NRF_SAADC_INPUT_AIN1
#define SENSOR_2_INPUT_PIN NRF_SAADC_INPUT_AIN2
#define SENSOR_3_INPUT_PIN NRF_SAADC_INPUT_AIN3
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN4

#define SENSOR_0_ADC_CHANNEL 0
#define SENSOR_1_ADC_CHANNEL 1
#define SENSOR_2_ADC_CHANNEL 2
#define SENSOR_3_ADC_CHANNEL 3
#define SENSOR_4_ADC_CHANNEL 4 */

/* #define ADC_SCALING_FACTOR 1137.778 // See page 358 of nRF52832 Product Specification for details
// ADC_OUTPUT =  [V(P) – V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
// ADC_OUTPUT = V * 1137.778 */
#define DIVIDER_RESISTANCE 47000
// Resistor value used in every resistive divider with the flex sensors

// #define NUMBER_OF_SENSORS 5

/* #define PWM_OUTPUT_PIN NRF_GPIO_PIN_MAP(0, 17) */

/* // Don't look at this next line if you value your sanity
#define PWM_CONFIG(frequency_hz) { .output_pins = {PWM_OUTPUT_PIN, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, }, .irq_priority = APP_IRQ_PRIORITY_LOWEST, .base_clock = NRF_PWM_CLK_125kHz, .count_mode = NRF_PWM_MODE_UP, .top_value = ((int) (125000)/frequency_hz), .load_mode = NRF_PWM_LOAD_INDIVIDUAL, .step_mode = NRF_PWM_STEP_AUTO}
 */

// Macros that evaluate at compile time to config structs with desired frequencies
/* static nrf_drv_pwm_config_t const pwm_config_C4 = PWM_CONFIG(261.6256);
static nrf_drv_pwm_config_t const pwm_config_D4 = PWM_CONFIG(293.6648);
static nrf_drv_pwm_config_t const pwm_config_E4 = PWM_CONFIG(329.6276);
static nrf_drv_pwm_config_t const pwm_config_F4 = PWM_CONFIG(349.2282);
static nrf_drv_pwm_config_t const pwm_config_G4 = PWM_CONFIG(391.9954);
static nrf_drv_pwm_config_t const pwm_config_A4 = PWM_CONFIG(440.0000);
static nrf_drv_pwm_config_t const pwm_config_B4 = PWM_CONFIG(493.8833);
static nrf_drv_pwm_config_t const pwm_config_C5 = PWM_CONFIG(523.2511); */

// static nrf_saadc_value_t flex_sensor_readings[NUMBER_OF_SENSORS];
// static nrf_saadc_value_t flex_sensor_thresholds[NUMBER_OF_SENSORS];
// static bool is_flexed[NUMBER_OF_SENSORS];

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

// void saadc_callback(nrfx_saadc_evt_t const * p_event) {} // don't care about SAADC callbacks

// ret_code_t initialize_adc() {
//     nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
//     saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
//     ret_code_t error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
//     return error_code;
// }

// ret_code_t initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel, nrf_saadc_channel_config_t channel_config) {
//     channel_config.pin_p = pin;
//     ret_code_t error_code = nrfx_saadc_channel_init(channel, &channel_config);
//     return error_code;
// }

// // Sample a particular analog channel in blocking mode
// nrf_saadc_value_t sample_value(uint8_t channel) {
//     nrf_saadc_value_t val;
//     ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
//     APP_ERROR_CHECK(error_code);
//     return val;
// }

// void update_flex_sensor_readings() {
//     flex_sensor_readings[0] = sample_value(SENSOR_0_ADC_CHANNEL);
//     flex_sensor_readings[1] = sample_value(SENSOR_1_ADC_CHANNEL);
//     flex_sensor_readings[2] = sample_value(SENSOR_2_ADC_CHANNEL);
//     flex_sensor_readings[3] = sample_value(SENSOR_3_ADC_CHANNEL);
//     flex_sensor_readings[4] = sample_value(SENSOR_4_ADC_CHANNEL);
// }

// float adc_input_voltage(nrf_saadc_value_t adc_reading) {
//     return adc_reading / ADC_SCALING_FACTOR;
// }

// float flex_resistance_kohms(float voltage) {
//     // Supply voltage = 5 V. That's where the magic number 5 comes from.
//     return (DIVIDER_RESISTANCE/5 * voltage)/(1 - voltage/5)/1000;
// }

// void display_readings() {
//     int i;
//     float voltage;
//     float resistance;
//     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
//         voltage = adc_input_voltage(flex_sensor_readings[i]);
//         resistance = flex_resistance_kohms(voltage);
//         printf("sample %d: %d | %f V | %f kOhms\n", i, flex_sensor_readings[i], voltage, resistance);

//     }
// }

// void update_sensor_thresholds() {
//     int i;
//     int j;
//     int32_t sums[NUMBER_OF_SENSORS];
//     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
//         sums[i] = 0;
//     }
//     int32_t count = 10; // number of samples to take
//     printf("Preparing to calibrate sensors. Please bend all fingers.\n");
//     nrf_delay_ms(2000);
//     printf("Calibrating...\n");
//     for (i = 0; i < count; i++) {
//         update_flex_sensor_readings();
//         for (j = 0; j < NUMBER_OF_SENSORS; j++) {
//             sums[j] += flex_sensor_readings[j];
//             nrf_delay_ms(10);
//         }
//     }
//     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
//         flex_sensor_thresholds[i] = (nrf_saadc_value_t) (sums[i]/count);
//     }
// }

// bool is_single_sensor_flexed(int sensor_number) {
//     // Returns true if the ADC reading off the sensor is above 80% of the sensor's calibrated threshold value.
//     return flex_sensor_readings[sensor_number] >= 0.8 * flex_sensor_thresholds[sensor_number];
// }

// void update_flexed() {
//     int i;
//     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
//         is_flexed[i] = is_single_sensor_flexed(i);
//     }
// }

/* static nrf_drv_pwm_t pwm = NRF_DRV_PWM_INSTANCE(0); // The only PWM instance we ever use
static bool is_pwm_initialized = false;

static nrf_pwm_values_individual_t pwm_duty_cycle_sequence_values[] = {{25}};
// Changing this number changes the duty cycle.
// For musical purposes, this just changes the timbre of your note. They all sound pretty bad with square waves.
// 25 seemed less annoying than some other options.

static nrf_pwm_sequence_t const pwm_duty_cycle_sequence =
{
    .values.p_individual = pwm_duty_cycle_sequence_values,
    .length              = NRF_PWM_VALUES_LENGTH(pwm_duty_cycle_sequence_values),
    .repeats             = 0,
    .end_delay           = 0
};
// This "sequence" stuff is if you're interested in iterating over a range of duty cycles, which we are not.
// So we can ignore it.

void pwm_init(const nrf_drv_pwm_config_t* pwm_config_ptr) {
    // Pass in a pointer to the config struct corresponding to your desired frequency
    // Init PWM without error handler:
    APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm, pwm_config_ptr, NULL));
    is_pwm_initialized = true;
}

void pwm_uninit() {
    nrf_drv_pwm_uninit(&pwm);
    is_pwm_initialized = false;
}

void pwm_playback() {
    nrf_drv_pwm_simple_playback(&pwm, &pwm_duty_cycle_sequence, 1, NRF_DRV_PWM_FLAG_LOOP);
}

void pwm_start(const nrf_drv_pwm_config_t* pwm_config_ptr) {
    if (is_pwm_initialized) {
        pwm_uninit();
    }
    pwm_init(pwm_config_ptr); // Re-init PWM with new frequency
    pwm_playback();
} */

// void play_C_scale(int sustain_length_ms) {
//     start_note(C4);
//     nrf_delay_ms(sustain_length_ms);
//     stop_note(C4);
//     start_note(D4);
//     nrf_delay_ms(sustain_length_ms);
//     stop_note(D4);
//     start_note(E4);
//     nrf_delay_ms(sustain_length_ms);
//     stop_note(E4);
// }

int main() {
    //----------Initialization stuff--------------------------------------------------------------------------------
    ret_code_t error_code = NRF_SUCCESS;

    // initialize RTT library
    error_code = initialize_rtt();
    APP_ERROR_CHECK(error_code);

    // initialize ADC
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
    //----------End initialization stuff-----------------------------------------------------------------------------
    
    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    // int i;
    // for (i = 0; i < 10; i++) {
    //     play_C_scale(500);
    // }

    
    nrf_delay_ms(2000);
    printf("RTT working...\n");

    // Calibrate sensors
    update_sensor_thresholds(flex_sensor_readings, flex_sensor_thresholds);

    nrf_delay_ms(5000);

    while (1) {
        update_flex_sensor_readings();
        update_flexed();
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_flexed[i]);
        }
        printf("\n");
        nrf_delay_ms(1);
    }
}
