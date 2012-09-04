// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

// TODO: Move to a library or header
void init();
void stop();
void move(int dir);

#include <AFMotor.h>
//#include <Ultrasonic.h>

#define DEBUG 1

#define NUM_MOTORS 2
#define M_STEER_PORT 1
#define M_DRIVE_PORT 2

#define SPEEDUP_STEP_DELAY 10

AF_DCMotor steer_motor = AF_DCMotor(M_STEER_PORT);
AF_DCMotor drive_motor = AF_DCMotor(M_DRIVE_PORT);

int currentSpeed = 0;

//#define TRIGGER_PIN 44
//#define ECHO_PIN 46

//Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup() {
  if (DEBUG == 1) {
    Serial.begin(9600);
  }
  // Init
  init();
}


float distance = 0.0;

void loop() {
//  measure();
  move(FORWARD);
  stop();
  move(BACKWARD);
  stop();
  init();
  delay(500);
}


//void measure() {
//  distance = ultrasonic.Ranging(CM);
//  if (DEBUG == 1) {
//    Serial.print("distance: ");
//    Serial.print(distance);
//    Serial.print("cm");
//    Serial.println("");
//  }
//}


void initBoth() {
  currentSpeed = 0;
  drive_motor.setSpeed(0);
  drive_motor.run(RELEASE);
  steer_motor.setSpeed(0);
  steer_motor.run(RELEASE);
}

void stop() {
  for (int i = 255; i > 0; i--) {
    currentSpeed = i;
    drive_motor.setSpeed(i);
    delay(SPEEDUP_STEP_DELAY);
  }
}

void move(int dir) {
  for (int i = 0; i < 255; i++) {
    currentSpeed = i;
    drive_motor.run(dir);
    drive_motor.setSpeed(i);
    delay(SPEEDUP_STEP_DELAY);
  }
}

