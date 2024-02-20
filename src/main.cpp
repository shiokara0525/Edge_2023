#include<Arduino.h>
#include <Servo.h>
#include<BLDC.h>

BLDC dribbler;

void setup() {
  Serial.begin(9600);
  dribbler.setup();
}

void loop() {
  dribbler.run();
}