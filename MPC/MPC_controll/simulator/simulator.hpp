#ifndef _SIMULATOR_HPP_
#define _SIMULATOR_HPP_

#include "robot.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Coordinates{
    public:
    double x;
    double z;
};

class Simulation{
    public:
    const double delta_t = 1E-3; 
    double c_time = 0;
    double DELTA_T = 0.1;
    double finish_time;
    bool release_bar;

    robotClass robot;
    double&x = robot.state.pose[0];
    double&z = robot.state.pose[1];
    double&thm = robot.state.pose[2];
    double&th = robot.state.pose[3];
    double&dx = robot.state.velo[0];
    double&dz = robot.state.velo[1];
    double&dthm = robot.state.velo[2];
    double&dth = robot.state.velo[3];
    double r=0;
    std::string output_csv_file_path = "./period.csv";
    double min_hand2bar;
    
    Coordinates origin;
    Coordinates hand;
    Coordinates weight;
    Coordinates bar;

    std::ofstream log_simu;

    Simulation();
    void simu_reset();
    void set_state(double state[]);
    void kinematics();
    void simu_loop();
    void simu_step();
    double get_hand2bar();
    void logging();

    stateClass RungeKutta(stateClass (*func)(stateClass), stateClass state);
    

    private:
};

#endif