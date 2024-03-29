#include "visualize.hpp"
#include <iostream>
#include <stdlib.h>

Visualize visu;

int main(int argc, char *argv[]){
    system("cp ../simulator/log_simu.csv ./status.csv");
    system("export DISPLAY=:0");
    visu.visualize(argc,argv);
    return 0;
}
