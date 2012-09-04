#include <Ultrasonic.h>

#define TRIGGER_PIN 44
#define ECHO_PIN 46

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()
{
  Serial.begin(9600);                           // start the serial port
}

void loop()
{
  float dist_cm = ultrasonic.Ranging(CM);       // get distance
  Serial.println(dist_cm);                      // print the distance
  
  delay(200);                                   // arbitary wait time.
}
