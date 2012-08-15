// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE 0xFFF8

// Analog in numbers
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Number of inputs
#define NUM_INPUTS 4

// Runtime constants
const int ADJUST[NUM_INPUTS] = {0, 0, 0, 0};
const int ADD[NUM_INPUTS] = {0, 0, 0, 0};
const int INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};

// Delay
int LOOP_DELAY = 10;

// Analog in readings
int in_raw[NUM_INPUTS];
int in[NUM_INPUTS];
// Calculated values
int values[NUM_INPUTS];

// Notes
const int notes[NUM_INPUTS] = {0x3C, 0x3D, 0x3E, 0x3F};

const String labels[NUM_INPUTS] = {
  "BOTTOM_LEFT",
  "TOP_LEFT",
  "BOTTOM_RIGHT",
  "TOP_RIGHT"
};

// Setup routine
void setup() {
  if (DEBUG > 0) {
    Serial.begin(9600); // DEBUG
    LOOP_DELAY = 500;
  }

  int octave_step = 12;
  int octave_start = 12;
  midiSetup(octave_start, octave_step);
}

// Main loop
void loop() {
  // Read inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    in_raw[i] = analogRead(INPUTS[i]); 
    in[i] = (((in_raw[i] & REDUCE_NOISE) + ADD[i]) << ADJUST[i]); 
  }

  /*
    - We have 4 photo sensors that go like this:
    * *
    * *
  */
  
  // Play
  for (int i = 0; i < NUM_SYNTHS; i++) {
    // No input
    // Or hand moved
    // Silence
    if (in[i] == 0) {
      noteOff(i);
    }

    // Some input
    if (in[i] > 0) {
      // Play new note
      noteOn(i, notes[i], 127);
    } 
  } 
  
  // Wait
  delay(LOOP_DELAY);
}

