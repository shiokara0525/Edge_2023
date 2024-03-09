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

int Mval = 0;
int avaliable[4] = {0,0,0,0};
int Core2_send = 0;
//======================================================neopiku======================================================//
#define DELAYVAL 500
#define PIN        30 
#define NUMPIXELS 16

int Neo[16] = {12,11,10,9,8,7,6,5,4,3,2,1,0,15,14,13};
int Neo_p = 999;

Adafruit_NeoPixel pixels(DELAYVAL, PIN, NEO_GRB + NEO_KHZ800);
//======================================================カメラ======================================================//
int goal_color = 0;  //青が0 黄色が1
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
  Serial1.begin(115200);

  Serial1.write(38);
  Serial1.write(Mval);
  for(int i = 0; i < 4; i++){
    Serial1.write(avaliable[i]);
  }
  Serial1.write(37);

  ball.begin();
  line.begin();
  ac.setup();
  cam_front.begin();
  cam_back.begin();
  pixels.begin();
  pixels.clear();
  pinMode(LED,OUTPUT);
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
  line.getLINE_Vec();
  ball.getBallposition();
  ac.getAC_val();
  // ball.print();
  // Serial.print(" | ");
  // line.print();
  // Serial.print(" | ");
  // line.print_2();
  // Serial.print(" | ");
  // ac.print();
  // Serial.print(" | ");
  // cam_front.print();
  // Serial.print(" | ");
  // cam_back.print();
  // Serial.print(" | ");
  Serial.println();

  if(Core2_send == 1){
    Serial1.write(38);
    Serial1.write(uint8_t(ball.x_pos));
    Serial1.write(uint8_t(ball.y_pos));
    Serial1.write(ball.get_val);
    Serial1.write(ball.ball_get);
    Serial1.write(37);
  }
  else if(Core2_send == 2){
    Serial1.write(38);
    Serial1.write(line.data_byte[0]);
    Serial1.write(line.data_byte[1]);
    Serial1.write(line.data_byte[2]);
    Serial1.write(line.data_byte[3]);
    Serial1.write(37);
  }
  else if(Core2_send == 3){
    Serial1.write(38);
    Serial1.write(cam_front.data_byte[0]);
    Serial1.write(cam_front.data_byte[1]);
    Serial1.write(cam_front.data_byte[2]);
    Serial1.write(cam_front.data_byte[3]);
    Serial1.write(37);
  }
  else if(Core2_send == 4){
    Serial1.write(38);
    Serial1.write(cam_back.data_byte[0]);
    Serial1.write(cam_back.data_byte[1]);
    Serial1.write(cam_back.data_byte[2]);
    Serial1.write(cam_back.data_byte[3]);
    Serial1.write(37);
  }

  if(toogle_f != digitalRead(toogle_P)){
    pixels.clear();
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



void serialEvent1(){
  uint8_t reBuf[4];
  if(Serial1.available() < 4){
    return;
  }
  for(int i = 0; i < 4; i++){
    reBuf[i] = Serial1.read();
  }
  while(Serial1.available()){
    Serial1.read();
  }


  if(reBuf[0] == 38 && reBuf[3] == 37){
    if(reBuf[1] == 0){
      Mval = reBuf[2];
    }
    for(int i = 1; i < 5; i++){
      if(reBuf[1] == i){
        avaliable[i - 1] = reBuf[2];
      }
    }
    if(reBuf[1] == 5){
      Core2_send = reBuf[2];
    }
  }
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

  for(int i = 0; i < 6; i++){
    // Serial.print(" ");
    // Serial.print(reBuf[i]);
  }
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

  for(int i = 0; i < 6; i++){
    // Serial.print(" ");
    // Serial.print(reBuf[i]);
  }
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
  // Serial.println("sawa4");
  int n;
  int x,y;
  word revBuf_word[7];
  byte revBuf_byte[7];
  //受信データ数が、一定時間同じであれば、受信完了としてデータ読み出しを開始処理を開始する。
  //受信データあり ※6バイト以上になるまでまつ
  if(Serial8.available() < 7){
    // Serial.print(Serial8.available());
    return;
  }

  revBuf_byte[0] = Serial8.read();
  if(revBuf_byte[0] != 0xFF){
    // Serial.print("!!!!!");
    return;
  }
    //---------------------------
    //受信データをバッファに格納
    //---------------------------
    n = 1;
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
  // for(int i = 0; i < 7; i++){
  //   Serial.print(revBuf_byte[i]);
  //   Serial.print(" ");
  // }
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
    // Serial.print("!!!!!!!!!!!!!");
  }
  else{
    // printf("ERR_REV");
  }
  
}