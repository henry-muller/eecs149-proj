#ifndef PWM_INSTRUMENT_H
#define PWM_INSTRUMENT_H

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

void pwm_instrument_init();
void pwm_instrument_play(instrument_state_t* state);

#endif /* PWM_INSTRUMENT_H */
