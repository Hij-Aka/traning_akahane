#ifndef _OPTIMIZE_H_
#define _OPTIMIZE_H_

#include "../simulator/simulator.hpp"

#define TARGET_PARTITION 1//最適化する次元
#define ACCEPTANCE_J 0.05 //評価関数の目標値(m)
#define COUNT_NUM 10//最急降下行うmax回数
// #define SIMU_STEPS 2000　//何msシミュレータ回すか

class SteepestDescent{
    private:

    public:
    double k; 
    double J, J_pre;
    int step_count;
    double input_state[8];
    double initial_time;
    double finish_time;
    double pre_initial_time;
    std::ofstream log_SD;
    Eigen::Matrix<double, TARGET_PARTITION, 1> delta_J;
    Eigen::Matrix<double, TARGET_PARTITION, 1> input; // 
    Eigen::Matrix<double, TARGET_PARTITION, 1> input_tmp;
    Eigen::Matrix<double, TARGET_PARTITION, 1> input_pre;
    Eigen::Matrix<double, TARGET_PARTITION, 1> add_input;
    Eigen::Matrix<double, TARGET_PARTITION, 1> delta_input;
    Eigen::Matrix<double, TARGET_PARTITION, 1> diff();
    Simulation simu;

    SteepestDescent();
    void optimize();
    void step();
    void logging();
    void first_logging();

    double simu_loop_opti(Eigen::Matrix<double, TARGET_PARTITION, 1> inp, double state_ini[]);
    void simu_loop_opti_con(Eigen::Matrix<double, TARGET_PARTITION, 1> inp, double state_ini[],double *j,double *fin_time);

};

#endif