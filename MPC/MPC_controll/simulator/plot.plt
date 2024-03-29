set datafile separator "\t"
 
file="./log_simu.csv"
set terminal png 
set output "./fig/r.png"
set grid
set xlabel "Time[sec]"
set ylabel "r [m]"
p [][-0.05:0.05] file u 1:10 axis x1y1 title "r : weight position" w l lw 1;


set terminal png 
set output "./fig/theta-r.png"
set grid
ymax = 180
ymin = -180
set yrange  [ymin:ymax]
set xlabel "Time[sec]"
set ylabel "theta []"
set y2tics
set y2label "r [m]"
set y2range [-0.05:0.05]
p [][] file u 1:5 axis x1y1 title "theta : robot orientation" w l lw 1, file u 1:10 axis x1y2 title "r : weight position" w l lw 1;

set terminal png 
set output "./fig/x-z.png"
set grid
set xlabel "Time[sec]"
set ylabel "position [m]"
p [][] file u 1:2 axis x1y1 title "x : Horizontal position" w l lw 1, file u 1:3 axis x1y1 title "z : Vir./vitical position" w l lw 1;

set terminal png 
set output "./fig/h2b.png"
set grid
set xlabel "Time[sec]"
set ylabel "Distance[m]"
p [][0:0.5] file u 1:12 axis x1y1 title "Distance beween the hand and  the bar" w l lw 1;