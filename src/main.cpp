#include<Arduino.h>
#include<OLED_a.h>
#include<line.h>
#include<ball.h>
#include<ac.h>
#include<Cam.h>
#include<motor_a.h>


BALL ball;  //ボールのオブジクトだよ(基本的にボールの位置取得は全部ここ)
AC ac;      //姿勢制御のオブジェクトだよ(基本的に姿勢制御は全部ここ)
LINE line;  //ラインのオブジェクトだよ(基本的にラインの判定は全部ここ)
motor_attack MOTOR;
Cam cam(4);
oled_attack OLED;

void setup(){
  OLED.setup();
  OLED.OLED();
}

void loop(){
  delay(100);
  Serial.println("OK!!!"); 
}