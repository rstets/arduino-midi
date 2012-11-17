// Global constants
#include "global.h"

// Include MIDI helper utility
#include "midi_helpers.h"

// Adjust analog in readings
#define REDUCE_NOISE 0xFFF0

// Analog in numbers
#define IN_TOP_RIGHT 0
#define IN_BOTTOM_LEFT 1
#define IN_TOP_LEFT 2
#define IN_BOTTOM_RIGHT 3

// Number of inputs
#define NUM_INPUTS 4

// Runtime constants
const int ADJUST[NUM_INPUTS] = {0, 0, 0, 0};
const int REDUCE[NUM_INPUTS] = {0, 0, 0, 0};
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
  "TOP_RIGHT",
  "BOTTOM_LEFT",
  "TOP_LEFT",
  "BOTTOM_RIGHT"
};

// Setup routine
void setup() {
  if (DEBUG > 0) {
    Serial.begin(9600); // DEBUG
    LOOP_DELAY = 500;
  }

  int octave_step = 0;
  int octave_start = 0;
  midiSetup(octave_start, octave_step);
}

// Main loop
void loop() {
  // Read inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    in_raw[i] = analogRead(INPUTS[i]); 
    in[i] = (((in_raw[i] & REDUCE_NOISE) + ADD[i]) << ADJUST[i] >> REDUCE[i]);
//    Serial.print(labels[i]);
//    Serial.print(": ");
//    Serial.print(in[i]);
//    Serial.print(", ");
  }
//  Serial.println("\n");

  /*
    - We have 4 photo sensors that go like this:
    * *
    * *
  */
  int top = in[IN_TOP_LEFT] + in[IN_TOP_RIGHT];
  int bottom = in[IN_BOTTOM_LEFT] + in[IN_BOTTOM_RIGHT];
  int v_min = 0;
  int v_max = top + bottom;
  int v_mid = v_max >> 1;

  int left = in[IN_BOTTOM_LEFT] + in[IN_TOP_LEFT];
  int right = in[IN_BOTTOM_RIGHT] + in[IN_TOP_RIGHT];

  int h_min = 0;
  int h_max = left + right;
  int h_mark = left;
  int h_mid = h_max >> 1;
  int h_quarter = h_mid >> 1;
  int h_diff = right - h_mid;
  int h_abs = h_diff;
  
  int h_section_1 = h_quarter;
  int h_section_2 = h_mid;
  int h_section_3 = h_mid + h_quarter;
  int h_sections[4] = {
    h_section_1,
    h_section_2,
    h_section_2,
    h_max
  };
  int h_sect;
  int h_section;
  for (h_sect = 0; h_sect < 4; h_sect++) {
    h_section = h_sections[h_sect];
    if (h_abs > h_section) break;
  }

  
    if (DEBUG > 0) {
      // Print
      Serial.print("h_abs: ");
      Serial.print(h_abs);
      Serial.print(", h_max: ");
      Serial.print(h_max);
      Serial.print(", h_diff: ");
      Serial.print(h_diff);
      Serial.print(", h_mid: ");
      Serial.print(h_mid);
      Serial.print(", left: ");
      Serial.print(left);
      Serial.print(", right: ");
      Serial.print(right);
      Serial.print(", h_sect: ");
      Serial.print(h_sect);
      Serial.print(", h_section: ");
      Serial.print(h_section);
        Serial.println("\n");
    }
  
  // Play
//  for (int i = 0; i < NUM_SYNTHS; i++) {
//    // No input
//    // Or hand moved
//    // Silence
//    if (in[i] == 0) {
//      noteOff(i);
//    }
//
//    // Some input
//    if (in[i] > 0) {
//      // Play new note
//      noteOn(i, notes[i], 127);
//    } 
//  } 
  
  // Wait
  delay(LOOP_DELAY);
}

