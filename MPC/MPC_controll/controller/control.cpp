#include "control.hpp"
#include <iostream>
#include <fstream>
#include "../eigen3/Eigen/Dense"
#include <stdlib.h>
using std::cout;
using std::endl;
 
#define FILE_DIR_con "log_opt"
 
Control::Control(){
    log_opti.open("log_opti.csv",std::ios::trunc);
    log_opti << "input\t" <<"pre_release_time\t" << "initial_time\t" <<  "thm\t" << "th\t" << "d_thm\t" << "d_th\t" << endl;  
    log_opti << "count\t" <<"step\t" << "J\t" <<  "finish_time\t" << "input\t" << endl;  
    opti_count = 0;
    release_time = 1.8;
    start();
}
 
void Control::start(){
    double start_time = (double)clock()/CLOCKS_PER_SEC;//開始時刻取得
}
 
void Control::sendData(double t){
}

void Control::readData(int arr[5]){
    // update the robot state
    sd.input_state[2] = 0.0174532925*(double)arr[0];//thm
    sd.input_state[3] = 0.0174532925*(double)arr[1];// th
    sd.input_state[4] = 0;
    sd.input_state[5] = 0;
    sd.input_state[6] = 0;//dthm
    sd.input_state[7] = 0.0174532925*(double)arr[3];// dth
 
    sd.initial_time = (double)arr[4]/1000.;//time
    std::cout << "thm " << sd.input_state[2] << "th " << sd.input_state[3] << "d_thm " << sd.input_state[6] << "d_th " << sd.input_state[7] << "time " << sd.initial_time <<std::endl;
}
 
void Control::optimize(){
    opti_count ++;
    sd.step_count = 0;
    sd.first_logging();
    sd.input << (pre_release_time - sd.initial_time);//現在時刻からバーを離すまでの時間
    log_opti    << sd.input << "\t"
                << pre_release_time << "\t"
                << sd.initial_time << "\t"
                << sd.input_state[2] << "\t"
                << sd.input_state[3] << "\t"
                << sd.input_state[6] << "\t"
                << sd.input_state[7] << "\t"
                << endl;

    for(int i=0;i<COUNT_NUM;i++){
        sd.step();
        log_opti    << opti_count << "\t"
                    << i << "\t"
                    << sd.J << "\t"
                    << sd.finish_time << "\t"
                    << sd.input_pre << "\t"
                    << endl;
        if(sd.J< ACCEPTANCE_J){
            break;
        } 
    }

    pre_release_time = sd.input[0] + sd.initial_time; 
    release_time = 1000. * sd.input[0] + 1000. * sd.initial_time;//バーを離す時刻
    finish_time = 1000. * sd.finish_time + 1000. * sd.initial_time;//空中相が終わる時刻（次のバーを掴む時刻）
    std::cout << "release_time " << release_time << " finish_time " << finish_time <<std::endl;
}
 
