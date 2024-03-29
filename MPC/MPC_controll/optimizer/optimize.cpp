#include "optimize.hpp"
#include <iostream>
#include <fstream>
#include "../eigen3/Eigen/Dense"
#include <stdlib.h>
#include <chrono>

#define STEP_N 1000
#define SIMU_STEP 3000//3700
// #define STOP_OPTIMIZE
// #define PRINT_OPTI
#define OUTPUT_log_SD

using std::cout;
using std::endl;
using namespace std::chrono;
// int main(){
//     SteepestDescent sd;
//     sd.optimize();
//     system("gnuplot plot.plt");
//     return 0;    
// }

// double input_state[8] = {0, 0, -M_PI/2, -M_PI/2, 0, 0, INI_D_THM, 0};

SteepestDescent::SteepestDescent(){
    // コンストラクタ
    // インスタンス生成時に実行される関数   
    #ifdef OUTPUT_log_SD
    first_logging();
    #endif
    step_count = 0;
    k = 0.02;  
    input_pre << 0;
    input << 1.9; 
    input_tmp << 0;
    add_input << 0;
    delta_input << -0.05;
    input_state[0]= 0;
    input_state[1]= 0;
    input_state[2]= M_PI/2;//thm
    input_state[3]= -M_PI/2;//th
    input_state[4]= 0;
    input_state[5]= 0;
    input_state[6]= 0;//INI_D_THM;//d_thm
    input_state[7]= 0;//0;//8;//0;//-11//10;//d_th
    
}

Eigen::Matrix<double, TARGET_PARTITION, 1> SteepestDescent::diff(){
    Eigen::Matrix<double, TARGET_PARTITION, 1> delta;
    for(int i=0; i<TARGET_PARTITION; i++){
        delta[i] = (J - J_pre)/delta_input[i];
    }
    return delta;
}

void SteepestDescent::optimize() {
    std::cout << "OPTIMIZE!!\n" << J << std::endl;
    // // clock_t start = clock();
    auto start = std::chrono::system_clock::now(); //計測スタート
    for(int i=0;i<COUNT_NUM;i++){
        step();
        if(J< ACCEPTANCE_J){/*  */            
            std::cout  << "input "<< input << "\n";
            std::cout  << "finish_time "<< finish_time <<  "\n";
            // cout << "get solution!" << endl;
            break;
        } 
    }
    auto end = std::chrono::system_clock::now();
    auto dur = end - start; 
    // clock_t end = clock();
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    // // 要した時間をミリ秒（1/1000秒）に変換して表示
    // double time = 1000. * (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << msec << " milli sec \n";
    // getchar();
    // simu_loop_opti(input);
    std::cout << "finish!!" << std::endl;
    return;
} 

void SteepestDescent::step(){    
    // current state
    simu_loop_opti_con(input, input_state, &J, &finish_time);
    
    #ifdef PRINT_OPTI
    std::cout << "STEP_" << step_count << std::endl;
    std::cout << "J: " << J<< std::endl;
    std::cout << " input: " <<input<< "\n input_pre: "<< input_pre << std::endl;
    #endif

    #ifdef OUTPUT_log_SD
    logging();
    #endif

    // 勾配による探索
    for(int i=0; i<TARGET_PARTITION; i++){
        input_tmp = input;
        // input[i] を delta_input[i]だけ変化させて，シミュレート
        input_tmp[i] += delta_input[i];
        // cout << " input_tmp" << input_tmp[i] << endl;
        double d_j,d_finish_time;
        simu_loop_opti_con(input_tmp, input_state, &d_j, &d_finish_time);
        delta_J[i] = d_j - J;
        // cout << " d_j" << d_j<< endl;
        add_input[i] = -k* delta_J[i] / delta_input[i];
        // std::getchar(); 
    }

    input_pre = input;
    input += add_input;
    
    // simu_loop_opti_con(input, input_state, &J, &finish_time);   

    #ifdef PRINT_OPTI
    std::cout << "delta_J: " <<delta_J[0] << std::endl;
    std::cout << "add_input: " << add_input << std::endl;
    std::cout << "input_next: " <<input << std::endl;
    std::cout << "next!!\n" << std::endl;
    #endif

    step_count ++;
    #ifdef STOP_OPTIMIZE
    std::getchar(); 
    #endif
}

