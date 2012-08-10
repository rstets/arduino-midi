#ifndef MIDI_HELPERS_H
#define MIDI_HELPERS_H

#include "global.h"
#include "Arduino.h"

// Number if MIDI synths
#define NUM_SYNTHS 2
// MIDI baud rate
#define MIDI_BAUD_RATE 31250
// Extra velocity
#define ADD_VELOCITY 0
// Minimal velocity
#define MIN_VELOCITY 0
// Upper value limit is 127
#define UPPER_LIMIT B01111111 // 0x7F

// Interfaces
void midiSetup(int octave_start, int octave_step);
void sendMidiMessage(int cmd, int pitch, int velocity);
void playSynth(int i, int note, int velocity);

#endif
