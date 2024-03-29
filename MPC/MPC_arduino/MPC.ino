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

sensors_event_t event;
sensors_event_t angVelocityData;

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
const char inp_m = 3;

// Offset for IMU
const float theta_offset = 0.06;

// Hysteresis thresholds for signs of theta and dtheta/dt
const float theta_threshold = 0.02;
const float d_theta_threshold = 0.05;

// gamma differentiation
float last_gamma_value;
unsigned long last_gamma_time;

// theta differentiation
float last_theta_value;
unsigned long last_theta_time;

char DIN_PIN = 12;
char buff[500];
char buff_2[500];
int f_length = 50;
int time_1 = 553;//610(30)
int time_2 = 1000000;//空中相へ移る時間
int time_3 = 1000000;//次のバーを掴む時間
uint16_t delay_grip_cl = 225;
uint16_t delay_grip_op = 180;
uint16_t delay_loop = 300;
uint16_t delay_gri_angle = 180;
uint16_t delay_gri_ini = 113;
uint16_t delay_m = 39;
int time_delay = 8;
static bool IsFirst = false;
static bool IsFirst_2 = true;
String msg;
uint16_t time_ini;
uint16_t time_aerial = 0;
static int time_start;
uint16_t finish_time;
uint16_t time_threshold = 70;
uint16_t time_threshold_2 = 100;
uint16_t time_ori = 0;
int times[5000];
int time_cons;
int16_t val[5000];
int16_t val_ini;
int16_t gy[5000];
int16_t yaw[5000];
int16_t yaw_cur;
int16_t yaw_pre;
int time_2_serve[10];
int time_3_serve[10];
int count_2 = 0;
int count_3 = 0;
int time_end = 5000;
int aerial_threshold = 0;

float differentiator(const float value, float *last_value, unsigned long *last_time)
{
  unsigned long time = millis();

  float differentiated_value = (value - *last_value) / (time - *last_time) * 1e6;

  *last_value = value;
  *last_time = time;

  return differentiated_value;
}

unsigned long read_counter()
{
  unsigned long return_value = 0;
  byte bytes_to_read = datawidth;
  byte val;

  digitalWrite(CS, LOW);
  SPI.transfer(READ_CNTR);
  while (bytes_to_read > 0)
  {
    val = SPI.transfer(0x00);
    bytes_to_read--;
    return_value = (return_value << 8) | val;
  }
  digitalWrite(CS, HIGH);
  return return_value;
}

// main arduino functions

