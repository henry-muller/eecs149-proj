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

#include "types.h"

#include <nrf.h>

#define PIN_MCK    (13)
#define PIN_SCK    (14) // same as BCK
#define PIN_LRCK   (15)
#define PIN_SDOUT  (16) // same as DIN

void new_instrument_init() {
    return;
}
void new_instrument_play(instrument_state_t* state) {
    return;
}
