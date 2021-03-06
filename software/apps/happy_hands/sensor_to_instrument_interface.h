#ifndef SENSOR_TO_INSTRUMENT_INTERFACE_H
#define SENSOR_TO_INSTRUMENT_INTERFACE_H

#include "types.h"
#include "flex_sensor_handler.h"
#include "rotary_switch_handler.h"

#define NOTE_1_SENSOR 0
#define NOTE_2_SENSOR 1
#define NOTE_3_SENSOR 2
#define NOTE_4_SENSOR 3
#define NOTE_5_SENSOR 6
#define NOTE_6_SENSOR 7
#define NOTE_7_SENSOR 8
#define NOTE_8_SENSOR 9
#define PITCH_BEND_DOWN_SENSOR 4
#define PITCH_BEND_UP_SENSOR 5

musical_key_t get_key();

bool is_note_index_input_present(int note_index);

pitch_bend_t get_pitch_bend();

void raise_volume();

volume_command_t get_volume_command();

#endif /* SENSOR_TO_INSTRUMENT_INTERFACE_H */
