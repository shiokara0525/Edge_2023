#include<Arduino.h>

void setup(){
  Serial.begin(9600);
  Serial7.begin(9600);
}

void loop(){
  Serial.print(Serial1.available());
  delay(10);
}

void serialEvent7(){
  int a = Serial7.read();
  Serial.println(a);
}