#include<Arduino.h>
#include <Servo.h>
#include<BLDC.h>

BLDC dribbler;

void setup() {
  Serial.begin(9600);
  dribbler.setup();
}

void loop() {
  line.getLINE_Vec();
  ball.getBallposition();
  ac.getAC_val();
  // ball.print();
  // Serial.print(" | ");
  // line.print();
  // Serial.print(" | ");
  // ac.print();
  // Serial.print(" | ");
  // cam_front.print();
  // Serial.print(" | ");
  // cam_back.print();
  // Serial.print(" | ");
  Serial.println();

  if(1){
    // MOTOR.Moutput(4,200);
    delay(5000);
    kick();
  }

  if(toogle_f != digitalRead(toogle_P)){
    pixels.clear();
    MOTOR.motor_0();
    Switch();
  }
}



void Switch(){
  // digitalWrite(LED,HIGH);
  // toogle_f = digitalRead(toogle_P);
  // delay(100);
  // while(digitalRead(toogle_P) == toogle_f);
  // digitalWrite(LED,LOW);
  // ac.setup_2();
  // Target_dir = ac.dir_n;
  // toogle_f = digitalRead(toogle_P);
  // delay(100);
  // while(digitalRead(toogle_P) == toogle_f);
  // toogle_f = digitalRead(toogle_P);  //トグルがもげちゃったからいったんLチカでスタート
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  ac.setup_2();
  // Target_dir = ac.dir_n;
  delay(1000);
  digitalWrite(LED,HIGH);
}



void kick(){
  // esc.writeMicroseconds(1000);
  digitalWrite(C,LOW);
  delay(10);
  digitalWrite(K,HIGH);
  digitalWrite(LED,HIGH);
  delay(50);
  digitalWrite(K,LOW);
  digitalWrite(LED,LOW);
  delay(10);
  digitalWrite(C,HIGH);
  // MOTOR.Moutput(4,-200);
}



void serialEvent3(){
  uint8_t reBuf[5];
  if(Serial3.available() < 5){
    return;
  }

  for(int i = 0; i < 5; i++){
    reBuf[i] = Serial3.read();
  }
  while(Serial3.available()){
    Serial3.read();
  }

  if(reBuf[0] == 38 && reBuf[4] == 37){
    if(reBuf[3] == 0){
      cam_back.on = 0;
    }
    else{
      if(cam_back.color == reBuf[1]){
        cam_back.on = 1;
        cam_back.Size = reBuf[3];
        cam_back.ang = -(reBuf[2] - 127);
      }
      else{
        cam_back.on = 0;
      }
    }
  }

  for(int i = 0; i < 5; i++){
    // Serial.print(reBuf[i]);
    // Serial.print(" ");
  }
  // Serial.println();
  // Serial.print("sawa");
}



void serialEvent4(){
  uint8_t reBuf[5];
  if(Serial4.available() < 5){
    return;
  }

  for(int i = 0; i < 5; i++){
    reBuf[i] = Serial4.read();
  }
  while(Serial4.available()){
    Serial4.read();
  }

  if(reBuf[0] == 38 && reBuf[4] == 37){
    if(reBuf[3] == 0){
      cam_front.on = 0;
    }
    else{
      if(cam_front.color == reBuf[1]){
        cam_front.on = 1;
        cam_front.Size = reBuf[3];
        cam_front.ang = -(reBuf[2] - 127);
      }
      else{
        cam_front.on = 0;
      }
    }
  }
  for(int i = 0; i < 5; i++){
    Serial.print(reBuf[i]);
    Serial.print(" ");
  }
  // Serial.println();
  // Serial.print("sawa");
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