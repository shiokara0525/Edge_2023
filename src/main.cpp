#include<Arduino.h>

int count = 0;

void setup(){
  Serial.begin(9600);
  Serial7.begin(9600);
}

void loop(){
  count++;
  delay(100);
  Serial7.write(count % 256);
}