void setup()
{
  //ls7366r
  pinMode(CS, OUTPUT);
  pinMode(CNT_EN, OUTPUT);
  digitalWrite(CNT_EN, HIGH);
  SPI.begin(CS);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(CS, LOW);
  SPI.transfer(WRITE_MDR0);
  SPI.transfer(0x01);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(WRITE_MDR1);
  SPI.transfer(0x00); //byte
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(CLR_CNTR);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(CLR_STR);
  digitalWrite(CS, HIGH);

  // Serial communication
  Serial.begin(115200);
  Serial.setTimeout(50);

  // IMU
  // Serial.println("Sensor start");
  if (!bno.begin()) {
    // Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);

  // MOTOR
  pinMode(inp_m, OUTPUT);
  digitalWrite(inp_m,LOW);  
 
  // SERVO
  myservo_1.attach(7,900,2100);//brown　Arduino側
  myservo_2.attach(4,900,2100);//red IMU側
  
  // SWITCH
  pinMode(DIN_PIN, INPUT_PULLUP );

  times[0] = 0;
  gy[0] = 0;
}

void loop() {
  myservo_1.write(60);//グリッパ開
  myservo_2.write(80);//グリッパ開
  if(digitalRead(DIN_PIN)==LOW){  //リミットスイッチ押したら
    IsFirst = true;
    delay(500);
  }
  while(IsFirst == true){
    myservo_1.write(160);//グリッパ閉
    delay(1000);
    myservo_2.write(180);//グリッパ閉
    delay(3000);
        
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw[0] = - euler.z();
    yaw_pre = yaw[0];
    time_ini = millis();//初期時刻取得
    myservo_2.write(60);//グリッパ開 
    while((millis() - time_ini) < 150){
    }
    digitalWrite(inp_m, HIGH);//モータ始動
    
    time_start = time_ini + delay_grip_op;//ブラキエーション開始時刻計算
    
    while(yaw_cur == yaw_pre){
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      yaw_cur = - euler.z();
    }

    times[1] = millis() - time_start + time_delay; //スタートからの時間を取得
    long count = read_counter();
    count = (count + 12288) % 16384;
    val[1] = abs (360 * count / 16384);// 左回転で正
    yaw[1] = yaw_cur;
    yaw_pre = yaw_cur;
    gy[1] = 1000. * (yaw[1] - yaw[0])/(times[1]+time_delay);
    String string_times =  String(times[1]);
    String string_val =  String(val[1]);
    String string_yaw =  String(yaw[1]);
    String string_gyro =  String(gy[1]);
    msg = string_times + '!' + string_val + '"' + string_yaw + '#'+ string_gyro + '$';//区切り文字をつけてまとめる
    Serial.print(msg);

    int i = 2;//配列番号初期化

    while(IsFirst_2 == true){ 
      //空中相
      int t_check  = millis() - time_start + time_delay;
      if((time_2 - delay_grip_op + time_threshold) <= t_check){
        digitalWrite(inp_m,LOW);
        myservo_1.write(60);
        // Serial.println(t_test);
        while((time_3 - delay_grip_cl + time_threshold + aerial_threshold) >= time_aerial){
          time_aerial = millis() - time_start + time_delay;
        }
        myservo_2.write(180);
        IsFirst_2 = false;//flag off
      }

      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      yaw_cur = - euler.z();

      if(yaw_cur != yaw_pre){
        times[i] = millis() - time_start + time_delay; //スタートからの時間を取得
        long count = read_counter();
        count = (count + 12288) % 16384;
        val[i] = abs (360 * count / 16384);// 左回転で正
        yaw[i] = yaw_cur;
        yaw_pre = yaw_cur;
        gy[i] = 1000. * (yaw[i] - yaw[i-1])/(times[i]-times[i-1]);    
        String string_times =  String(times[i]);
        String string_val =  String(val[i]);
        String string_yaw =  String(yaw[i]);
        String string_gyro =  String(gy[i]);
        msg = string_times + '!' + string_val + '"' + string_yaw + '#'+ string_gyro + '$';//区切り文字をつけてまとめる
        // Serial.print(msg);
        i++;
      }

      // send data to PC
      Serial.print(msg);

      //receive data from PC
      if(Serial.available() > 0){//もしPCから値が送られてきたら
        String ct_1;
        int lg = Serial.readBytesUntil('\t', buff, f_length);
        if(lg == 4){
          for(int i = 0;i < lg;i++){
          ct_1 = ct_1 + buff[i];
          }
          time_2 = ct_1.toInt();
          time_2_serve[count_2] = time_2;
          count_2 ++;
        }
        String ct_2;
        lg = Serial.readBytesUntil('\n', buff_2, f_length);
        if(lg == 4){
          for(int i = 0;i < lg;i++){
          ct_2 = ct_2 + buff_2[i];
          }
          time_3 = ct_2.toInt();
          time_3_serve[count_3] = time_3;
          count_3 ++;
        }
      }

      //終了
      if(times[i] > time_end){
        IsFirst_2 = false;//flag off
      }
      
    }

    delay(500);
    Serial.println();
    for(int j=0;j<10;j++){
      Serial.print("time_2: ");
      Serial.print(time_2_serve[j]);
      Serial.print('\t');
      Serial.print("time_3: ");
      Serial.println(time_3_serve[j]);
    }
    for(int j=1;j<1000;j++){
      Serial.print(times[j]);
      Serial.print('\t');
      Serial.print(val[j]);
      Serial.print('\t');
      Serial.print(yaw[j]);
      Serial.print('\t');
      Serial.println(gy[j]);
    }
  }
}
