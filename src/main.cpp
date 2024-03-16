#include <Arduino.h>
#include<Adafruit_NeoPixel.h>
#include<ac.h>
#include<ball.h>
#include<line.h>
#include<motor_a.h>
#include<MA.h>
#include<timer.h>
#include<Cam.h>
#include<BLDC.h>

BALL ball;
LINE line;
AC ac;
motor_attack MOTOR;
BLDC dribbler;

int print_flag = 1;
int macau_flag = 0;
timer macau_t;

//======================================================neopiku======================================================//
#define DELAYVAL 500
#define PIN        30 
#define NUMPIXELS 16

int Neo[16] = {12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13};
int Neo_p = 999;

Adafruit_NeoPixel pixels(DELAYVAL, PIN, NEO_GRB + NEO_KHZ800);
//======================================================カメラ======================================================//
int goal_color = 1;  //青が0 黄色が1
Cam cam_front(4);
Cam cam_back(3);
//======================================================スタートスイッチ======================================================//
int LED = 13;
int toogle_f;
int toogle_P = 27;
void Switch();
//======================================================関数たち======================================================/

void setup() {
  Serial.begin(9600);
  ball.begin();
  line.begin();
  ac.setup();
  cam_front.begin();
  cam_back.begin();
  pixels.begin();
  pixels.clear();
  dribbler.setup();
  dribbler.run();
  delay(200);
  pinMode(LED,OUTPUT);
  if(goal_color == 0){
    cam_front.color = 0;  //青が0 黄色が1
    cam_back.color = 1;  //青が0 黄色が1
  }
  else if(goal_color == 1){
    cam_front.color = 1;  //青が0 黄色が1
    cam_back.color = 0;  //青が0 黄色が1
  }
  dribbler.stop();
  Switch();
}

void loop() {
  line.getLINE_Vec();
  ball.getBallposition();
  ac.getAC_val();
  cam_front.getCamdata();
  cam_back.getCamdata();
  angle go_ang(0,true);
  float AC_val = ac.getAC_val();
  // int Target_dir = ac.dir_target;


  // MOTOR.moveMotor_0(go_ang,120,0,0);
  dribbler.run();
  // if(macau_flag == 0){
  //   MOTOR.motor_ac(AC_val);
  //   if(1000 < macau_t.read_ms()){
  //     macau_flag = 1;
  //   }
  // }
  // if(macau_flag == 1){
  //   MOTOR.motor_ac(100);
  //   if(170 < abs(ac.dir)){
  //     macau_flag = 2;
  //   }
  // }
  // else if(macau_flag == 2){
  //   MOTOR.motor_ac(150);
  //   if(40 < abs(ac.dir) && abs(ac.dir) < 50){
  //     macau_flag = 3;
  //   }
  // }
  // else if(macau_flag == 3){
  //   if(macau_t.read_ms() < 300){
  //     MOTOR.motor_0();
  //   }
  //   else{
  //     macau_flag = 0;
  //     macau_t.reset();
  //   }
  // }


  if(print_flag == 1){
    Serial.print(" | ");
    ball.print();
    // Serial.print(" | ");
    // line.print();
    // Serial.print(" | ");
    // line.print_2();
    // Serial.print(" | ");
    // ac.print();
    // Serial.print(" | ");
    // cam_front.print();
    // Serial.print(" | AC_val : ");
    // Serial.print(AC_val);
    // cam_back.print();
    // Serial.print(" | ");
    Serial.println();
  }

  if(toogle_f != digitalRead(toogle_P)){
    pixels.clear();
    dribbler.stop();
    MOTOR.motor_0();
    Switch();
  }
}



void Switch(){
  digitalWrite(LED,HIGH);
  toogle_f = digitalRead(toogle_P);
  delay(100);
  while(digitalRead(toogle_P) == toogle_f);
  digitalWrite(LED,LOW);
  ac.setup_2();
  // Target_dir = ac.dir_n;
  toogle_f = digitalRead(toogle_P);
  delay(100);
  while(digitalRead(toogle_P) == toogle_f);
  toogle_f = digitalRead(toogle_P);  //トグルがもげちゃったからいったんLチカでスタート
}



void serialEvent3(){
  // Serial.print("sawa1");
  uint8_t reBuf[6];
  if(Serial3.available() < 6){
    return;
  }

  for(int i = 0; i < 6; i++){
    reBuf[i] = Serial3.read();
  }
  while(Serial3.available()){
    Serial3.read();
  }

  if(reBuf[0] == 38 && reBuf[5] == 37){
    for(int i = 0; i < 4; i++){
      cam_back.data_byte[i] = reBuf[i+1];
    }
  }

  // for(int i = 0; i < 6; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println(" sawa1 ");
}



void serialEvent4(){
  // Serial.print("sawa2");
  uint8_t reBuf[6];
  if(Serial4.available() < 6){
    return;
  }

  for(int i = 0; i < 6; i++){
    reBuf[i] = Serial4.read();
  }
  while(Serial4.available()){
    Serial4.read();
  }

  if(reBuf[0] == 38 && reBuf[5] == 37){
    for(int i = 0; i < 4; i++){
      cam_front.data_byte[i] = reBuf[i+1];
    }
  }

  // for(int i = 0; i < 6; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println(" sawa2 ");
}


void serialEvent6(){
  // Serial.print(" sawa ");
  uint8_t read[6];
  int n = 1;
  if(Serial6.available() < 6){
    return;
  }
  read[0] = Serial6.read();
  if(read[0] != 38){
    return;
  } 
  while(0 < Serial6.available()){
    if(n < 6){
      read[n] = Serial6.read();
    }
    else{
      Serial6.read();
    }
    n++;
  }

  if(read[0] == 38 && read[5] == 37){
    line.data_byte[0] = read[1];
    line.data_byte[1] = read[2];
    line.data_byte[2] = read[3];
    line.data_byte[3] = read[4];
  }
  else{
    // Serial.print(" Error!! ");
  }
  // for(int i = 0; i < 6; i++){
  //   Serial.print(read[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}



void serialEvent8(){
  uint8_t read[8];
  unsigned int contain[4];
  int x,y;
  if(Serial8.available() < 8){
    return;
  }
  read[0] = Serial8.read();
  if(read[0] != 0xFF){
    return;
  }
  for(int i = 1; i < 8; i++){
    read[i] = Serial8.read();
  }
  while(Serial8.available()){
    Serial8.read();
  }

  if(read[0] == 0xFF && read[7] == 0xAA){
    contain[0] = (uint16_t(read[1]) << 8);
    contain[1] = (uint16_t(read[2]));
    x = int16_t(contain[0] | contain[1]);
    contain[2] = (uint16_t(read[3]) << 8);
    contain[3] = (uint16_t(read[4]));
    y = int16_t(contain[2] | contain[3]);
    ball.ball_x.demandAve(x);
    ball.ball_y.demandAve(y);
    ball.get_resister_1(read[5]);
    ball.get_resister_2(read[6]);
  }

  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  // Serial.println();
}