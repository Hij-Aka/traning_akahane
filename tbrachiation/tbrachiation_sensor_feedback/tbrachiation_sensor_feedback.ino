#define Kp      40
#define Ki      0
#define Kd      3

// Offsets for IMU and encoder data
#define thm_delta  90
#define offset_th  0

// Hysteresis thresholds for signs of theta and dtheta/dt
#define th_threshold 5
#define d_th_threshold 0.5

// IMU SETUP
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno;
sensors_event_t event;
sensors_event_t angVelocityData;

// ROT_ENCODER SETUP
#include <SPI.h>
// Pins for SPI
#define CS 11
#define CNT_EN 13
// Configuration of registers
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

// DA_CONVERTER SETUP
#define PCF8591 (0x90 >> 1)      // Device address = 0       
#define PCF8591_DAC_ENABLE 0x48
#define PCF8591_ADC_CH0 0x40
#define PCF8591_ADC_CH1 0x41
#define PCF8591_ADC_CH2 0x42
#define PCF8591_ADC_CH3 0x43

// Motor control
float duty = 0;
float t, dt, t_pid;
float t_last, t_pid_last, P_last, thm_last;
float thm;
int thm_target;
float P, I, D, U;
float th, d_th, th_last;

// FOR SPI
byte value = 0;
byte datawidth = 4;

// FOR DA
byte dac_value = 0;

char analogPin = 7;
//char s=0;
char Vref = DAC1;//12
// char dir = 3;//3
char br = 5;//
char power = 3;
int sw;

float T_last;

// IMU hysteresis
int s_th, s_d_th;
int s = 1;
int thm_arr[10] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180};

void setup() {
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

  Wire1.begin();

  Serial.begin(115200);
  Serial.setTimeout(50);
  //  IMU
  /* Initialise the sensor */
  Serial.println("Sensor start"); Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);
  //  MOTOR
  // pinMode(dir, OUTPUT);
  // pinMode(br, OUTPUT);
  pinMode(power, OUTPUT);
  pinMode(Vref, OUTPUT);
  digitalWrite(power, HIGH);
  // digitalWrite(dir,HIGH);
  // digitalWrite(br,LOW);
  analogWriteResolution(12);
  // digitalWrite(br,HIGH);
}

void loop() {
//  keyBOARD();
  t = micros();
  //  IMU data evaluation
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    th = euler.z() + offset_th;
    d_th = (th - th_last)/(t - t_last)/1e-6;

    hysteresis_relay(&s_th, th, th_threshold);
    hysteresis_relay(&s_d_th, d_th, d_th_threshold);
    s = s_th * s_d_th;
  
    // Input calculation
    thm_target = thm_delta * (1 + s);
 
  PID();

  //   Printout
  Serial.print("time[ms] = ");
  Serial.print(t * 1e-6);
  Serial.print('\t');
  Serial.print("theta = ");
  Serial.print(th);
  Serial.print('\t');
  Serial.print("dtheta/dt = ");
  Serial.print(d_th);
  Serial.print('\t');
  Serial.print("sign(theta*dtheta/dt) = ");
  Serial.print(s);
  Serial.print('\t');
  Serial.print("thm = ");
  Serial.print(thm);
   Serial.print('\t');
  Serial.print("duty = ");
  Serial.print(duty);
  Serial.print('\t');
  Serial.print("Current = ");
  Serial.println(duty/4096*3.3);
  th_last = th;
  t_last = t;
}

void hysteresis_relay(int* sign, const float val, const float thr) {
  if (val > thr)
    *sign = 1;
  else if (val < -thr)
    *sign = -1;
//  else
//    *sign = 0;
}


inline void PID() {
  t_pid = micros();
  long count = read_counter();
  //  count = count % 16384;
  thm = 360 * count / 16384;

  dt = (t_pid - t_pid_last) * 1e-6;

  P = thm_target - thm;
  D = (P - P_last) / dt;
  U = Kp * P + Kd * D;

  duty = (U > 2000) ? 4000 : (U < -2000) ? 0 : U + 2000;
  analogWrite(Vref, duty);

  t_pid_last = t_pid;
  P_last = P;
  thm_last = thm;
}

unsigned long read_counter() {
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

void putDAC(byte dac_value) {
  Wire1.beginTransmission(PCF8591);    //Calls the 8591 to attention.
  Wire1.write(PCF8591_DAC_ENABLE);    //Send a DAC enable word.
  Wire1.write(dac_value);            //Send the desired DAC value (0-255)
  Wire1.endTransmission();
}

void keyBOARD() {
  char val;
  if (Serial.available() > 0) {
    val = Serial.read();
    if (val == 'a') {
      s = 1;
    } else if (val == 's') {
      s = -1;
    }
  }
}

void ThmCon() {
  int i;
  while (i < 10) {
    t = millis();
    if (t - t_last >= 20) {
      thm_target = thm_arr[i];
      t_last = t;
      i++;
    }
    PID();
  }
}
