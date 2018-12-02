#include "types.h"
#include "flex_sensor_handler.h"
#include "rotary_switch_handler.h"
#include "accelerometer_handler.h"

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

bool is_note_index_input_present(int note_index) {
    bool result = false;
    switch(note_index) {
        case 1:
            result = is_sensor_flexed(NOTE_1_SENSOR);
            break;
        case 2:
            result = is_sensor_flexed(NOTE_2_SENSOR);
            break;
        case 3:
            result = is_sensor_flexed(NOTE_3_SENSOR);
            break;
        case 4:
            result = is_sensor_flexed(NOTE_4_SENSOR);
            break;
        case 5:
            result = is_sensor_flexed(NOTE_5_SENSOR);
            break;
        case 6:
            result = is_sensor_flexed(NOTE_6_SENSOR);
            break;
        case 7:
            result = is_sensor_flexed(NOTE_7_SENSOR);
            break;
        case 8:
            result = is_sensor_flexed(NOTE_8_SENSOR);
            break;
    }
    return result;
}

pitch_bend_t get_pitch_bend() {
    pitch_bend_t result;
    if ((is_sensor_flexed(PITCH_BEND_DOWN_SENSOR) && is_sensor_flexed(PITCH_BEND_UP_SENSOR)) || (!is_sensor_flexed(PITCH_BEND_DOWN_SENSOR) && !is_sensor_flexed(PITCH_BEND_UP_SENSOR))) {
        result = NO_PITCH_BEND;
    } else if (is_sensor_flexed(PITCH_BEND_DOWN_SENSOR)) {
        result = PITCH_BEND_DOWN;
    } else { // is_sensor_flexed(PITCH_BEND_UP_SENSOR)
        result = PITCH_BEND_UP;
    }
    return result;
}

volume_level_t get_volume_level() {
    volume_command_t volume_command = adc_value_to_volume_command(get_)
}