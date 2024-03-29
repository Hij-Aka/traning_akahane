#include "control.hpp"
#include "serial.hpp"
#include <chrono>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#define  COUNT_CONT 10
#define  COUNT_READ 2000
using std::cout;
using std::endl;

int* ip_pre = NULL;
struct timeval myTime_start;    // time_t構造体を定義．1970年1月1日からの秒数を格納するもの
struct timeval myTime_fin;    // time_t構造体を定義．1970年1月1日からの秒数を格納するもの

 
int main(){
    Control control;
    Serial arduino;
    arduino.log();
    arduino.init();
    while(1){
        control.readData(arduino.read_two());
        if (0 < control.sd.initial_time < 10 && abs(control.sd.input_state[2]) < 10 && abs(control.sd.input_state[3]) < 10 && abs(control.sd.input_state[7]) < 200){
            control.optimize();   
            arduino.write_one((int)control.release_time,(int)control.finish_time);//Arduinoへバーを離すタイミングと掴むタイミングを送る
        }
    }
    arduino.close_s();  
  
}
 
