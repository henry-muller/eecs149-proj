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
#include "nrf_drv_pwm.h"

#include "types.h"

#define INSTRUMENT_OUTPUT_PIN NRF_GPIO_PIN_MAP(0, 17)
#define PWM_CONFIG(frequency_hz) { .output_pins = {INSTRUMENT_OUTPUT_PIN, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, }, .irq_priority = APP_IRQ_PRIORITY_LOWEST, .base_clock = NRF_PWM_CLK_125kHz, .count_mode = NRF_PWM_MODE_UP, .top_value = ((int) (125000)/frequency_hz), .load_mode = NRF_PWM_LOAD_INDIVIDUAL, .step_mode = NRF_PWM_STEP_AUTO}
#define NUMBER_OF_NOTES 8

// Macros that evaluate at compile time to config structs with desired frequencies
static nrf_drv_pwm_config_t const pwm_config_C4 = PWM_CONFIG(261.6256);
static nrf_drv_pwm_config_t const pwm_config_D4 = PWM_CONFIG(293.6648);
static nrf_drv_pwm_config_t const pwm_config_E4 = PWM_CONFIG(329.6276);
static nrf_drv_pwm_config_t const pwm_config_F4 = PWM_CONFIG(349.2282);
static nrf_drv_pwm_config_t const pwm_config_G4 = PWM_CONFIG(391.9954);
static nrf_drv_pwm_config_t const pwm_config_A4 = PWM_CONFIG(440.0000);
static nrf_drv_pwm_config_t const pwm_config_B4 = PWM_CONFIG(493.8833);
static nrf_drv_pwm_config_t const pwm_config_C5 = PWM_CONFIG(523.2511);

static const nrf_drv_pwm_config_t* config_struct_pointers[NUMBER_OF_NOTES] = {
    &pwm_config_C4,
    &pwm_config_D4,
    &pwm_config_E4,
    &pwm_config_F4,
    &pwm_config_G4,
    &pwm_config_A4,
    &pwm_config_B4,
    &pwm_config_C5
};

static const nrf_drv_pwm_config_t* get_config_struct_pointer(musical_note_t note) {
    return config_struct_pointers[note];
}

static nrf_drv_pwm_t pwm = NRF_DRV_PWM_INSTANCE(0); // The only PWM instance we ever use

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

static void pwm_init(const nrf_drv_pwm_config_t* pwm_config_ptr) {
    // Pass in a pointer to the config struct corresponding to your desired frequency
    // Init PWM without error handler:
    APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm, pwm_config_ptr, NULL));
    //APP_ERROR_CHECK(nrf_drv_pwm_init(&pwm, &pwm_config_C4, NULL));
    is_pwm_initialized = true;
}

static void pwm_uninit() {
    nrf_drv_pwm_uninit(&pwm);
    is_pwm_initialized = false;
}

static void pwm_playback() {
    nrf_drv_pwm_simple_playback(&pwm, &pwm_duty_cycle_sequence, 1, NRF_DRV_PWM_FLAG_LOOP);
}

static void pwm_start(const nrf_drv_pwm_config_t* pwm_config_ptr) {
    if (is_pwm_initialized) {
        pwm_uninit();
    }
    //printf("PWM line 90\n");
    pwm_init(pwm_config_ptr); // Re-init PWM with new frequency
    //printf("PWM line 92");
    pwm_playback();
}

static void start_note(musical_note_t note) {
    pwm_start(get_config_struct_pointer(note));
}

static void stop_note(musical_note_t note) {
    if (is_pwm_initialized) {
        pwm_uninit();
    }
}

void pwm_instrument_init() {
    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

void pwm_instrument_play(instrument_state_t* state) {
    int i = 0;
    bool still_looking = true;
    while (i < 5 && still_looking) {
        if (state->notes_to_play[i]!= NO_NOTE) {
            printf("i=%d, REQUESTED NOTE %d\n", i, state->notes_to_play[i]);
            start_note(state->notes_to_play[i]);
            still_looking = false;
        } else {
            stop_note(C4);
        }
        i++;
    }
}
