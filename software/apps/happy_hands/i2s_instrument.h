#ifndef I2S_INSTRUMENT_H
#define I2S_INSTRUMENT_H

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

#include "nrf_i2s.h"
#include "nrfx_i2s.h"

#include "types.h"

void i2s_instrument_init();

void i2s_instrument_play(instrument_state_t* state);

#endif /* I2S_INSTRUMENT_H */
