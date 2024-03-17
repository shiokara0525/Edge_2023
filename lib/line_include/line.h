#pragma once

#include <timer.h>
#include <Arduino.h>
#include <angle.h>
#define Long 5

class LINE{
public:
    int getLINE_Vec(); //ラインセンサのベクトル(距離,角度)を取得する関数
    int switchLineflag(angle);
    float decideGoang(angle,int);
    void print();
    void print_2();
    void begin();
    double line_switch(int,double,int); 
    LINE();

    double dis; //ラインのベクトルの長さ
    double ang; //ラインの和のベクトルの角度
    float dis_X;
    float dis_Y;
    int side_flag;  //1は90°で検知、2は-90°で検知、3は0°、4は180°
    int num;
    int flag;
    int data_on[27];
    uint8_t data_byte[4];
    double ele_X[27]; //ラインセンサのX座標
    double ele_Y[27]; //ラインセンサのY座標
    int LINE_Level = 0;
 
    int LINE_on; //ラインがロボットの下になかったら0,あったら1にする
    int line_flag = 0;

private:
    double PI = 3.1415926535897932384626; //円周率
    int A = 0;
    timer timer1; //タイマーの宣言
};