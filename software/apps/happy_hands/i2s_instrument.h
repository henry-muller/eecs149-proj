#ifndef I2S_INSTRUMENT_H
#define I2S_INSTRUMENT_H

#include "types.h"

void i2s_instrument_init();

void i2s_instrument_play(instrument_state_t *state);

void i2s_instrument_init_hal();

void i2s_instrument_play_hal(instrument_state_t *state);

#endif /* I2S_INSTRUMENT_H */
