#include<Arduino.h>
#include<ball.h>
#include<line.h>
#include<Cam.h>

#define send_lenth 7

BALL ball;
LINE line;
Cam cam_front(4);
Cam cam_back(3);

int Mode = 0;
int Mode_B = 999;
int avaliable[4];
int M_val;

void setup(){
  ball.begin();
  line.begin();
  cam_front.begin();
  Serial.begin(9600);
  Serial7.begin(9600);
}

void loop(){
  ball.getBallposition();
  line.getLINE_Vec();
  cam_front.getCamdata();
  int Flag = 0;
  uint8_t send[send_lenth] = {38,0,0,0,0,0,37};
  int send_flag = 0;

  if(Mode == 0){
    if(Mode != Mode_B){
      Mode_B = Mode;
      Flag = 1;
    }
  }
  else if(Mode == 1){
    if(Mode != Mode_B){
      Mode_B = Mode;
      Flag = 1;
    }
    Serial.print(" | ");
    ball.print();
    Serial.print(" | ");
    // line.print();
    // Serial.print(" | ");
    // cam_front.print();
    Serial.println();
  }
  else if(Mode == 4){
    if(Mode != Mode_B){
      Mode_B = Mode;
      Flag = 1;
    }
    send[1] = 4;
    send[2] = ball.x_pos;
    send[3] = ball.y_pos;
    send[4] = ball.get_1;
    send[5] = ball.get_2;
    send_flag = 1;
  }
  else if(Mode == 5){
    if(Mode != Mode_B){
      Mode_B = Mode;
      Flag = 1;
    }
    send[1] = 5;
    send[2] = line.data_byte[0];
    send[3] = line.data_byte[1];
    send[4] = line.data_byte[2];
    send[5] = line.data_byte[3];
    send_flag = 1;
  }
  else if(Mode == 6){
    if(Mode != Mode_B){
      Mode_B = Mode;
      Flag = 1;
    }
    send[1] = 6;
    send[2] = cam_front.data_byte[0];
    send[3] = cam_front.data_byte[1];
    send[4] = cam_front.data_byte[2];
    send[5] = cam_front.data_byte[3];
    send_flag = 1;
  }

  if(send_flag == 1){
    Serial7.write(send,send_lenth);
  }
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
      if(read[2] == 0){
        Serial.print("Yellow");
      }
      else{
        Serial.print("Blue");
      }
    }
    if(read[1] == 3){
      if(read[2] == 1){
        Serial.print(" enter !!!!");
      }
      else if(read[2] == 2){
        Serial.print(" start!!!! ");
        Mode = 1;
      }
      else if(read[2] == 0){
        Serial.print(" finish!!!! ");
        Mode = 0;
      }
    }
    if(read[1] == 4){
      Serial.print(" ball!! ");
      Mode = 4;
    }
    if(read[1] == 5){
      Serial.print(" line!! ");
      Mode = 5;
    }
    if(read[1] == 6){
      Serial.print(" cam!! ");
      Mode = 6;
    }
  }
  // for(int i = 0; i < 7; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  Serial.println();
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
}



void serialEvent6(){
  // Serial.println(" sawa3 ");
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