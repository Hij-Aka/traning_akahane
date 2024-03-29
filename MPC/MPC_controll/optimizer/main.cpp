#include "optimize.hpp"

int main(){
    SteepestDescent sd;
    sd.optimize();
    system("gnuplot plot.plt");
    return 0;    
}