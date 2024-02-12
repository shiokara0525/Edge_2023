#include <Arduino.h>
#include<Adafruit_NeoPixel.h>
#include<ac.h>
#include<ball.h>
#include<line.h>
#include<motor_a.h>
#include<MA.h>
#include<timer.h>
#include<Cam.h>

BALL ball;
LINE line;
AC ac;
motor_attack MOTOR;
timer Timer;

int A = 0;
int B = 999;
const int ang_180 = 210;
const int ang_90 = 150;
const int ang_30 = 70;
const int ang_10 = 10;
const int far_th = 130;
int go_val = 220;
//======================================================きっく======================================================//
void kick();
const int C = 32;
const int K = 31;
int kick_flag = 0;
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
//======================================================ライン======================================================//
int Line_flag = 0;
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
  pinMode(LED,OUTPUT);
  pinMode(K,OUTPUT);
  pinMode(C,OUTPUT);
  digitalWrite(C,HIGH);
  digitalWrite(K,LOW);
  if(goal_color == 0){
    cam_front.color = 0;  //青が0 黄色が1
    cam_back.color = 1;  //青が0 黄色が1
  }
  else if(goal_color == 1){
    cam_front.color = 1;  //青が0 黄色が1
    cam_back.color = 0;  //青が0 黄色が1
  }
  Switch();
}

void loop() {
  ball.getBallposition();
  float AC_val = 100;
  angle go_ang(ball.ang,true);
  int max_val = go_val;
  int line_flag = line.getLINE_Vec();
  int AC_flag = 0; //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0; //0だったらキックしない 1だったらキック

  if(line_flag == 1){
    A = 20;
  }
  else{
    if(ball.ball_get == 1){
      A = 11;
    }
    else{
      A = 10;
    }
  }



  if(A == 10){  //回り込むやつ
    if(A != B){
      B = A;
    }
    int ang_180_ = ang_180;
    int ang_90_ = ang_90;
    int ang_30_ = ang_30;
    int ang_10_ = ang_10;
    if(70 < abs(ac.dir)){
      ball.ang -= ac.dir;
    }

    if(abs(ball.ang) < 10){
      go_ang = ang_10_ / 10.0 * ball.ang;
    }
    else if(abs(ball.ang) < 30){
      max_val -= 30;
      go_ang = ((ang_30_ - ang_10_) / 20.0 * (abs(ball.ang) - 10) + ang_10_);
    }
    else if(abs(ball.ang) < 90){
      go_ang = ((ang_90_ - ang_30_) / 60.0 * (abs(ball.ang) - 30) + ang_30_);
    }
    else{
      go_ang = ((ang_180_ - ang_90) / 90.0 * (abs(ball.ang) - 90) + ang_90_);
    }

    go_ang = go_ang.degree * (ball.ang < 0 ? -1 : 1);
  }


  if(A == 11){
    if(A != B){
      B = A;
      Timer.reset();
      kick_flag = 0;
    }
    AC_flag = 1;
    
    if(kick_flag == 0 && 200 < Timer.read_ms()){
      kick_ = 1;
      kick_flag = 1;
      Timer.reset();
    }
    else if(kick_flag == 1 && 400 < Timer.read_ms()){
      kick_ = 1;
      Timer.reset();
    }
    go_ang = 0;
  }

  if(A == 20){  //ラインから逃げるやつ
    angle line_ang(line.ang,true);
    if(A != B){
      B = A;
      Line_flag = line.switchLineflag(line_ang);
    }
    go_ang = line.decideGoang(line_ang,Line_flag);
  }

  if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(cam_front.ang);
  }

  if(kick_ == 1){
    kick();
  }

  MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
  // Serial.print(" | ");
  // Serial.print(go_ang.degree);
  // Serial.print(" | ");
  // ball.print();
  // Serial.print(" | ");
  // line.print();
  // Serial.print(" | ");
  // cam_front.print();
  if(toogle_f != digitalRead(toogle_P)){
    MOTOR.motor_0();
    Switch();
  }
  Serial.println();
}



void Switch(){
  digitalWrite(LED,HIGH);
  toogle_f = digitalRead(toogle_P);
  delay(100);
  while(digitalRead(toogle_P) == toogle_f);
  digitalWrite(LED,LOW);
  ac.setup_2();
  toogle_f = digitalRead(toogle_P);
  delay(100);
  while(digitalRead(toogle_P) == toogle_f);
  toogle_f = digitalRead(toogle_P);
}



