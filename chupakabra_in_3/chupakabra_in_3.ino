// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE B11111100

// Analog in numbers
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Diagonal values (equivalent to input numbers)
#define VALUE_BOTTOM_LEFT 0
#define VALUE_TOP_LEFT 1
#define VALUE_BOTTOM_RIGHT 2
#define VALUE_TOP_RIGHT 3
// Horizontal-vertical values
#define VALUE_TOP 4
#define VALUE_RIGHT 5
#define VALUE_BOTTOM 6
#define VALUE_LEFT 7

// Number of inputs
#define NUM_INPUTS 4
// Number of values
#define NUM_VALUES 8

// Runtime constants
const int ADJUST[NUM_INPUTS] = {0, 0, 0, 0};
const int ADD[NUM_INPUTS] = {0, 0, 0, 0};
const int INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};

// Delay
int LOOP_DELAY = 250;

// Analog in readings
int in[NUM_INPUTS];
// Calculated values
int values[NUM_VALUES];
// Previous hand position
int old_hand_position = 0;

// Notes
const int notes[NUM_VALUES] = {0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43};

const String value_labels[NUM_VALUES] = {
  "BOTTOM_LEFT",
  "TOP_LEFT",
  "BOTTOM_RIGHT",
  "TOP_RIGHT",
  "TOP",
  "RIGHT",
  "BOTTOM",
  "LEFT"
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
    in[i] = ((analogRead(INPUTS[i]) & REDUCE_NOISE + ADD[i]) << ADJUST[i]); 
  }

  values[VALUE_BOTTOM_LEFT] = in[IN_BOTTOM_LEFT];
  values[VALUE_BOTTOM_RIGHT] = in[IN_BOTTOM_RIGHT];
  values[VALUE_TOP_LEFT] = in[IN_TOP_LEFT];
  values[VALUE_TOP_RIGHT] = in[IN_TOP_RIGHT];

  values[VALUE_TOP] = (in[IN_TOP_LEFT] + in[IN_TOP_RIGHT]);
  values[VALUE_RIGHT] = (in[IN_TOP_RIGHT] + in[IN_BOTTOM_RIGHT]);
  values[VALUE_BOTTOM] = (in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT]);
  values[VALUE_LEFT] = (in[IN_BOTTOM_LEFT] + in[IN_TOP_LEFT]);

  /*
  OK. A bit of analysis now.
  - We have 4 photo sensors that go like this:
  * *
  * *

  - Let's say we have the greatest value at top-left input.
  - And 0 at bottom-left
  - The left value will be equal to the sum of those two
  - Thus equal to top-left value
  - In that case we are not sure which one is really active
  - I suggest we have to prefer "top-left" before "left"
  - Because if top-left and bottom-left are not zero the sum will be bigger than any of two
  */
  
  // Detect maximum value and position
  int hand_position = 0;
  int max_value = 0;
  for (int v = 0; v < NUM_VALUES; v++) {
    int value = values[v];
    if (value > max_value) {
      max_value = value;
      hand_position = v;
    }
  }

  // Detect maximum input and position
  int input_position = 0;
  int max_input = 0;

  for (int i = 0; i < NUM_INPUTS; i++) {
    int input = in[i];
    if (input > max_input) {
      max_input = input;
      input_position = i;
    }
  }

  // Value equals input means other of two inputs for value is zero.
  if (max_value == max_input) {
    max_value = max_input;
    hand_position = input_position;
  }

//  if (DEBUG > 0 && max_value > 0) {
//    Serial.println("\n");
//    Serial.print(value_labels[hand_position]);
//  }

  // Play
  for (int i = 0; i < NUM_SYNTHS; i++) {
    // No input
    // Or hand moved
    // Silence
    if (max_value == 0 || old_hand_position != hand_position) {
      noteOff(i);
    }

    // Some input
    if (max_value > 0) {
      // Play new note
      noteOn(i, notes[hand_position], max_value << 2);
      old_hand_position = hand_position;
    } 
  } 
  
  // Wait
  delay(LOOP_DELAY);
}

