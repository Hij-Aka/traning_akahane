#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>

Adafruit_BNO055 bno;
Servo myservo_1;
Servo myservo_2;


const float pi = 3.14159;

// ROT_ENCODER SETUP

//  Pins for SPI
#define CS 8
#define CNT_EN 9

//  Configuration of registers
#define CLR_MDR0 0x08
#define CLR_MDR1 0x10
#define CLR_CNTR 0x20
#define CLR_STR 0x30
#define READ_MDR0 0x48
#define READ_MDR1 0x50
#define READ_CNTR 0x60
#define READ_OTR 0x68
#define READ_STR 0x70
#define WRITE_MDR1 0x90
#define WRITE_MDR0 0x88
#define WRITE_DTR 0x98
#define LOAD_CNTR 0xE0
#define LOAD_OTR 0xE4

//  Datawidth
const byte datawidth = 4;

// Motor communication
const char inp_m = DAC1;

// Motor control
float t;

char DIN_PIN = 12;
static bool IsFirst = false;
static bool IsFirst_2 = true;
static bool IsFirst_3 = true;
static bool IsFirst_4 = true;
static bool IsFirst_5 = true;
static bool IsFirst_6 = true;
static bool IsFirst_7 = true;

uint16_t time_ini;

// main arduino functions

void setup()
{
  // Serial communication
  Serial.begin(115200);
  Serial.setTimeout(50);

  // MOTOR

  pinMode(inp_m, OUTPUT);
 
  // SERVO
  myservo_1.attach(13,560,2480);//上
  myservo_2.attach(6,560,2480);//下  
  // SWITCH
  pinMode(DIN_PIN, INPUT_PULLUP );
}

void loop() {
  analogWrite(inp_m,255);
  myservo_1.write(110);//グリッパ開
  myservo_2.write(110);//グリッパ開
  if(digitalRead(DIN_PIN)==LOW){  //リミットスイッチ押したら
    IsFirst = true;
    delay(1000);
  }
  while(IsFirst == true){
    myservo_1.write(0);//グリッパ閉
    delay(1000);
    myservo_2.write(0);//グリッパ閉
    delay(3000);
        
    time_ini = millis();//初期時刻取得
    myservo_2.write(120);//グリッパ開 

    while(1){ 
      t = millis()-  time_ini;
      if(t >= 400 &&IsFirst_2 == true){
        analogWrite(inp_m,255);//0//160//255
        IsFirst_2 = false;
      }
      if(t >= 800 &&IsFirst_3 == true){
        analogWrite(inp_m,50);//0//160//255
        IsFirst_3 = false;
      }
      if(t >= 1200 &&IsFirst_4 == true){
        analogWrite(inp_m,255);//0//160//255
        IsFirst_4 = false;
      }
      if(t >= 1600 &&IsFirst_5 == true){
        analogWrite(inp_m,50);//0//160//255
        IsFirst_5 = false;
      }
       if(t >= 2000 &&IsFirst_6 == true){
        analogWrite(inp_m,255);//0//160//255
        IsFirst_6 = false;
      }
      if(t >= 2350){
        myservo_1.write(120);//グリッパ開 
        delay(150);
        myservo_2.write(0);//グリッパ開 
        delay(1000000);
      }
    } 
  }
}
