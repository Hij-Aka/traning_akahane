set datafile separator "\t"

file="./log_SD.csv" 

set terminal png 
set output "./fig/j-releaseTime.png"
set grid
set xlabel "step[sec]"
set ylabel "Distance [m]"
set y2tics
set y2label "Release Time [sec]"
set autoscale x
set autoscale y2
p [][] file u 1:2 axis x1y1 title "J : Distance between hand to bar" w l lw 1, file u 1:3 axis x1y2 title "Release Time" w l lw 1;
