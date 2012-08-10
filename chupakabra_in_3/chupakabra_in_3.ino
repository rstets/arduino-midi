// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE B11111100

// Analog in labels
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Value labels
#define VALUE_S_TOP 0
#define VALUE_S_RIGHT 1
#define VALUE_S_BOTTOM 2
#define VALUE_S_LEFT 3

#define VALUE_O_ALL 0

// Number of inputs
#define NUM_INPUTS 4
// Number of values
#define NUM_S_VALUES 4

// Runtime constants
const int ADJUST[NUM_INPUTS] = {0, 0, 0, 0};
const int ADD[NUM_INPUTS] = {0, 0, 0, 0};
const int INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};

// Delay
int LOOP_DELAY = 250; // Max: 2^8

// Analog in readings
int in[NUM_INPUTS];
// Calculated values
int values[NUM_S_VALUES];
// Notes
const int notes_s[NUM_S_VALUES] = {12,15,18,21};
const int notes_d[NUM_INPUTS] = {24,27,30,32};

const String value_s_labels[NUM_S_VALUES] = {
  "TOP",
  "RIGHT",
  "BOTTOM",
  "LEFT"
};

const String input_labels[NUM_INPUTS] = {
  "BOTTOM_LEFT",
  "TOP_LEFT",
  "BOTTOM_RIGHT",
  "TOP_RIGHT",
};

// Setup routine
void setup() {
  if (DEBUG > 0) {
    Serial.begin(9600); // DEBUG
    LOOP_DELAY = 500;
  } else {
    Serial.begin(MIDI_BAUD_RATE);
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

  values[VALUE_S_TOP] = (in[IN_TOP_LEFT] + in[IN_TOP_RIGHT]);
  values[VALUE_S_RIGHT] = (in[IN_TOP_RIGHT] + in[IN_BOTTOM_RIGHT]);
  values[VALUE_S_BOTTOM] = (in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT]);
  values[VALUE_S_LEFT] = (in[IN_BOTTOM_LEFT] + in[IN_TOP_LEFT]);

  int value_o = (in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT] + in[IN_TOP_LEFT] + in[IN_TOP_RIGHT]);
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
  int v = 0;
  
  int hand_position_s = 0;
  int max_value_s = 0;
  for (v = 0; v < NUM_S_VALUES; v++) {
    int value = values[v];
    if (value > max_value_s) {
      max_value_s = value;
      hand_position_s = v;
    }
  }

  int hand_position_d = 0;
  int max_value_d = 0;
  for (v = 0; v < NUM_INPUTS; v++) {
    int value = in[v];
    if (value > max_value_d) {
      max_value_d = value;
      hand_position_d = v;
    }
  }

  if (DEBUG && max_value_s > 0 && max_value_d > 0) {
    Serial.println("------");
    Serial.print(value_s_labels[hand_position_s]);
    Serial.print(":");
    Serial.print(max_value_s);
    Serial.print(" ");
    Serial.print(input_labels[hand_position_d]);
    Serial.print(":");
    Serial.print(max_value_d);
    Serial.print(" ");
    Serial.print("O:");
    Serial.print(value_o);
    Serial.println("\n");
  }


  // Play synths
//  for (int i = 0; i < NUM_SYNTHS; i++) {
    //playSynth(i, pitch[i], velocity[i]);
//  }
  
  // Wait
  delay(LOOP_DELAY);
//  delayMicroseconds(25);
}

