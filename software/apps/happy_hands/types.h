#ifndef TYPES_H
#define TYPES_H

#define NUMBER_OF_NOTE_INDICES 8
#define NUMBER_OF_NOTES 27

typedef enum {
    B3,

    C4,
    D4_FLAT,
    D4,
    E4_FLAT,
    E4,
    F4,
    G4_FLAT,
    G4,
    A4_FLAT,
    A4,
    B4_FLAT,
    B4,

    C5,
    D5_FLAT,
    D5,
    E5_FLAT,
    E5,
    F5,
    G5_FLAT,
    G5,
    A5_FLAT,
    A5,
    B5_FLAT,
    B5,

    C6,

    NO_NOTE
} musical_note_t;

typedef enum {
    PITCH_BEND_DOWN = -1,
    NO_PITCH_BEND,
    PITCH_BEND_UP
} pitch_bend_t;

typedef enum {
    C = 1,
    D_FLAT,
    D,
    E_FLAT,
    E,
    F,
    G_FLAT,
    G,
    A_FLAT,
    A,
    B_FLAT,
    B
} musical_key_t;

typedef enum {
    DOWN,
    HOLD,
    UP
} volume_command_t;

typedef struct {
    musical_note_t notes_to_play[NUMBER_OF_NOTE_INDICES];
    int volume_level;
} instrument_state_t;

#endif /* TYPES_H */