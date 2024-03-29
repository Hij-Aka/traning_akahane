#ifndef _VISUALIZE_HPP_
#define _VISUALIZE_HPP_

#include "../simulator/simulator.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include <typeinfo>
#include <unistd.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include "../eigen3/Eigen/Dense"
#include <GL/glut.h>

using namespace std::chrono;
using std::string;

#define IMAGE_WIDTH 720.0
#define IMAGE_HEIGHT 480.0
#define RATIO (IMAGE_WIDTH/IMAGE_HEIGHT)

#define PLOT_SIZE 1
#define X_POS 0.3
#define X_DISP (PLOT_SIZE/RATIO)
#define Y_POS 0
#define Y_DISP (PLOT_SIZE)


class Visualize
{   
    private:
    static Simulation simu;

    static double time_current;

    static void display();
    static std::vector<string> read_data(string& input, char delimiter);
    static void set_data();
    static void printSimulation(void);
    static void DrawString(string str, int w, int h, int x0, int y0);
    static void printCircle(int n,double x,double y,double r); 
    static void printArc(double x,double y,double r,double rad);
    void init(void);
    static void idle(void);

    public:
    Visualize();
    int visualize(int argc, char *argv[]);
};

#endif 