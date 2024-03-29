#include "robot.hpp"
#include <math.h>

using namespace Eigen;

stateClass::stateClass(){
      pose << 0, 0, 0, 0; //x,z,th_m,th
      velo << 0, 0, 0, 0;
      torque = 0;
}

robotClass::robotClass(){

}

stateClass robotClass::aerial_eom(stateClass state){  //返り値はstateClass,eom自体はrobotClass
      double x = state.pose[0];
      double z = state.pose[1];
      double th_m = state.pose[2];
      double th = state.pose[3];
      double d_x = state.velo[0];
      double d_z = state.velo[1];
      double d_th_m = state.velo[2];
      double d_th = state.velo[3];

      Matrix<double,4,4> inertia; 
            inertia(0,0)= m_1+m_2;
            inertia(0,1)= 0;
            inertia(0,2)= l_1*m_2*sin(th_m)*sin(th);
            inertia(0,3)= -((2*l_1*m_2*cos(th_m)-L_ROBOT*m_2-L_ROBOT*m_1)*cos(th))/2;
            inertia(1,0)= 0;
            inertia(1,1)= m_1+m_2;
            inertia(1,2)= -l_1*m_2*sin(th_m)*cos(th);               
            inertia(1,3)= -((2*l_1*m_2*cos(th_m)-L_ROBOT*m_2-L_ROBOT*m_1)*sin(th))/2;
            inertia(2,0)= l_1*m_2*sin(th_m)*sin(th);
            inertia(2,1)= -l_1*m_2*sin(th_m)*cos(th);
            inertia(2,2)= pow(l_1,2)*m_2*pow(sin(th_m),2)*pow(sin(th),2)+pow(l_1,2)*m_2*pow(sin(th_m),2)*pow(cos(th),2)+j_l;
            inertia(2,3)= 0;
            inertia(3,0)= -((2*l_1*m_2*cos(th_m)-L_ROBOT*m_2-L_ROBOT*m_1)*cos(th))/2;
            inertia(3,1)= -((2*l_1*m_2*cos(th_m)-L_ROBOT*m_2-L_ROBOT*m_1)*sin(th))/2;
            inertia(3,2)= 0;
            inertia(3,3)= ((4*pow(l_1,2)*m_2*pow(cos(th_m),2)-4*L_ROBOT*l_1*m_2*cos(th_m)+pow(L_ROBOT,2)*m_2+pow(L_ROBOT,2)*m_1)*pow(sin(th),2)+(4*pow(l_1,2)*m_2*pow(cos(th_m),2)-4*L_ROBOT*l_1*m_2*cos(th_m)+pow(L_ROBOT,2)*m_2+pow(L_ROBOT,2)*m_1)*pow(cos(th),2)+4*j_w+4*j_b)/4;

      Matrix<double,4,1> H; 
            H(0)= ((((2*pow(d_th,2)+2*pow(d_th_m,2))*cos(th_m)*l_1-pow(d_th,2)*L_ROBOT)*sin(th)+4*d_th_m*d_th*cos(th)*l_1*sin(th_m))*m_2-pow(d_th,2)*L_ROBOT*sin(th)*m_1)/2;
            H(1)= -((((2*pow(d_th,2)+2*pow(d_th_m,2))*cos(th_m)*l_1-pow(d_th,2)*L_ROBOT)*cos(th)-4*d_th_m*d_th*sin(th)*l_1*sin(th_m))*m_2-pow(d_th,2)*L_ROBOT*cos(th)*m_1)/2;
            H(2)= (((2*cos(th_m)*pow(d_th,2)+2*cos(th_m)*pow(d_th_m,2))*pow(l_1,2)-L_ROBOT*pow(d_th,2)*l_1)*m_2*sin(th_m)*pow(sin(th),2)+((2*cos(th_m)*pow(d_th,2)+2*cos(th_m)*pow(d_th_m,2))*pow(cos(th),2)*pow(l_1,2)-L_ROBOT*pow(d_th,2)*pow(cos(th),2)*l_1)*m_2*sin(th_m))/2;
            H(3)= (-2*sin(th_m)*d_th_m*d_th*pow(l_1,2)*m_2*cos(th_m)+sin(th_m)*d_th_m*d_th*L_ROBOT*l_1*m_2)*pow(cos(th),2)-2*sin(th_m)*d_th_m*d_th*pow(sin(th),2)*pow(l_1,2)*m_2*cos(th_m)+sin(th_m)*d_th_m*d_th*pow(sin(th),2)*L_ROBOT*l_1*m_2;
       
      Matrix<double,4,1> U;
            U(0)= 0;
            U(1)= -G*m_2-G*m_1;
            U(2)= G*l_1*m_2*sin(th_m)*cos(th);
            U(3)= -G*m_2*(L_ROBOT/2-l_1*cos(th_m))*sin(th)+(G*L_ROBOT*m_1*sin(th))/2;

      Matrix<double,4,1> tau;
            tau(0)= -eta_2*d_x;
            tau(1)= -eta_2*d_z;
            //   tau(2)= state.force;
            tau(2)= 0;//state.torque;
            tau(3)= -eta_2*d_th;

      Matrix<double,4,1> Acc;
            Acc = inertia.inverse()*(tau - H + U);
            // std::cout << "print inertia " << inertia << std::endl;
            
      stateClass state_diff;
            state_diff.velo = Acc;
            state_diff.pose = state.velo; 
      return state_diff; 
}

