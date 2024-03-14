#include<Arduino.h>

int count = 0;
int avaliable[4] = {0,1,2,3};

void setup(){
  Serial.begin(9600);
  Serial7.begin(9600);
  Serial7.write(38);
  Serial7.write(0);
  for(int i = 0; i < 4; i++){
    Serial7.write(avaliable[i]);
  }
  Serial7.write(37);
}

void loop(){
  delay(10);
}

void serialEvent7(){
  uint8_t read[7];
  if(Serial7.available() < 7){
    return;
  }
  read[0] = Serial7.read();
  if(read[0] != 38){
    return;
  }
  for(int i = 1; i < 7; i++){
    read[i] = Serial7.read();
  }

  if(read[0] == 38 && read[6] == 37){
    if(read[1] == 1){
      for(int i = 0; i < 4; i++){
        avaliable[i] = read[i+2];
      }
      for(int i = 0; i < 4; i++){
        Serial.print(" ");
        Serial.print(avaliable[i]);
      }
    }
    if(read[1] == 1){
    }
  }
  // for(int i = 0; i < 7; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  Serial.println();
}