// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE 0xFFFE // 0xFFF8 // remove least 3 bits (2^3 = 8)

// Analog in labels
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Value labels
#define VALUE_BOTTOM 0
#define VALUE_TOP 1
#define VALUE_LEFT 2
#define VALUE_RIGHT 3
#define VALUE_ALL 4

// Number of inputs
#define NUM_INPUTS 4
// Number of values
#define NUM_VALUES 5

// Runtime constants
const unsigned int ADJUST[NUM_INPUTS] = {1, 1, 1, 1};
const unsigned int ADD[NUM_INPUTS] = {0, 0, 0, 0};
const unsigned int INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};
// Delay
unsigned int LOOP_DELAY = 10;

// Analog in readings
unsigned int in[NUM_INPUTS];
// Calculated values
unsigned int values[NUM_VALUES];

// Setup routine
void setup() {
  if (DEBUG > 0) {
    Serial.begin(9600); // DEBUG
    LOOP_DELAY = 500;
  } else {
    Serial.begin(MIDI_BAUD_RATE);
  }
  
  unsigned int octave_step = 7;
  midiSetup(octave_step);
}

// Main loop
void loop() {
  // Read inputs
  for (unsigned int i = 0; i < NUM_INPUTS; i++) {
    in[i] = ((analogRead(INPUTS[i]) & REDUCE_NOISE + ADD[i]) << ADJUST[i]); 
  }

  values[VALUE_BOTTOM] = in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT];
  values[VALUE_TOP] = in[IN_TOP_LEFT] + in[IN_TOP_RIGHT];
  values[VALUE_LEFT] = in[IN_BOTTOM_LEFT] + in[IN_TOP_LEFT];
  values[VALUE_RIGHT] = in[IN_TOP_RIGHT] + in[IN_BOTTOM_RIGHT];
  values[VALUE_ALL] = in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT] + in[IN_TOP_LEFT] + in[IN_TOP_RIGHT];
  
  unsigned int pitch[NUM_SYNTHS] = {values[VALUE_LEFT], values[VALUE_RIGHT]};
  unsigned int velocity[NUM_SYNTHS] = {values[VALUE_TOP], values[VALUE_BOTTOM]};

  // Play synths
  for (unsigned int i = 0; i < NUM_SYNTHS; i++) {
    playSynth(i, pitch[i], velocity[i]);
  }
  
  // Wait
  delay(LOOP_DELAY);
}

