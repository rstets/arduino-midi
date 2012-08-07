#ifndef MIDI_HELPERS_H
#define MIDI_HELPERS_H

#include "global.h"
#include "Arduino.h"

// Number if MIDI synths
#define NUM_SYNTHS 2
// MIDI baud rate
#define MIDI_BAUD_RATE 31250
// Extra velocity
#define ADD_VELOCITY 64
// Upper value limit is 127
#define UPPER_LIMIT 0x007F

// Interfaces
void midiSetup(unsigned int octave_step);
void sendMidiMessage(unsigned int cmd, unsigned int pitch, unsigned int velocity);
void playSynth(unsigned int i, unsigned int note, unsigned int velocity);

#endif
