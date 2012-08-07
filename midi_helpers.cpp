#include "midi_helpers.h"

// Constants
const unsigned int NOTE_ON = B10010000;
const unsigned int NOTE_OFF = B10000000;
const unsigned int CC = B10110000;

// MIDI "constants"
unsigned int MIDI_CH[NUM_SYNTHS];
unsigned int OCTAVE[NUM_SYNTHS];

// Cache currently playing MIDI note
unsigned int note_on[NUM_SYNTHS] = {0, 0};


void midiSetup(unsigned int octave_step) {
  // Initialize MIDI "constants"
  unsigned int midi_channel = 0;
  unsigned int octave = 12;
  for (unsigned int i = 0; i < NUM_SYNTHS; i++) {
    MIDI_CH[i] = midi_channel;
    midi_channel++;
    
    OCTAVE[i] = octave;
    octave += octave_step;
  }
}

void sendMidiMessage(unsigned int cmd, unsigned int pitch, unsigned int velocity) {
  if (DEBUG == 0) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity);
  }
}

void playSynth(unsigned int i, unsigned int note_in, unsigned int velocity_in) {
  // Go
  if (note_on[i] == 0 && velocity_in > 8) {
    note_in += OCTAVE[i];
    note_in &= UPPER_LIMIT;
    velocity_in += ADD_VELOCITY;
    velocity_in &= UPPER_LIMIT;

    if (DEBUG > 0) {
      Serial.println("Note on");
      Serial.println(i);
      Serial.println(note_in);
      Serial.println(velocity_in);
    }
    sendMidiMessage(NOTE_ON | MIDI_CH[i], note_in, velocity_in);
    note_on[i] = note_in;
  }

  if (velocity_in <= 8 && note_on[i] > 0) {
    if (DEBUG > 0) {
      Serial.println("Note off");
      Serial.println(i);
      Serial.println(note_on[i]);
      Serial.println(velocity_in);
    }
    sendMidiMessage(NOTE_OFF | MIDI_CH[i], note_on[i], velocity_in);
    note_on[i] = 0;
  }
}
