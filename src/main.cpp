#include<Arduino.h>

int count = 0;
int avaliable[4] = {0,1,2,3};
int M_val = 0;
int goal_color = 0;
char *goal[2] = {"Yellow","Blue"};

void setup(){
  Serial.begin(9600);
  Serial7.begin(9600);
  Serial7.write(38);
  Serial7.write(0);
  for(int i = 0; i < 4; i++){
    Serial7.write(avaliable[i]);
  }
  Serial7.write(37);
  delay(100);
  Serial7.write(38);
  Serial7.write(1);
  Serial7.write(M_val);
  Serial7.write(0);
  Serial7.write(0);
  Serial7.write(0);
  Serial7.write(37);
  delay(100);
  Serial7.write(38);
  Serial7.write(2);
  Serial7.write(goal_color);
  Serial7.write(0);
  Serial7.write(0);
  Serial7.write(0);
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
    if(read[1] == 0){
      for(int i = 0; i < 4; i++){
        avaliable[i] = read[i+2];
      }
      for(int i = 0; i < 4; i++){
        Serial.print(" ");
        Serial.print(avaliable[i]);
      }
    }
    if(read[1] == 1){
      M_val = read[2];
      Serial.print(" ");
      Serial.print(M_val);
    }
    if(read[1] == 2){
      Serial.print(" color : ");
      Serial.print(goal[read[2]]);
    }
    if(read[1] == 3){
      Serial.print(" enter !!!!");
    }
  }
  // for(int i = 0; i < 7; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  Serial.println();
}