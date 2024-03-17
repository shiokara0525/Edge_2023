#include<Arduino.h>
#include<OLED_a.h>
#include<line.h>
#include<ball.h>
#include<ac.h>
#include<Cam.h>
#include<motor_a.h>


oled_attack OLED;
LINE line;
BALL ball;
AC ac;
Cam cam(4);
motor_attack MOTOR;

void setup(){
  OLED.OLED();
}

void loop(){
  delay(100);
  Serial.println("OK!!!"); 
}