# brachiation_simulator_share
It has 4 directory (simulator,optimizer,visualizer,controller).
"Simulator" is a program that solves the equations of motion for the robot using Runge-Kutta4.
"Optimizer" is a program that optimizes the evaluated value to be smaller than the target value using the steepest descent method.
"Visualizer" is a program that visualizes the robot's motion based on the time series data calculated by "simulator".
"Controller" is a program that communicates serially with the actual robot, reads the sensor status, and optimizes the robot's trajectory in real time accordingly.
