// Analog in numbers
#define IN_BOTTOM_LEFT 0
#define IN_TOP_LEFT 1
#define IN_BOTTOM_RIGHT 2
#define IN_TOP_RIGHT 3

// Number of inputs
#define NUM_INPUTS 4

// Delay
#define LOOP_DELAY 100

// Inputs
const int INPUTS[NUM_INPUTS] = {A0, A1, A2, A3};

// Analog in readings
int in[NUM_INPUTS];


const String labels[NUM_INPUTS] = {
  "BOTTOM_LEFT",
  "TOP_LEFT",
  "BOTTOM_RIGHT",
  "TOP_RIGHT"
};

// Setup routine
void setup() {
  Serial.begin(9600);
}

// Main loop
void loop() {
  // Read inputs
  for (int i = 0; i < NUM_INPUTS; i++) {
    in[i] = analogRead(INPUTS[i]); 
    // Print
    Serial.print(in[i]);
    Serial.print(", ");
  }
  Serial.println("\n");

  /*
    - We have 4 photo sensors that go like this:
    * *
    * *
  */
  
  // Wait
  delay(LOOP_DELAY);
}