stateClass robotClass::excitation_eom(stateClass state){  //返り値はstateClass,eom自体はrobotClass
      double th_m = state.pose[2];
      double th = state.pose[3];
      double d_th_m = state.velo[2];
      double d_th = state.velo[3];

      Matrix<double,2,2> inertia;
            inertia(0,0) = j_l;
            inertia(0,1) = 0;
            inertia(1,0) = 0;
            inertia(1,1) = (4*pow(l_1,2)*m_2*pow(cos(th_m),2)+4*L_ROBOT*l_1*m_2*cos(th_m)+pow(L_ROBOT,2)*m_2+4*j_w+4*j_b)/4;
           
      Matrix<double,2,1> H; 
            H(0)= ((2*pow(l_1,2)*m_2*cos(th_m)+L_ROBOT*l_1*m_2)*sin(th_m)*pow(d_th,2))/2;
            H(1)= -2*sin(th_m)*(d_th_m)*(d_th)*pow(l_1,2)*m_2*cos(th_m)-sin(th_m)*(d_th_m)*(d_th)*L_ROBOT*l_1*m_2;

      Matrix<double,2,1> U;
            U(0)= -G*l_1*m_2*sin(th_m)*cos(th);
            U(1)= G*m_2*(-l_1*cos(th_m)-L_ROBOT/2)*sin(th)-(G*L_ROBOT*m_1*sin(th))/2;

      Matrix<double,2,1> tau;
            tau(0)= 0;//state.torque - l_1*fric*m_2*G*pow(sin(th),2) + -eta*l_1*sin(th_m)*d_th_m*sin(th);//指令トルク，摩擦抵抗，粘性抵抗
            tau(1)= -eta*d_th;

      Matrix<double,2,1> Acc;
            Acc = inertia.inverse()*(tau - H + U);
            // std::cout << "print inertia " << inertia << std::endl;
            
      stateClass state_diff;
            state_diff.pose = state.velo; 
            state_diff.velo[0] = 0;
            state_diff.velo[1] = 0;
            // theta_mを一定に
            state_diff.velo[2] = 0;
            // state_diff.velo[2] = Acc[0];//回転速度を無視しているからコメントアウト中
            
            state_diff.velo[3] = Acc[1];
            state_diff.torque = state.torque;

      //std::cout << "test" << Acc << std::endl;
      return state_diff; 
}