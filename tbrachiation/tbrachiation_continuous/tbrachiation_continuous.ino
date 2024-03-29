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

// Motor communication
const char inp_m = 12;

// Motor control
float duty = 0;
float t;

char DIN_PIN = 11;
static bool IsFirst = false;
static bool IsFirst_2 = true;
static bool IsFirst_3 = true;
static bool IsFirst_4 = true;
static bool IsFirst_5 = true;

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
  myservo_1.attach(7,560,2480);//上
  myservo_2.attach(4,560,2480);//下  
  // SWITCH
  pinMode(DIN_PIN, INPUT_PULLUP );
}

void loop() {
  analogWrite(inp_m,160);//0//160//255
  myservo_1.write(110);//グリッパ開
  myservo_2.write(110);//グリッパ開
  if(digitalRead(DIN_PIN)==LOW){  //リミットスイッチ押したら
    IsFirst = true;
    delay(500);
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
      if(t >= 775 &&IsFirst_3 == true){
        analogWrite(inp_m,0);//0//160//255
        IsFirst_3 = false;
      }
      if(t >= 930){
        myservo_2.write(0);//グリッパ開 
        delay(200);
        myservo_1.write(120);//グリッパ開 
        delay(1000000);
      }
    } 
  }
}
