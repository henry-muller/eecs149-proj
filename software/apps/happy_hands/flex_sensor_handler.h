#ifndef FLEX_SENSOR_HANDLER_H
#define FLEX_SENSOR_HANDLER_H

#include <stdbool.h>

void update_flex_sensor_thresholds();

bool is_sensor_flexed(int sensor_number);

void initialize_flex_sensors();

#endif /* FLEX_SENSOR_HANDLER_H */