// void SteepestDescent::step(){    
//     // current state
//     J = simu_loop_opti(input, input_state);
//     // std::cout << "J!!" << J << std::endl;
    
//     #ifdef PRINT_OPTI
//     std::cout << "STEP_" << step_count << std::endl;
//     std::cout << "J: " << J<< std::endl;
//     std::cout << " input: " <<input<< "\n input_pre: "<< input_pre << std::endl;
//     #endif

//     // logging();

//     // 勾配による探索
//     for(int i=0; i<TARGET_PARTITION; i++){
//         input_tmp = input;
//         // input[i] を delta_input[i]だけ変化させて，シミュレート
//         input_tmp[i] += delta_input[i];
//         // cout << " input_tmp" << input_tmp[i] << endl;
//         double d_j = simu_loop_opti(input_tmp, input_state);
//         delta_J[i] = d_j - J;
//         // cout << " d_j" << d_j<< endl;
//         add_input[i] = -k* delta_J[i] / delta_input[i];
//         // std::getchar(); 
//     }

//     input_pre = input;
//     input += add_input;

//     #ifdef PRINT_OPTI
//     std::cout << "delta_J: " <<delta_J[0] << std::endl;
//     std::cout << "add_input: " << add_input << std::endl;
//     std::cout << "input_next: " <<input << std::endl;
//     std::cout << "next!!\n" << std::endl;
//     #endif

//     step_count ++;
//     #ifdef STOP_OPTIMIZE
//     std::getchar(); 
//     #endif
// }

void SteepestDescent::first_logging(){
    log_SD.open("log_SD.csv",std::ios::trunc);
    // log_SD.open("log_SD.csv",std::ios::app);
    log_SD  << "STEP\t"
            << "J\t"
            << std::flush;
    for(int i=0; i<TARGET_PARTITION; i++){
        log_SD << "input\t" << std::flush ;
    }
    for(int i=0; i<TARGET_PARTITION; i++){
        log_SD << "add_input\t" << std::flush ;
    }
    log_SD << std::endl;
}

void SteepestDescent::logging(){
    log_SD  << step_count << "\t" 
            << J << "\t" << std::flush;
    for(int i=0; i<TARGET_PARTITION; i++){
        log_SD << input[i] <<"\t" << std::flush ;
    }      
    for(int i=0; i<TARGET_PARTITION; i++){
        log_SD << add_input[i] <<"\t" << std::flush ;
    }
    log_SD << std::endl;
}

double SteepestDescent::simu_loop_opti(Eigen::Matrix<double, TARGET_PARTITION, 1> inp, double state_ini[]){
    double J=1;
    simu.simu_reset();
    simu.set_state(state_ini);
    for(int i=0;i<SIMU_STEP;i++){
        if(simu.c_time > inp[0]) simu.release_bar = 1;
        simu.simu_step();
        if(simu.z < -1){
            break;
        } 
    }
    J = simu.min_hand2bar;

    return J;
}

void SteepestDescent::simu_loop_opti_con(Eigen::Matrix<double, TARGET_PARTITION, 1> inp, double state_ini[],double *j,double *fin_time){
    // double J=1;
    // clock_t start = clock();
    simu.simu_reset();
    simu.set_state(state_ini);
    // auto start = std::chrono::system_clock::now(); //計測スタート
    for(int i=0;i<SIMU_STEP;i++){
        if(simu.c_time > inp[0]) simu.release_bar = 1;
        simu.simu_step();
        if(simu.z < -1){
            break;
        } 
    }
    // auto end = std::chrono::system_clock::now();
    // auto dur = end - start; 
    // clock_t end = clock();
    // auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    // 要した時間をミリ秒（1/1000秒）に変換して表示
    // double time = 1000. * (double)(end - start) / CLOCKS_PER_SEC;
    // std::cout << msec << " milli sec \n";
    *fin_time = simu.finish_time;
    *j = simu.min_hand2bar;
    // clock_t end = clock();
    // double time = 1000. * (double)(end - start) / CLOCKS_PER_SEC;
    // std::cout << time << " milli sec \n";
}
