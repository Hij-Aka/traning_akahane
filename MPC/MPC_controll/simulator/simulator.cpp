#include "simulator.hpp"
#include <chrono>
#define STOP_SIMULATION

// #define PRINT_SIMU
#define RELEASE_TIME 1.94553
#define FINISH_TIME 3

using std::cout;
using std::endl;
using namespace std::chrono;

Simulation::Simulation(){
    simu_reset();

}
///コンストラクタ，初期設定をここに書く，一度だけ実行

void Simulation::simu_reset(){  
    // robot.state.pose << 0, 0, acos((0.00008889*514-0.05244)/l_1), 0.0174532925*-27; //x,z,theta_m,theta
    // robot.state.pose << 0, 0, M_PI/2, -M_PI/2; //x,z,theta_m,theta
    robot.state.pose << 0, 0, M_PI/3, -M_PI/2; //x,z,theta_m,theta
    // robot.state.pose << 0, 0, acos((0.00008889*390-0.05244)/l_1), 0.0174532925*-74; //x,z,theta_m,theta
    // robot.state.velo << 0, 0, INI_D_THM, 0;
    robot.state.velo << 0, 0, 0, 0;
    robot.state.torque = 0;
    c_time = 0;
    min_hand2bar = 10;  
    bar.x = SECOUND_BAR_X;
    bar.z = SECOUND_BAR_Z;
    
    release_bar = 0;

    log_simu.open("log_simu.csv",std::ios::trunc);
    log_simu  << "time\t"
            << "x\tz\ttheta_m[deg]\ttheta[deg]\t"
            << "dx\tdz\tdtheta_m[deg]\tdtheta[deg]\t"
            << "r\t"
            << "torque\t"
            << "hand2bar\t"
            << std::flush;
    log_simu << std::endl;
}

void Simulation::set_state(double state[]){
    x = state[0];
    z = state[1];
    thm = state[2];
    th = state[3];
    dx = state[4];
    dz = state[5];
    dthm = state[6];
    dth = state[7];
}

void Simulation::kinematics(){
    origin.x = x;
    origin.z = z;
    r = l_1*cos(thm);
    hand.x = x + L_ROBOT*sin(th);
    hand.z = z - L_ROBOT*cos(th);
    weight.x = x+(r + L_ROBOT/2)*sin(th);
    weight.z = z-(r + L_ROBOT/2)*cos(th);
}

void Simulation::simu_loop(){
    //std::ofstream ofs_csv_file(output_csv_file_path);
    logging();
    auto start = std::chrono::system_clock::now(); ///計測スタート
    while(1){
        if(c_time > RELEASE_TIME) release_bar = 1;
        simu_step();
        if(c_time > FINISH_TIME) break;
    }
    auto end = std::chrono::system_clock::now();
    auto dur = end - start; 
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    std::cout << msec << " milli sec \n";
}

void Simulation::simu_step(){
    #ifdef PRINT_SIMU
    std::cout << "time : " << c_time << " x : " << robot.state.pose[0] << " d_x : " << robot.state.velo[0] << " z : " << robot.state.pose[1] << " d_z : " << robot.state.velo[1] << " theta_m : " << 360/2/M_PI*robot.state.pose[2] << " d_theta_m : " << robot.state.velo[2] << " r : " << l_1*cos(robot.state.pose[2]) << " theta : " << 360/2/M_PI*robot.state.pose[3] << " d_theta : " << robot.state.velo[3] << std::endl;
    #endif

    if(release_bar){
        // cout << "--AERIAL MODEL--" << endl;
        robot.state = RungeKutta(robotClass::aerial_eom, robot.state);
        double h2b = get_hand2bar();
        if(h2b < min_hand2bar) {
            min_hand2bar = h2b;///評価関数を更新
            finish_time = c_time;///評価関数を更新した時の時刻を更新
            }//std::cout << "finish_time " << finish_time <<std::endl;
    } 
    else robot.state = RungeKutta(robotClass::excitation_eom, robot.state);
    //  robot.state.pose[2] = M_PI/3; //x,z,theta_m,theta
    robot.state.velo[2] = INI_D_THM;
    // robot.state.velo[2] = 0;
    // robot.state.velo[2] = Ea/Kt-R_m/pow(Kt,2)*robot.state.torque;

    kinematics();

    /// update the _time
    c_time += delta_t;
    logging();

    #ifndef STOP_SIMULATION
        getchar();
    #endif
}

 stateClass Simulation::RungeKutta(stateClass (*func)(stateClass), stateClass state){ ///*funcがポインタ関数（eom)、stateClassがeomの引数
 
    stateClass state_org = state;
    stateClass K1,K2,K3,K4;  
    
    K1.pose = delta_t * (*func)(state).pose; 
    K1.velo = delta_t * (*func)(state).velo;

    state.pose = state_org.pose + 0.5*K1.pose;
    state.velo = state_org.velo + 0.5*K1.velo;
    K2.pose = delta_t * (*func)(state).pose;
    K2.velo = delta_t * (*func)(state).velo;

    state.pose = state_org.pose + 0.5*K2.pose;
    state.velo = state_org.velo + 0.5*K2.velo;
    K3.pose = delta_t * (*func)(state).pose;
    K3.velo = delta_t * (*func)(state).velo;

    state.pose = state_org.pose + K3.pose;
    state.velo = state_org.velo + K3.velo;
    K4.pose = delta_t * (*func)(state).pose;
    K4.velo = delta_t * (*func)(state).velo;

    stateClass new_state;
    new_state.pose = state_org.pose + (K1.pose + 2*K2.pose + 2*K3.pose + K4.pose)/6;
    new_state.velo = state_org.velo + (K1.velo + 2*K2.velo + 2*K3.velo + K4.velo)/6;
    // state.accel = (*func)(state).velo;
    new_state.torque = state_org.torque;
    
    return new_state;
    ///EOMをもとに状態の変化量を出力する

}

double Simulation::get_hand2bar(){
    double hand2bar;
    hand2bar = sqrt(pow(bar.x-hand.x,2) + pow(bar.z-hand.z,2));
    return hand2bar;
}

// void Simulation::logging(){
//     log_simu    << c_time << "\t"
//                 << x <<"\t"<< z << "\t"<< thm << "\t"<< th << "\t"
//                 << dx <<"\t"<< dz << "\t"<< dthm << "\t"<< dth << "\t"
//                 << l_1*cos(thm) << "\t"
//                 << robot.state.torque << "\t"
//                 << min_hand2bar
//                 <<endl;

// }

void Simulation::logging(){
    log_simu    << c_time << "\t"
                << x <<"\t"<< z << "\t"<< thm*R2D << "\t"<< th*R2D << "\t"
                << dx <<"\t"<< dz << "\t"<< dthm*R2D << "\t"<< dth*R2D << "\t"
                << l_1*cos(thm) << "\t"
                << robot.state.torque << "\t"
                << min_hand2bar
                <<endl;

}