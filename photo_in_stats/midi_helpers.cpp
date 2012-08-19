#include "midi_helpers.h"

// MIDI baud rate
#define MIDI_BAUD_RATE 31250
// Extra velocity
#define ADD_VELOCITY 0
// Minimal velocity
#define MIN_VELOCITY 0
// Upper value limit is 127
#define UPPER_LIMIT 0x007F // 0x7F

// Constants
const int NOTE_ON = B10010000;
const int NOTE_OFF = B10000000;
const int CC = B10110000;

// MIDI "constants"
int MIDI_CH[NUM_SYNTHS];
int OCTAVE[NUM_SYNTHS];

// Cache currently playing MIDI note
int note_on[NUM_SYNTHS];


void midiSetup(int octave_start, int octave_step) {
  // Initialize MIDI "constants"
  int midi_channel = 0;
  int octave = 12;
  for (int i = octave_start; i < NUM_SYNTHS; i++) {
    note_on[i] = 0;
    MIDI_CH[i] = midi_channel;
    midi_channel++;
    
    OCTAVE[i] = octave;
    octave += octave_step;
  }
  
  // If not debugging
  if (DEBUG == 0) {
    // Set up MIDI output
    Serial.begin(MIDI_BAUD_RATE);
  }    
}

void sendMidiMessage(int cmd, int pitch, int velocity) {
  if (DEBUG == 0) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity);
  }
}

void noteOn(int i, int note_in, int velocity_in) {
  // Go
  if (note_on[i] == 0 && velocity_in > MIN_VELOCITY) {
    note_in += OCTAVE[i];
    if (note_in > UPPER_LIMIT) {
      note_in = UPPER_LIMIT;
    }
    velocity_in += ADD_VELOCITY;
    if (velocity_in > UPPER_LIMIT) {
      velocity_in = UPPER_LIMIT;
    }

    if (DEBUG > 0) {
      Serial.println("\n");
      Serial.print("Note on: ");
      Serial.print(note_in);
      Serial.print(", ");
      Serial.print(velocity_in);
    }
    sendMidiMessage(NOTE_ON | MIDI_CH[i], note_in, velocity_in);
    note_on[i] = note_in;
  }
}

void noteOff(int i) {
  if (note_on[i] > 0) {
    if (DEBUG > 0) {
      Serial.println("\n");
      Serial.print("Note off: ");
      Serial.print(note_on[i]);
    }
    sendMidiMessage(NOTE_OFF | MIDI_CH[i], note_on[i], 0);
    note_on[i] = 0;
  }
}
