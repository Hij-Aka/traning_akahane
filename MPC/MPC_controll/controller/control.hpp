#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_
 
#include <time.h>
#include <chrono>
#include <thread>
#include <iostream>
#include "../optimizer/optimize.hpp"
 
class Control{
    private:
    
    public: 
        Control();
        SteepestDescent sd;
        std::ofstream log_opti;
        double start_time;
        double current_time;
        double release_time = 0;
        double finish_time;
        double pre_release_time = 1.8;
        unsigned int opti_count;
        double r_get;
        double th_get;
        double dr_get;
        double dth_get;
        bool converge;
        void start();   
        void sendData(double t);
        void readData(int arr[5]);
        void optimize();
        void first_logging();
        void logging();
};
 
#endif