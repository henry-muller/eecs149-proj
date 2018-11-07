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
#include "nrf_drv_pwm.h"
//#include "nrf_drv_clock.h"
#include "nrfx_pwm.h"

#include "buckler.h"
#include "pin_assignments.h"

#define SENSOR_0_ADC_CHANNEL 0
#define SENSOR_1_ADC_CHANNEL 1
#define SENSOR_2_ADC_CHANNEL 2
#define SENSOR_3_ADC_CHANNEL 3
#define SENSOR_4_ADC_CHANNEL 4

#define ADC_SCALING_FACTOR 1137.778 // See page 358 of nRF52832 Product Specification for details
// ADC_OUTPUT =  [V(P) – V(N)] * GAIN/REFERENCE * 2^(RESOLUTION - m)
// ADC_OUTPUT = V * 1137.778

#define DIVIDER_RESISTANCE 47000
// Resistor value used in every resistive divider with the flex sensors

#define NUMBER_OF_SENSORS 5

uint8_t OUTPUT_PIN = NRF_GPIO_PIN_MAP(0, 17);

nrf_saadc_value_t flex_sensor_readings[NUMBER_OF_SENSORS];
nrf_saadc_value_t flex_sensor_thresholds[NUMBER_OF_SENSORS];
bool is_flexed[NUMBER_OF_SENSORS];

// callback for SAADC events
void saadc_callback(nrfx_saadc_evt_t const * p_event) {
    // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value(uint8_t channel) {
    nrf_saadc_value_t val;
    ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
    APP_ERROR_CHECK(error_code);
    return val;
}

void update_flex_sensor_readings() {
    flex_sensor_readings[0] = sample_value(SENSOR_0_ADC_CHANNEL);
    flex_sensor_readings[1] = sample_value(SENSOR_1_ADC_CHANNEL);
    flex_sensor_readings[2] = sample_value(SENSOR_2_ADC_CHANNEL);
    flex_sensor_readings[3] = sample_value(SENSOR_3_ADC_CHANNEL);
    flex_sensor_readings[4] = sample_value(SENSOR_4_ADC_CHANNEL);
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

float adc_input_voltage(nrf_saadc_value_t adc_reading) {
    return adc_reading / ADC_SCALING_FACTOR;
}

float flex_resistance_kohms(float voltage) {
    // Supply voltage = 5 V. That's where the magic number 5 comes from.
    return (DIVIDER_RESISTANCE/5 * voltage)/(1 - voltage/5)/1000;
}

void display_readings() {
    int i;
    float voltage;
    float resistance;
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        voltage = adc_input_voltage(flex_sensor_readings[i]);
        resistance = flex_resistance_kohms(voltage);
        printf("sample %d: %d | %f V | %f kOhms\n", i, flex_sensor_readings[i], voltage, resistance);

    }
}

void update_sensor_thresholds() {
    int i;
    int j;
    int32_t sums[NUMBER_OF_SENSORS];
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        sums[i] = 0;
    }
    int32_t count = 10; // number of samples to take
    printf("Preparing to calibrate sensors. Please bend all fingers.\n");
    nrf_delay_ms(2000);
    printf("Calibrating...\n");
    for (i = 0; i < count; i++) {
        update_flex_sensor_readings();
        for (j = 0; j < NUMBER_OF_SENSORS; j++) {
            sums[j] += flex_sensor_readings[j];
            nrf_delay_ms(10);
        }
    }
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        flex_sensor_thresholds[i] = (nrf_saadc_value_t) (sums[i]/count);
    }
}

bool is_single_sensor_flexed(int sensor_number) {
    // Returns true if the ADC reading off the sensor is above 80% of the sensor's calibrated threshold value.
    return flex_sensor_readings[sensor_number] >= 0.8 * flex_sensor_thresholds[sensor_number];
}

void update_flexed() {
    int i;
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        is_flexed[i] = is_single_sensor_flexed(i);
    }
}

static nrf_drv_pwm_t pwm_0 = NRF_DRV_PWM_INSTANCE(0);
nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};
nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};

void pwm_update_duty_cycle(uint8_t duty_cycle)
{
    
    // Check if value is outside of range. If so, set to 100%
    if(duty_cycle >= 100)
    {
        seq_values->channel_0 = 100;
    }
    else
    {
        seq_values->channel_0 = duty_cycle;
    }
    
    nrf_drv_pwm_simple_playback(&pwm_0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}


static void pwm_init(void)
{
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            OUTPUT_PIN, // channel 0
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_250kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm_0, &config0, NULL));
    
}


nrf_drv_pwm_config_t pwm_config = NRFX_PWM_DEFAULT_CONFIG;
NRFX_PWM_DEFAULT_CONFIG_OUT0_PIN


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

    // initialize GPIO driver
    if (!nrfx_gpiote_is_init()) {
        error_code = nrfx_gpiote_init();
    }
    APP_ERROR_CHECK(error_code);
    
    uint8_t led1 = NRF_GPIO_PIN_MAP(0, 17);
    uint8_t led2 = NRF_GPIO_PIN_MAP(0, 18);

    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
    error_code = nrfx_gpiote_out_init(led1, &out_config);
    APP_ERROR_CHECK(error_code);
    error_code = nrfx_gpiote_out_init(led2, &out_config);
    APP_ERROR_CHECK(error_code);
    
    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
        ;
    
    pwm_init();

    /*
    for (;;)
    {
        for(int i = 0; i <= 100; i++)
        {
            nrf_delay_ms(10);
            pwm_update_duty_cycle(i);
        }
    }*/

    pwm_update_duty_cycle(50);
    
    /*
    while(1) {
        nrf_gpio_pin_toggle(led1);
        nrf_gpio_pin_toggle(led2);
        nrf_delay_ms(400);
        printf("HELLO\n");
    } */
    
    /*
    APP_PWM_INSTANCE(PWM0, 1);
    app_pwm_config_t pwm0_config = APP_PWM_DEFAULT_CONFIG_1CH(5000L, 17);
    error_code = app_pwm_init(&PWM0, &pwm0_config, pwm_ready_callback);
    APP_ERROR_CHECK(error_code);
    app_pwm_enable(&PWM0);
    uint32_t value;
    printf("You should see this\n");
    while(true)
    {
        for (uint8_t i = 0; i < 40; ++i)
        {
            value = (i < 20) ? (i * 5) : (100 - (i - 20) * 5);
            
            // Set the duty cycle - keep trying until PWM is ready. 
            while (app_pwm_channel_duty_set(&PWM0, 0, value) == NRF_ERROR_BUSY);
            nrf_delay_ms(25);
        }
    }
    printf("You should not see this\n");
    */


    int i;
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
