#ifndef FLEX_SENSOR_HANDLER_H
#define FLEX_SENSOR_HANDLER_H

#include <stdbool.h>
// #include <stdint.h>
// #include <stdio.h>

// #include "app_error.h"
// #include "nrf.h"
// #include "nrf_delay.h"
// #include "nrf_gpio.h"
// #include "nrf_log.h"
// #include "nrf_log_ctrl.h"
// #include "nrf_log_default_backends.h"
// #include "nrf_pwr_mgmt.h"
// #include "nrf_serial.h"
// #include "nrfx_gpiote.h"
// #include "nrfx_saadc.h"

void update_flex_sensor_thresholds();

bool is_sensor_flexed(int sensor_number);

void initialize_flex_sensors();

#endif /* FLEX_SENSOR_HANDLER_H */
