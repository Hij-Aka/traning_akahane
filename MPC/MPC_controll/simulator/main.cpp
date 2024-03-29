
#include <stdio.h>
#include "robot.hpp"
#include "link_config.hpp"
#include "simulator.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

Simulation simu;

int main(){
    simu.simu_loop();
    system("gnuplot plot.plt");
    return 0;
}