void kick(){
  // esc.writeMicroseconds(1000);
  digitalWrite(C,LOW);
  delay(10);
  digitalWrite(K,HIGH);
  digitalWrite(LED,HIGH);
  delay(10);
  digitalWrite(K,LOW);
  digitalWrite(LED,LOW);
  delay(10);
  digitalWrite(C,HIGH);
  // MOTOR.Moutput(4,-200);
}



void serialEvent3(){
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
    if(reBuf[3] == 0){
      cam_back.on = 0;
    }
    else{
      if(cam_back.color == reBuf[1]){
        cam_back.on = 1;
        cam_back.Size = reBuf[3];
        cam_back.ang = -(reBuf[2] - 127);
        cam_back.senter = reBuf[4];
      }
    }
  }

  for(int i = 0; i < 6; i++){
    // Serial.print(reBuf[i]);
    // Serial.print(" ");
  }
  // Serial.println();
  // Serial.print("sawa");
}



void serialEvent4(){
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
    if(reBuf[3] == 0){
      cam_front.on = 0;
    }
    else{
      if(cam_front.color == reBuf[1]){
        cam_front.on = 1;
        cam_front.Size = reBuf[3];
        cam_front.ang = (reBuf[2]-80)*3/4;
        cam_front.senter = reBuf[4];
      }
    }
  }

  for(int i = 0; i < 6; i++){
    // Serial.print(" ");
    // Serial.print(reBuf[i]);
  }
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
  int n;
  int x,y;
  word revBuf_word[7];
  byte revBuf_byte[7];
  //受信データ数が、一定時間同じであれば、受信完了としてデータ読み出しを開始処理を開始する。
  //受信データあり ※6バイト以上になるまでまつ
  if(Serial8.available()>= 7){
    //---------------------------
    //受信データをバッファに格納
    //---------------------------
    n = 0;
    while(Serial8.available()>0 ){ //受信データがなくなるまで読み続ける
      //6バイト目まではデータを格納、それ以上は不要なデータであるため捨てる。
      if(n < 7){
        revBuf_byte[n] = Serial8.read();   //revBuf_byte[n] = Serial2.read();
      }
      else{
        Serial8.read(); //Serial2.read();  //読みだすのみで格納しない。
      }
      n++;
    }
    //---------------------------
    //データの中身を確認
    //---------------------------
    //データの先頭、終了コードあることを確認
    if((revBuf_byte[0] == 0xFF ) && ( revBuf_byte[6] == 0xAA )){
    //いったんWORD型（16bitデータ）としてから、int16_tとする。
      revBuf_word[0] = (uint16_t(revBuf_byte[1])<< 8);//上位8ビットをbyteから、Wordに型変換して格納　上位桁にするため8ビットシフト
      revBuf_word[1] = uint16_t(revBuf_byte[2]);//下位8ビットをbyteから、Wordに型変換して格納      
      x = int16_t(revBuf_word[0]|revBuf_word[1]);//上位8ビット、下位ビットを合成（ビットのORを取ることで格納する。）
      // ※int ではなく　int16_t　にすることが必要。intだけだと、32ビットのintと解釈されマイナス値がマイナスとみなされなくなる、int16_tは、16ビット指定の整数型変換
      revBuf_word[2] = (uint16_t(revBuf_byte[3])<< 8);//上位8ビットをbyteから、Wordに型変換して格納　上位桁にするため8ビットシフト
      revBuf_word[3] = uint16_t(revBuf_byte[4]);//下位8ビットをbyteから、Wordに型変換して格納      
      y = int16_t(revBuf_word[2]|revBuf_word[3]);//上位8ビット、下位ビットを合成（ビットのORを取ることで格納する。）
      // ※int ではなく　int16_t　にすることが必要。intだけだと、32ビットのintと解釈されマイナス値がマイナスとみなされなくなる、int16_tは、16ビット指定の整数型変換
      ball.get_resister(revBuf_byte[5]);

      x = ball.ball_x.demandAve(x);
      y = ball.ball_y.demandAve(y);
    }
    else{
      // printf("ERR_REV");
    }
  }
}