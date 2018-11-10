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

// #define NUMBER_OF_NOTES 10
#define INSTRUMENT_OUTPUT_PIN NRF_GPIO_PIN_MAP(0, 17)
#define PWM_CONFIG(frequency_hz) { .output_pins = {INSTRUMENT_OUTPUT_PIN, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, NRF_DRV_PWM_PIN_NOT_USED, }, .irq_priority = APP_IRQ_PRIORITY_LOWEST, .base_clock = NRF_PWM_CLK_125kHz, .count_mode = NRF_PWM_MODE_UP, .top_value = ((int) (125000)/frequency_hz), .load_mode = NRF_PWM_LOAD_INDIVIDUAL, .step_mode = NRF_PWM_STEP_AUTO}

typedef enum {
    C4,
    D4,
    E4,
    F4,
    G4,
    A4,
    B4,
    C5,
    NUMBER_OF_NOTES
} musical_note_t;

void start_note(musical_note_t note);

void stop_note(musical_note_t note);