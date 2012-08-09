// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE B11111111 // 0xFF

// Analog in labels
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Value labels
#define VALUE_TOP 0
#define VALUE_TOP_RIGHT 1
#define VALUE_RIGHT 2
#define VALUE_BOTTOM_RIGHT 3
#define VALUE_BOTTOM 4
#define VALUE_BOTTOM_LEFT 5
#define VALUE_LEFT 6
#define VALUE_TOP_LEFT 7
#define VALUE_ALL 8

// Number of inputs
#define NUM_INPUTS 4
// Number of values
#define NUM_VALUES 9

// Runtime constants
const byte ADJUST[NUM_INPUTS] = {1, 1, 1, 1};
const byte ADD[NUM_INPUTS] = {0, 0, 0, 0};
const byte INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};

// Delay
byte LOOP_DELAY = 250; // Max: 2^8

// Analog in readings
byte in[NUM_INPUTS];
// Calculated values
byte values[NUM_VALUES];
// Notes
const byte notes[NUM_VALUES] = {12,15,18,21,24,27,30,32,36};

// Setup routine
void setup() {
  if (DEBUG > 0) {
    Serial.begin(9600); // DEBUG
    LOOP_DELAY = 500;
  } else {
    Serial.begin(MIDI_BAUD_RATE);
  }
  
  byte octave_step = 12;
  byte octave_start = 12;
  midiSetup(octave_start, octave_step);
}

// Main loop
void loop() {
  // Read inputs
  for (byte i = 0; i < NUM_INPUTS; i++) {
    in[i] = ((analogRead(INPUTS[i]) & REDUCE_NOISE + ADD[i]) << ADJUST[i]); 
  }

  values[VALUE_TOP] = (in[IN_TOP_LEFT] + in[IN_TOP_RIGHT]) > 1;
  values[VALUE_TOP_LEFT] = in[IN_TOP_LEFT];
  values[VALUE_LEFT] = (in[IN_BOTTOM_LEFT] + in[IN_TOP_LEFT]) > 1;
  values[VALUE_BOTTOM_LEFT] = in[IN_BOTTOM_LEFT];
  values[VALUE_BOTTOM] = (in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT]) > 1;
  values[VALUE_BOTTOM_RIGHT] = in[IN_BOTTOM_RIGHT];
  values[VALUE_RIGHT] = (in[IN_TOP_RIGHT] + in[IN_BOTTOM_RIGHT]) > 1;
  values[VALUE_TOP_RIGHT] = in[IN_TOP_RIGHT];
  values[VALUE_ALL] = (in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT] + in[IN_TOP_LEFT] + in[IN_TOP_RIGHT]) > 2;
  /*
  OK. A bit of analysis now.
  1) We have 4 photo sensors that go like this:
  * *
  * *
  2) Photo sensors are noise and shitty etc, but I'll deal with that later.
  3) The task is to get 9 values: Top, Top-right, Right, Bottom-right, so on and All. See constants above.
  4) First thing is that we could compare sensor values and depending on that detect where the hand is.
  5) That works find with angle values because they are equal to input values.
  6) Not so good for top, bottom etc, as they are the sum of two neighbouring input values. I suppose we could divide those by 2.
  7) As well as divide "All" by 4 (Bit shift right by 2. See above)
  8) One thing that bothers me is calibration... but I'll deal with that later as well.
  9) Now. The basic idea is ti produce 9 notes of the same velocity depending on where the hand is. For now.
  10) One synth. Constant velocity. Constant rate. Different notes
  */
  
  byte hand_position = 0;
  byte max_value = 0;
  for (byte v = 0; v < NUM_VALUES; v++) {
    byte value = values[v];
    if (value > max_value) {
      max_value = value;
      hand_position = v;
    }
  }
  byte pitch[NUM_SYNTHS] = {notes[hand_position]};
  byte velocity[NUM_SYNTHS] = {127};
  
  // TODO: Test it.
  if (DEBUG == 1) {
    Serial.print("Hand at: ");
    Serial.print(hand_position);
    Serial.print(", Max value: ");
    Serial.print(max_value);
    Serial.println("------");
  }

  // Play synths
  for (byte i = 0; i < NUM_SYNTHS; i++) {
    playSynth(i, pitch[i], velocity[i]);
  }
  
  // Wait
  delay(LOOP_DELAY);
}

