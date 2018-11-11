#include "flex_sensor_handler.h"
#include "rotary_switch_handler.h"
#include "instrument_player.h"  

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

musical_key_t get_key() {
    return (musical_key_t) get_rotary_switch_position();
}

bool is_note_index_input_present(note_index_t note_index) {
    bool result;
    switch(note_index) {
        case NOTE_1:
            result = is_sensor_flexed(NOTE_1_SENSOR);
        case NOTE_2:
            result = is_sensor_flexed(NOTE_2_SENSOR);
        case NOTE_3:
            result = is_sensor_flexed(NOTE_3_SENSOR);
        case NOTE_4:
            result = is_sensor_flexed(NOTE_4_SENSOR);
        case NOTE_5:
            result = is_sensor_flexed(NOTE_5_SENSOR);
        case NOTE_6:
            result = is_sensor_flexed(NOTE_6_SENSOR);
    }
}

bool is_pitch_bend_input_present(pitch_bend_t pitch_bend) {
    bool result;
    switch(pitch_bend) {
        case PITCH_BEND_DOWN:
            result = is_sensor_flexed(PITCH_BEND_DOWN_SENSOR);
        case PITCH_BEND_UP:
            result = is_sensor_flexed(PITCH_BEND_UP_SENSOR);
        case NO_PITCH_BEND:
            result = (!is_sensor_flexed(PITCH_BEND_UP) && !is_sensor_flexed(PITCH_BEND_DOWN)) || (is_sensor_flexed(PITCH_BEND_UP) && is_sensor_flexed(PITCH_BEND_DOWN));
    }
    return result;
}