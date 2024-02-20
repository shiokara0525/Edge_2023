#include<Arduino.h>
#include <Servo.h>

#define MAX_SIGNAL 2000  //PWM信号における最大のパルス幅[マイクロ秒]
#define MIN_SIGNAL 1000  //PWM信号における最小のパルス幅[マイクロ秒]
#define ESC_PIN 33  //ESCへの出力ピン
#define SIGNAL 1800

Servo esc;  //Servoオブジェクトを作成する．今回はESCにPWM信号を送るので，`esc`と命名している．
void BLDC_setup();
void BLDC_run();

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");
  Serial.println("Turn on power source, then wait 2 seconds and press any key.");
  BLDC_setup();
}

void loop() {
  BLDC_run();
}



void BLDC_setup(){
  esc.attach(ESC_PIN);  //ESCへの出力ピンをアタッチします
  Serial.println("Writing maximum output.");
  esc.writeMicroseconds(MAX_SIGNAL);  //ESCへ最大のパルス幅を指示します
  Serial.println("Wait 2 seconds.");
  delay(2000);
  Serial.println("Writing minimum output");
  esc.writeMicroseconds(MIN_SIGNAL);  //ESCへ最小のパルス幅を指示します
  Serial.println("Wait 2 seconds. Then motor starts");
  delay(2000);
}


void BLDC_run(){
  esc.writeMicroseconds(SIGNAL);  // パルス幅 `volume` のPWM信号を送信する
}