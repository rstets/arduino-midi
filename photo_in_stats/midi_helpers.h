#ifndef MIDI_HELPERS_H
#define MIDI_HELPERS_H

#include "global.h"
#include "Arduino.h"

// Interfaces
void midiSetup(int octave_start, int octave_step);
void sendMidiMessage(int cmd, int pitch, int velocity);
void noteOn(int i, int note, int velocity);
void noteOff(int i);

#endif
