#include "visualize.hpp"
#define POLYGON_N 24
#define RED 0.8, 0.16, 0.21
#define BLUE 0.11, 0.35, 0.8
#define ORANGE 0.9, 0.5, 0.15
#define GREEN 0, 1.0, 0

using namespace std;
using namespace std::chrono;

using std::cout;
using std::endl;
using std::getchar;
// extern Timer system_time;

Simulation Visualize::simu;
double Visualize::time_current;


double time_step = 1E-03;

Visualize::Visualize(){
  // start_time = system_clock::now();
  time_current = 0;
}

void Visualize::display()
{ 
  set_data();
  printSimulation();
  static int once=1;
  while(once){
    std::getchar();
    once -= 1;
  }
  if(time_current < 9.9999) time_current += time_step;
  getchar();
}

std::vector<string> Visualize::read_data(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

void Visualize::set_data(){
  static ifstream ifs("status.csv");
  static string line;
  static int once=2;
  while(once){
    getline(ifs, line);
    once -= 1;
  }
  cout<<"debug"<<endl;
  if (getline(ifs, line)) {
    vector<string> strvec = read_data(line, '\t');
    // for (int i=0; i<5;i++){
    //   state_visu->pose[i] = stod(strvec.at(i+1));
    //   state_visu->velo[i] = stod(strvec.at(i+6));
    // }
    // state_visu->torque[0] =stod(strvec.at(11));
    // state_visu->torque[1] =stod(strvec.at(12));
    // state_visu->torque[2] =stod(strvec.at(13));
    // state_visu->external_forces[4] =stod(strvec.at(14));
    // robot_virtical = state_visu->pose[0];
    // state_visu->pose[0] = 0;
    simu.x = stod(strvec.at(1));
    simu.z = stod(strvec.at(2));
    simu.thm = stod(strvec.at(3))*D2R;
    simu.th = stod(strvec.at(4))*D2R;
    simu.kinematics();

    std::cout << "READING DATE" << std::endl;
    cout << "x: " <<simu.x<< " z: "<<simu.z<< " thm: "<<simu.thm<< " th: "<<simu.th<< " r: "<<simu.r<<endl;
    cout << "o_x: " <<simu.origin.x<< " o_z: "<<simu.origin.z<<endl;
    cout << "l_hand_x: " <<simu.hand.x<< " l_hand_z: "<<simu.hand.z<<endl;
    cout << "w_x: " <<simu.weight.x<< " l_hand_z: "<<simu.weight.z<<endl;
  }
}

void Visualize::printSimulation()
{

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3d(BLUE);
  glPointSize(12);

  glBegin(GL_POINTS);
  //glVertex2d(-simu.origin.x*X_DISP+X_POS, simu.origin.z*Y_DISP);
  glVertex2d(-simu.hand.x*X_DISP+X_POS, simu.hand.z*Y_DISP);
  glVertex2d(-simu.weight.x*X_DISP+X_POS, simu.weight.z*Y_DISP);
  //glVertex2d(-simu.bar.x*X_DISP+X_POS, simu.bar.z*Y_DISP);
  glEnd();
  
  glColor3d(GREEN);
  glBegin(GL_POINTS);
  glVertex2d(-simu.origin.x*X_DISP+X_POS, simu.origin.z*Y_DISP);
  glEnd();

  glColor3d(RED);
  glBegin(GL_POINTS);
  glVertex2d(-simu.bar.x*X_DISP+X_POS, simu.bar.z*Y_DISP);
  glEnd();

  glLineWidth(6);
  glColor3d(BLUE);
  glBegin(GL_LINE_STRIP);
  glVertex2d(-simu.origin.x*X_DISP+X_POS, simu.origin.z*Y_DISP);
  glVertex2d(-simu.hand.x*X_DISP+X_POS, simu.hand.z*Y_DISP);
  glEnd();

  stringstream time;
  time << "TIME : " <<  time_current <<" [sec]";
  // jt << "LINK JOINT MOTOR TORQUE : " << state_visu->external_forces[4] <<" [N m]"; 
  // ft << "FORWARD MOTOR TORQUE : " <<  state_visu->torque[0]<<" [N m]";
  // mt << "MIDDLE MOTOR TORQUE : " <<  state_visu->torque[1]<<" [N m]";
  // bt << "REAR MOTOR TORQUE : " <<  state_visu->torque[2]<<" [N m]";
  // // st << "Spring Torque: " << state_visu->springTorque;
  // st << "Spring Torque: " << state_visu->external_forces[4];
  double font_pose = 20;
  DrawString(time.str(), IMAGE_WIDTH, IMAGE_HEIGHT, font_pose, font_pose);
  // DrawString(jt.str(), IMAGE_WIDTH, IMAGE_HEIGHT, font_pose, font_pose*2);
  // DrawString(ft.str(), IMAGE_WIDTH, IMAGE_HEIGHT, 20, 60);
  // DrawString(mt.str(), IMAGE_WIDTH, IMAGE_HEIGHT, 20, 80);
  // DrawString(bt.str(), IMAGE_WIDTH, IMAGE_HEIGHT, 20, 100);

  glFlush();  
}

int Visualize::visualize(int argc, char *argv[])
{ 
  time_current = 0;
  
  glutInitWindowPosition(1000, 50);
  glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  init();
  glutPostRedisplay();
  glutIdleFunc(idle);
  display();
  glutMainLoop();
  return 0;
}
// void Visualize::printCircle(int n, double x, double y, double r){
//   double rate;
//    glBegin(GL_LINE_STRIP); // ポリゴンの描画
//   // 円を描画
//   for (int i = 0; i < n+1; i++) {
//     // 座標を計算
//     rate = (double)i / n;
//     double _x = r * cos(2.0 * M_PI * rate) * PLOT_RATE / RATIO_XY  + x ;
//     double _y = r * sin(2.0 * M_PI * rate) *PLOT_RATE + y;
//     glVertex3f(_x, _y, 0.0); // 頂点座標を指定
//   }
//   glEnd(); 
// }

// void Visualize::printArc(double x,double y,double r,double rad){
//   double rate;
//   int n = 32;
//   glBegin(GL_LINE_STRIP); // ポリゴンの描画
//   // 円を描画
//   rad = rad * 2*M_PI;
//   for (int i = 0; i < n+1; i++) {
//     // 座標を計算
//     rate = (double)i / n;
//     double _x = r * cos(rad * rate) * PLOT_RATE / RATIO_XY  + x *PLOT_RATE/ratio+X_DISP;
//     double _y = r * sin(rad * rate) * PLOT_RATE + y*PLOT_RATE+Y_DISP;
//     glVertex3f(_x, _y, 0.0); // 頂点座標を指定
//   }
//   glEnd(); 
//   double arrowSize = 0.013;
//   if(rad<0) arrowSize *= -1;
//   if(rad==0) arrowSize = 0;
//   double _x = (r) * PLOT_RATE / RATIO_XY  + x *PLOT_RATE/ratio+X_DISP;
//   double _x_1 = (r+arrowSize) * PLOT_RATE / RATIO_XY  + x *PLOT_RATE/ratio+X_DISP;
//   double _x_2 = (r-arrowSize) * PLOT_RATE / RATIO_XY  + x *PLOT_RATE/ratio+X_DISP;
//   double _y = (y-arrowSize) *PLOT_RATE+Y_DISP;
//   glBegin(GL_TRIANGLES);
//   glVertex2d(_x_1, y*PLOT_RATE+Y_DISP);
//   glVertex2d(_x_2, y*PLOT_RATE+Y_DISP);
//   glVertex2d(_x, _y);
//   glEnd(); 
// }

void Visualize::DrawString(string str, int w, int h, int x0, int y0)
{
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    int size = (int)str.size();
    for(int i = 0; i < size; ++i){
        char ic = str[i];
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ic);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void Visualize::init(void)
{
  glClearColor(0.7, 0.7, 0.7, 1.0);
}

void Visualize::idle(void)
{ 
  glutPostRedisplay();
  //printJoint(state);
  // std::cout << calcEnergy(state) << " " << ReactForce << std::endl;
}

