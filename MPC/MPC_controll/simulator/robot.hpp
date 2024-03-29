#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "../eigen3/Eigen/Eigen"
#include <iostream>
#include "link_config.hpp"

using namespace Eigen;
//namespaceでEigen::を省略

class stateClass{
    public:
    stateClass();
    Eigen::Matrix<double, 4, 1> pose;
    Eigen::Matrix<double, 4, 1> velo;
    Eigen::Matrix<double, 4, 1> accel;
    double torque;
};

class robotClass{
    public:
    robotClass();
    stateClass state;
    static stateClass aerial_eom(stateClass state);
    static stateClass excitation_eom(stateClass state);
    //static をつけるとメンバが共有される
};

#endif 