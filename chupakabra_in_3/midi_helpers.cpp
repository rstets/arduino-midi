#include "midi_helpers.h"

// Constants
const byte NOTE_ON = B10010000;
const byte NOTE_OFF = B10000000;
const byte CC = B10110000;

// MIDI "constants"
byte MIDI_CH[NUM_SYNTHS];
byte OCTAVE[NUM_SYNTHS];

// Cache currently playing MIDI note
byte note_on[NUM_SYNTHS];


void midiSetup(byte octave_start, byte octave_step) {
  // Initialize MIDI "constants"
  byte midi_channel = 0;
  byte octave = 12;
  for (byte i = octave_start; i < NUM_SYNTHS; i++) {
    note_on[i] = 0;
    MIDI_CH[i] = midi_channel;
    midi_channel++;
    
    OCTAVE[i] = octave;
    octave += octave_step;
  }
}

void sendMidiMessage(byte cmd, byte pitch, byte velocity) {
  if (DEBUG == 0) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity);
  }
}

void playSynth(byte i, byte note_in, byte velocity_in) {
  // Go
  if (note_on[i] == 0 && velocity_in > MIN_VELOCITY) {
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
