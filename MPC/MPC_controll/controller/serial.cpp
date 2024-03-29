#include "serial.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
 
#define FILE_DIR "log"
//#define SERIAL_PORT "/dev/ttyUSB0"
#define SERIAL_PORT "/dev/ttyACM0"
std::ofstream lg;
 
#define SIZE 68
using std::cout;
using std::endl;
 
int Serial::init(){
   fd = open(SERIAL_PORT, O_RDWR,O_TRUNC);
   if (fd < 0){
       std::cout<< "open error!" <<std::endl;
       return -1;
   }
   tio.c_cflag += CREAD;               // 受信有効
   tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
   tio.c_cflag += CS8;                 // データビット:8bit
   tio.c_cflag += 0;                   // ストップビット:1bit
   tio.c_cflag += 0;                   // パリティ:None
   cfsetispeed( &tio, baudRate );
   cfsetospeed( &tio, baudRate );
   cfmakeraw(&tio);                    // RAWモード
   tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う
   ioctl(fd, TCSETS, &tio);
   return 0;
}
// int Serial::read_s(){
//     read(fd, buf, sizeof(buf));
//     int len;
//     while(1) {
//         len = read(fd, buf, sizeof(buf));
//         if (0 < len) {
//             for(int i = 0; i < len; i++) {
//                 std::cout << buf[i] ;
//                 lg << buf[i] << std::flush;
//             }
//         }
//     }
// }
 
int Serial::log(){
   std::string file_name;
   auto now = std::chrono::system_clock::now();
   auto now_c = std::chrono::system_clock::to_time_t(now);
   std::stringstream ss;
   ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
   file_name = FILE_DIR + ss.str() + ".csv";
   std::cout << file_name << std::endl;
   lg.open(file_name, std::ios::trunc);
   return 0;
}
 
void Serial::read_one(){
   //cout << "read : "<< std::flush;
   // read(fd, buf, sizeof(buf));
   int len;
   char buff[500];
   len = read(fd, buff, sizeof(buff));
   if (0 < len) {
       for(int i = 0; i < len; i++) {
           std::cout << buff[i];
           lg << buff[i] << std::flush;
       }
       std::cout << endl;
   }
}

// int* Serial::read_pre(){
//     //cout << "read : "<< std::flush;
//     // read(fd, buf, sizeof(buf));
//     ssize_t len;
//     // char buff_2[255] = "";
//     int num_1[2];
//     int num_2[2];
//     int num_3[2];
//     int* ip = NULL;
//     // int size = 0;
//     ip = (int*)malloc(sizeof(int) * 3);
//     // char wptr[476];
//     //wptr = (char*)malloc(len);
//     //sprintf(wptr,"%s",buff);
//     time_t t_st = time(NULL);
//     while (1){
//         time_t t_en = time(NULL);
//         if ((t_en - t_st) > 5){
//         goto OUT;  
//         }
//         tcflush(fd,TCIFLUSH);
//         int ch_1 = 0;
//         int ch_2 = 0;
//         int ch_3 = 0;
//         char buff[1000] = "";
//         len = read(fd, buff, sizeof(buff));
//         // std::cout << len << std::endl;
//         // if (false)
//         // if (len < 0) break;
//         char buff_2[1000] = "";
//         int len_2 = 0;
//         lg << "time_pre " << '\t' << "r_pre" << '\t' << "th_pre" << std::endl;
//         for(int i = 0;i < len;i++){
//             char c(buff[i]);
//             // std::cout << "size" << size <<std::endl;
//             if ((ch_1 == 2)&&(ch_2 == 2)&&(ch_3 == 2)){
//             ip[0] = num_1[1];
//             ip[1] = num_2[1];
//             ip[2] = num_3[1];
//             // std::cout << ip[0] << ip[1] << std::endl;
//             // lg << ip[0] << ip[1] << std::endl;
//             // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
//             lg << ip[0] << '\t' << ip[1] << '\t' << ip[2] << std::endl;
//             // tcflush(fd,TCIFLUSH);
//             goto OUT;
//             }else if((c == '!')){   // 区切り        tcflush(fd,TCIFLUSH);

//                 num_1[ch_1] = atof(buff_2);//time
//                 // std::cout << num_1[0] << std::endl;
//                 strcpy(buff_2, ""); // 行バッファクリア
//                 ch_1++;
//                 // size = 0;
//             }else if((c == '"')){
//                 num_2[ch_2] = atof(buff_2);//position
//                 // std::cout << num_2[ch_2] << std::endl;
//                 // printf( "Position%d\n", num_2);
//                 strcpy(buff_2, ""); // 行バッファクリア
//                 ch_2++;
//                 // size = 0;
//             }else if(c == '#'){
//                 num_3[ch_3] = atof(buff_2);//angle
//                 // std::cout << num_3[ch_3] << std::endl;
//                 // printf( "Angle%d\n", num_3);
//                 strcpy(buff_2, ""); // 行バッファクリア
//                 ch_3++;
//                 // size = 0;
//             // }else if ((c == '\b')&&(ch_1 == 1)){
//             //     num_1[1] = atoi(buff_2);
//             //     std::cout << num_1[1] << std::endl;
//             //     strcpy(buff_2, "");  // 行バッファクリア
//             //     ch_1 = 2;
//             // }else if((c == '\t')&&(ch_2 == 2)){
//             //     num_2[1] = atoi(buff_2);
//             //     // std::cout << num_2[ch_2] << std::endl;
//             //     // printf( "Position%d\n", num_2);
//             //     strcpy(buff_2, "");  // 行バッファクリア
//             // }else if((c == '\n')&&(ch_3 == 2)){
//             //     num_3[1] = atoi(buff_2);
//             //     // std::cout << num_3[ch_3] << std::endl;
//             //     // printf( "Angle%d\n", num_3);
//             //     strcpy(buff_2, "");  // 行バッファクリア
//             }else{
//                 len_2 = strlen(buff_2);
//                 buff_2[len_2] = c;
//                 //std::cout << "len_2" << len_2 << std::endl;  
//                 buff_2[len_2+1] = '\0';
//                 // size++;
//             }
//                 //     if ((ch_1 == 1)&&(ch_2 == 1)&&(ch_3 == 1)){
//                 // ip[0] = num_2[1];
//                 // ip[1] = num_3[1];
//                 // ip[2] = (num_2[1]-num_2[0])/(num_1[1]-num_1[0]);
//                 // ip[3] = (num_3[1]-num_3[0])/(num_1[1]-num_1[0]);
//                 // ip[4] = num_1[1];
//                 // // std::cout << ip[0] << ip[1] << std::endl;
//                 // // lg << ip[0] << ip[1] << std::endl;
//                 // std::cout << "time_1 " << num_1[0] << "time_2 " << num_1[1] <<std::endl;
//                 // // lg << ip[0] << ip[1] << ip[2] << ip[3] << ip[4] << std::endl;
//                 // break;
//         }
//     }
// OUT:
//     return ip;
// }

// int* Serial::read_two(){
//     //cout << "read : "<< std::flush;
//     // read(fd, buf, sizeof(buf));
//     ssize_t len;
//     int num_1[3];
//     int num_2[3];
//     int num_3[3];
//     int* ip = NULL;
//     int diff_time;
//     ip = (int*)malloc(sizeof(int) * 5);//取得した速度等を格納
//     // char wptr[476];
//     //wptr = (char*)malloc(len);
//     //sprintf(wptr,"%s",buff);
//     // time_t t_st = time(NULL);
//     lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
//     while (1){
//         // time_t t_en = time(NULL);//を初期化
//         // if ((t_en - t_st) > 5){
//         // goto OUT;  
//         // }
//         tcflush(fd,TCIFLUSH);//バッファをフラッシュ
//         int ch_1,ch_2,ch_3 = 0;
//         char buff[1000] = "";
//         len = read(fd, buff, sizeof(buff));
//         // std::cout << len << std::endl;
//         // if (false)
//         // if (len > 0){
//             char buff_2[1000] = "";
//             int len_2 = 0;
//             for(int i = 0;i < len;i++){
//                 char c(buff[i]);
//                 // std::cout << "size" << size <<std::endl;
//                 if ((ch_1 == 3)&&(ch_2 == 3)&&(ch_3 == 3)){
//                     diff_time = num_1[2]-num_1[1];
//                     if(diff_time == 0){
//                         diff_time = 1;
//                     }
//                     ip[0] = num_2[2];
//                     ip[1] = num_3[2];
//                     ip[2] = (num_2[2]-num_2[1])/diff_time;
//                     ip[3] = (num_3[2]-num_3[1])/diff_time;
//                     ip[4] = num_1[2];
//                     // std::cout << ip[0] << ip[1] << std::endl;
//                     // lg << ip[0] << ip[1] << std::endl;
//                     // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
//                     lg << ip[4] << '\t' << ip[0] << '\t' << ip[2] << '\t' << ip[1] << '\t' << ip[3] << std::endl;
//                     // tcflush(fd,TCIFLUSH);
//                     goto OUT;
//                 }else if((c == '!')){   // 区切り     
//                     num_1[ch_1] = atof(buff_2);
//                     std::cout << num_1[ch_1] << std::endl;
//                     // std::cout << num_1[0] << std::endl;
//                     strcpy(buff_2, ""); // 行バッファクリア
//                     ch_1++;
//                     // size = 0;
//                 }else if((c == '"')){
//                     num_2[ch_2] = atof(buff_2);
//                     std::cout << num_2[ch_2] << std::endl;
//                     // printf( "Position%d\n", num_2);
//                     strcpy(buff_2, ""); // 行バッファクリア
//                     ch_2++;
//                     // size = 0;
//                 }else if(c == '#'){
//                     num_3[ch_3] = atof(buff_2);
//                     std::cout << num_2[ch_3] << std::endl;
//                     // printf( "Angle%d\n", num_3);
//                     strcpy(buff_2, ""); // 行バッファクリア
//                     ch_3++;
//                 }else{
//                     len_2 = strlen(buff_2);
//                     buff_2[len_2] = c;
//                     //std::cout << "len_2" << len_2 << std::endl;  
//                     buff_2[len_2+1] = '\0';
//                     // size++;
//                 }
//                     //     if ((ch_1 == 1)&&(ch_2 == 1)&&(ch_3 == 1)){
//                     // ip[0] = num_2[1];
//                     // ip[1] = num_3[1];
//                     // ip[2] = (num_2[1]-num_2[0])/(num_1[1]-num_1[0]);
//                     // ip[3] = (num_3[1]-num_3[0])/(num_1[1]-num_1[0]);
//                     // ip[4] = num_1[1];
//                     // // std::cout << ip[0] << ip[1] << std::endl;
//                     // // lg << ip[0] << ip[1] << std::endl;
//                     // std::cout << "time_1 " << num_1[0] << "time_2 " << num_1[1] <<std::endl;
//                     // // lg << ip[0] << ip[1] << ip[2] << ip[3] << ip[4] << std::endl;
//                     // break;
//             // }
//         }
//     }
// OUT:
//     return ip;
// }

int* Serial::read_two(){
    //cout << "read : "<< std::flush;
    // read(fd, buf, sizeof(buf));
    ssize_t len;
    // char buff_2[255] = "";
    int num_1[3];
    int num_2[3];
    int num_3[3];
    int num_4[3];
    int* ip = NULL;
    // int size = 0;
    ip = (int*)malloc(sizeof(int) * 5);
    // char wptr[476];
    //wptr = (char*)malloc(len);
    //sprintf(wptr,"%s",buff);
    // time_t t_st = time(NULL);
    // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
    // lg << "a " << '\t' << "a" << '\t' << "a" << '\t' << "a" << '\t' << "a" << std::endl;
    while (1){
        // time_t t_en = time(NULL);
        // if ((t_en - t_st) > 5){
        // goto OUT;  
        // }
        ONE_MORE:
        tcflush(fd,TCIFLUSH);
        int ch_1 = 0;
        int ch_2 = 0;
        int ch_3 = 0;
        int ch_4 = 0;
        char buff[1000] = "";
        len = read(fd, buff, sizeof(buff));
        // std::cout << len << std::endl;
        // if (false)
        // if (len < 0) break;
        char buff_2[1000] = "";
        int len_2 = 0;
        // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
        for(int i = 0;i < len;i++){
            char c(buff[i]);
            // std::cout << "size" << size <<std::endl;
            if(c == '$'){
                for(int j = i+1;j < len;j++){
                    char c(buff[j]);
                    if ((ch_1 == 2)&&(ch_2 == 2)&&(ch_3 == 2)&&(ch_4 == 2)){
                    // int diff_time = num_1[2]-num_1[1];
                    // if(diff_time == 0){
                    //     diff_time = 1;
                    // }
                    ip[0] = num_2[1];
                    ip[1] = num_3[1];
                    ip[2] = 0;//(num_2[2]-num_2[1])/diff_time;
                    ip[3] = num_4[1];
                    ip[4] = num_1[1];
                    // std::cout << ip[0] << ip[1] << std::endl;
                    // lg << ip[0] << ip[1] << std::endl;
                    // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
                    lg << ip[4] << '\t' << ip[0] << '\t' << ip[2] << '\t' << ip[1] << '\t' << ip[3]/1000. << std::endl;
                    // tcflush(fd,TCIFLUSH);
                    goto OUT;
                    }else if((c == '!')){   // 区切り        tcflush(fd,TCIFLUSH);
        
                        num_1[ch_1] = atof(buff_2);
                        // std::cout << num_1[0] << std::endl;
                        strcpy(buff_2, ""); // 行バッファクリア
                        ch_1++;
                        // size = 0;
                    }else if((c == '"')){
                        num_2[ch_2] = atof(buff_2);
                        // std::cout << num_2[ch_2] << std::endl;
                        // printf( "Position%d\n", num_2);
                        strcpy(buff_2, ""); // 行バッファクリア
                        ch_2++;
                        // size = 0;
                    }else if(c == '#'){
                        num_3[ch_3] = atof(buff_2);
                        // std::cout << num_3[ch_3] << std::endl;
                        // printf( "Angle%d\n", num_3);
                        strcpy(buff_2, ""); // 行バッファクリア
                        ch_3++;
                        // size = 0;
                    }else if (c == '$'){
                        num_4[ch_4] = atof(buff_2);
                        // std::cout << num_3[ch_3] << std::endl;
                        // printf( "Angle%d\n", num_3);
                        strcpy(buff_2, ""); // 行バッファクリア
                        ch_4++; 
                    //     num_1[1] = atoi(buff_2);
                    //     std::cout << num_1[1] << std::endl;
                    //     strcpy(buff_2, "");  // 行バッファクリア
                    //     ch_1 = 2;
                    // }else if((c == '\t')&&(ch_2 == 2)){
                    //     num_2[1] = atoi(buff_2);
                    //     // std::cout << num_2[ch_2] << std::endl;
                    //     // printf( "Position%d\n", num_2);
                    //     strcpy(buff_2, "");  // 行バッファクリア
                    // }else if((c == '\n')&&(ch_3 == 2)){
                    //     num_3[1] = atoi(buff_2);
                    //     // std::cout << num_3[ch_3] << std::endl;
                    //     // printf( "Angle%d\n", num_3);
                    //     strcpy(buff_2, "");  // 行バッファクリア
                    }else{
                        len_2 = strlen(buff_2);
                        buff_2[len_2] = c;
                        //std::cout << "len_2" << len_2 << std::endl;  
                        buff_2[len_2+1] = '\0';
                        // size++;
                    }
                }
                goto ONE_MORE;
            }        //     if ((ch_1 == 1)&&(ch_2 == 1)&&(ch_3 == 1)){
                // ip[0] = num_2[1];
                // ip[1] = num_3[1];
                // ip[2] = (num_2[1]-num_2[0])/(num_1[1]-num_1[0]);
                // ip[3] = (num_3[1]-num_3[0])/(num_1[1]-num_1[0]);
                // ip[4] = num_1[1];
                // // std::cout << ip[0] << ip[1] << std::endl;
                // // lg << ip[0] << ip[1] << std::endl;
                // std::cout << "time_1 " << num_1[0] << "time_2 " << num_1[1] <<std::endl;
                // // lg << ip[0] << ip[1] << ip[2] << ip[3] << ip[4] << std::endl;
                // break;
        }
    }
OUT:
    return ip;
}

// int* Serial::read_two(){
//     //cout << "read : "<< std::flush;
//     // read(fd, buf, sizeof(buf));
//     ssize_t len;
//     // char buff_2[255] = "";
//     int num_1[3];
//     int num_2[3];
//     int num_3[3];
//     int num_4[3];
//     int* ip = NULL;
//     // int* ip[5];
//     // int size = 0;
//     ip = (int*)malloc(sizeof(int) * 5);
//     // char wptr[476];
//     //wptr = (char*)malloc(len);
//     //sprintf(wptr,"%s",buff);
//     time_t t_st = time(NULL);
//     // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
//     // lg << "a " << '\t' << "a" << '\t' << "a" << '\t' << "a" << '\t' << "a" << std::endl;
//     while (1){
//         time_t t_en = time(NULL);
//         if ((t_en - t_st) > 5){
//         goto OUT;  
//         }
//         // one_more_time:
//         tcflush(fd,TCIFLUSH);
//         int ch_1;
//         int ch_2;
//         int ch_3;
//         int ch_4;
//         int len_2 = 0;
//         char buff[1000] = "";
//         char buff_2[1000] = "";
//         len = read(fd, buff, sizeof(buff));
//         // std::cout << len << std::endl;
//         // if (false)
//         // if (len < 0) break;
//         // char buff_2[100] = "";
//            // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
//         for(int i = 0;i < len;i++){
//             char c(buff[i]);
//             // std::cout << "size" << size <<std::endl;
//             // if(c == '$'){
//                 // for(int j = i + 1;j < len;j++ ){
//                     // char c(buff[j]);
//                     if ((ch_1 == 2)&&(ch_2 == 2)&&(ch_3 == 2)&&(ch_4 == 2)){
//                         // int diff_time = num_1[2]-num_1[1];
//                         // if(diff_time == 0){
//                         //     diff_time = 1;
//                         // }
//                         ip[0] = num_2[1];//r
//                         ip[1] = num_3[1];//th
//                         ip[2] = 0;//(num_2[2]-num_2[1])/diff_time;//d_r
//                         ip[3] = num_4[1];//d_th
//                         ip[4] = num_1[1];//time
//                         // std::cout << ip[0] << ip[1] << std::endl;
//                         // lg << ip[0] << ip[1] << std::endl;
//                         // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
//                         lg << ip[4] << '\t' << ip[0] << '\t' << ip[1] << '\t' << ip[2] << '\t' << ip[3] << std::endl;
//                         // tcflush(fd,TCIFLUSH);
//                         goto OUT;
//                         }else if((c == '!')){   // 区切り        tcflush(fd,TCIFLUSH);
            
//                             num_1[ch_1] = atof(buff_2);//time
//                             // std::cout << num_1[0] << std::endl;
//                             strcpy(buff_2, ""); // 行バッファクリア
//                             ch_1++;
//                             // size = 0;
//                         }else if((c == '"')){
//                             num_2[ch_2] = atof(buff_2);//r
//                             // std::cout << num_2[ch_2] << std::endl;
//                             // printf( "Position%d\n", num_2);
//                             strcpy(buff_2, ""); // 行バッファクリア
//                             ch_2++;
//                             // size = 0;
//                         }else if(c == '#'){
//                             num_3[ch_3] = atof(buff_2);//th
//                             // std::cout << num_3[ch_3] << std::endl;
//                             // printf( "Angle%d\n", num_3);
//                             strcpy(buff_2, ""); // 行バッファクリア
//                             ch_3++;
//                             // size = 0;
//                         }else if (c == '$'){
//                             num_4[ch_4] = atof(buff_2);//d_th
//                             // std::cout << num_3[ch_3] << std::endl;
//                             // printf( "Angle%d\n", num_3);
//                             strcpy(buff_2, ""); // 行バッファクリア
//                             ch_4++; 
//                         //     num_1[1] = atoi(buff_2);
//                         //     std::cout << num_1[1] << std::endl;
//                         //     strcpy(buff_2, "");  // 行バッファクリア
//                         //     ch_1 = 2;
//                         // }else if((c == '\t')&&(ch_2 == 2)){
//                         //     num_2[1] = atoi(buff_2);
//                         //     // std::cout << num_2[ch_2] << std::endl;
//                         //     // printf( "Position%d\n", num_2);
//                         //     strcpy(buff_2, "");  // 行バッファクリア
//                         // }else if((c == '\n')&&(ch_3 == 2)){
//                         //     num_3[1] = atoi(buff_2);
//                         //     // std::cout << num_3[ch_3] << std::endl;
//                         //     // printf( "Angle%d\n", num_3);
//                         //     strcpy(buff_2, "");  // 行バッファクリア
//                         }else{
//                             len_2 = strlen(buff_2);
//                             buff_2[len_2] = c;
//                             //std::cout << "len_2" << len_2 << std::endl;  
//                             buff_2[len_2+1] = '\0';
//                             // size++;
//                     }
//                 // }
//                 // goto one_more_time;
//             // }   //     if ((ch_1 == 1)&&(ch_2 == 1)&&(ch_3 == 1)){
//                 // ip[0] = num_2[1];
//                 // ip[1] = num_3[1];
//                 // ip[2] = (num_2[1]-num_2[0])/(num_1[1]-num_1[0]);
//                 // ip[3] = (num_3[1]-num_3[0])/(num_1[1]-num_1[0]);
//                 // ip[4] = num_1[1];
//                 // // std::cout << ip[0] << ip[1] << std::endl;
//                 // // lg << ip[0] << ip[1] << std::endl;
//                 // std::cout << "time_1 " << num_1[0] << "time_2 " << num_1[1] <<std::endl;
//                 // // lg << ip[0] << ip[1] << ip[2] << ip[3] << ip[4] << std::endl;
//                 // break;
//         }
//     }
// OUT:
//     return ip;
// }

int* Serial::read_pre(){
    //cout << "read : "<< std::flush;
    // read(fd, buf, sizeof(buf));
    ssize_t len_1_1;
    ssize_t len_2_1;
    // char buff_2[255] = "";
    int num_1[3];
    int num_2[3];
    int num_3[3];
    int* ip = NULL;
    // int size = 0;
    ip = (int*)malloc(sizeof(int) * 5);
    // char wptr[476];
    //wptr = (char*)malloc(len);
    //sprintf(wptr,"%s",buff);
    time_t t_st = time(NULL);
    // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
    // lg << "a " << '\t' << "a" << '\t' << "a" << '\t' << "a" << '\t' << "a" << std::endl;
    while (1){
        time_t t_en = time(NULL);
        // if ((t_en - t_st) > 5){
        // goto OUT;  
        // }
        tcflush(fd,TCIFLUSH);
        int ch_1 = 0;
        int ch_2 = 0;
        int ch_3 = 0;
        char buff_1_1[1000] = "";
        char buff_2_1[1000] = "";
        len_1_1 = read(fd, buff_1_1, sizeof(buff_1_1));
        len_2_1 = read(fd, buff_2_1, sizeof(buff_2_1));
        // std::cout << len << std::endl;
        // if (false)
        // if (len < 0) break;
        char buff_1_2[1000] = "";
        char buff_2_2[1000] = "";
        int len_1_2 = 0;
        int len_2_2 = 0;
        // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
        for(int i = 0;i < len_1_1;i++){
            char c(buff_1_1[i]);
            // std::cout << "size" << size <<std::endl;
            if ((ch_1 == 2)&&(ch_2 == 2)&&(ch_3 == 2)){
            int diff_time = num_1[2]-num_1[1];
            if(diff_time == 0){
                diff_time = 1;
            }
            ip[0] = num_2[2];
            ip[1] = num_3[2];
            ip[2] = (num_2[2]-num_2[1])/diff_time;
            ip[3] = (num_3[2]-num_3[1])/diff_time;
            ip[4] = num_1[2];
            // std::cout << ip[0] << ip[1] << std::endl;
            // lg << ip[0] << ip[1] << std::endl;
            // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
            lg << ip[4] << '\t' << ip[0] << '\t' << ip[2] << '\t' << ip[1] << '\t' << ip[3] << std::endl;
            // tcflush(fd,TCIFLUSH);
            goto OUT;
            }else if((c == '!')){   // 区切り        tcflush(fd,TCIFLUSH);
 
                num_1[ch_1] = atof(buff_1_2);
                // std::cout << num_1[0] << std::endl;
                strcpy(buff_1_2, ""); // 行バッファクリア
                ch_1++;
                // size = 0;
            }else if((c == '"')){
                num_2[ch_2] = atof(buff_1_2);
                // std::cout << num_2[ch_2] << std::endl;
                // printf( "Position%d\n", num_2);
                strcpy(buff_1_2, ""); // 行バッファクリア
                ch_2++;
                // size = 0;
            }else if(c == '#'){
                num_3[ch_3] = atof(buff_1_2);
                // std::cout << num_3[ch_3] << std::endl;
                // printf( "Angle%d\n", num_3);
                strcpy(buff_1_2, ""); // 行バッファクリア
                ch_3++;
            }else{
                len_1_2 = strlen(buff_1_2);
                buff_1_2[len_1_2] = c;
                //std::cout << "len_2" << len_2 << std::endl;  
                buff_1_2[len_1_2+1] = '\0';
                // size++;
            }
        }
    }
OUT:
    return ip;
}

int* Serial::read_three(){
    //cout << "read : "<< std::flush;
    // read(fd, buf, sizeof(buf));
    ssize_t len;
    // char buff_2[255] = "";
    int num_1[3];
    int num_2[3];
    int num_3[3];
    int* ip = NULL;
    // int size = 0;
    ip = (int*)malloc(sizeof(int) * 5);
    // char wptr[476];
    //wptr = (char*)malloc(len);
    //sprintf(wptr,"%s",buff);
    time_t t_st = time(NULL);
    lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
    while (1){
        time_t t_en = time(NULL);
        if ((t_en - t_st) > 5){
        goto OUT;  
        }
        tcflush(fd,TCIFLUSH);
        int ch_1 = 0;
        int ch_2 = 0;
        int ch_3 = 0;
        char buff[1000] = "";
        len = read(fd, buff, sizeof(buff));
        // std::cout << len << std::endl;
        // if (false)
        // if (len < 0) break;
        char buff_2[1000] = "";
        int len_2 = 0;
        // lg << "time " << '\t' << "r" << '\t' << "d_r" << '\t' << "theta" << '\t' << "d_theta" << std::endl;
        for(int i = 0;i < len;i++){
            char c(buff[i]);
            // std::cout << "size" << size <<std::endl;
            if ((ch_1 == 3)&&(ch_2 == 3)&&(ch_3 == 3)){
            int diff_time = num_1[2]-num_1[1];
            if(diff_time == 0){
                diff_time = 1;
            }
            ip[0] = num_2[2];
            ip[1] = num_3[2];
            ip[2] = (num_2[2]-num_2[1])/diff_time;
            ip[3] = (num_3[2]-num_3[1])/diff_time;
            ip[4] = num_1[2];
            // std::cout << ip[0] << ip[1] << std::endl;
            // lg << ip[0] << ip[1] << std::endl;
            // std::cout << "r " << ip[0] << "th " << ip[1] << "d_r " << ip[2] << "d_th " << ip[3] << "time " << ip[4] <<std::endl;
            // lg << ip[4] << '\t' << ip[0] << '\t' << ip[2] << '\t' << ip[1] << '\t' << ip[3] << std::endl;
            // tcflush(fd,TCIFLUSH);
            goto OUT;
            }else if((c == '!')){   // 区切り        tcflush(fd,TCIFLUSH);
 
                num_1[ch_1] = atof(buff_2);
                // std::cout << num_1[0] << std::endl;
                strcpy(buff_2, ""); // 行バッファクリア
                ch_1++;
                // size = 0;
            }else if((c == '"')){
                num_2[ch_2] = atof(buff_2);
                // std::cout << num_2[ch_2] << std::endl;
                // printf( "Position%d\n", num_2);
                strcpy(buff_2, ""); // 行バッファクリア
                ch_2++;
                // size = 0;
            }else if(c == '#'){
                num_3[ch_3] = atof(buff_2);
                // std::cout << num_3[ch_3] << std::endl;
                // printf( "Angle%d\n", num_3);
                strcpy(buff_2, ""); // 行バッファクリア
                ch_3++;
                // size = 0;
            // }else if ((c == '\b')&&(ch_1 == 1)){
            //     num_1[1] = atoi(buff_2);
            //     std::cout << num_1[1] << std::endl;
            //     strcpy(buff_2, "");  // 行バッファクリア
            //     ch_1 = 2;
            // }else if((c == '\t')&&(ch_2 == 2)){
            //     num_2[1] = atoi(buff_2);
            //     // std::cout << num_2[ch_2] << std::endl;
            //     // printf( "Position%d\n", num_2);
            //     strcpy(buff_2, "");  // 行バッファクリア
            // }else if((c == '\n')&&(ch_3 == 2)){
            //     num_3[1] = atoi(buff_2);
            //     // std::cout << num_3[ch_3] << std::endl;
            //     // printf( "Angle%d\n", num_3);
            //     strcpy(buff_2, "");  // 行バッファクリア
            }else{
                len_2 = strlen(buff_2);
                buff_2[len_2] = c;
                //std::cout << "len_2" << len_2 << std::endl;  
                buff_2[len_2+1] = '\0';
                // size++;
            }
                //     if ((ch_1 == 1)&&(ch_2 == 1)&&(ch_3 == 1)){
                // ip[0] = num_2[1];
                // ip[1] = num_3[1];
                // ip[2] = (num_2[1]-num_2[0])/(num_1[1]-num_1[0]);
                // ip[3] = (num_3[1]-num_3[0])/(num_1[1]-num_1[0]);
                // ip[4] = num_1[1];
                // // std::cout << ip[0] << ip[1] << std::endl;
                // // lg << ip[0] << ip[1] << std::endl;
                // std::cout << "time_1 " << num_1[0] << "time_2 " << num_1[1] <<std::endl;
                // // lg << ip[0] << ip[1] << ip[2] << ip[3] << ip[4] << std::endl;
                // break;
        }
    }
OUT:
    return ip;
}
 
int Serial::write_s(){
   ssize_t num = 0;
   int buff;
   ssize_t cnt = sizeof(buff);
   std::cout << "input [s]key " <<std::endl;
   while(1){
       std::cout << std::flush;
       int buff = getchar();
       std::cout << buff << std::endl;
       num = write(fd, &buff, 1);
       // std::cout << buff <<std::endl;
       if(num < 0){
           std::cout << "error: write" << std::endl;
           return(-1);
       }
       if(buff == 115){
           std::cout << "start" << std::endl;
           return 0;
       }
   }
}
 
void Serial::write_one(int r,int f){
    ssize_t err = 0;
    char buff[32];//char buff[16];
    // int time = 1000;//get_time();
    // int time_rel = 2000;
    // ssize_t cnt = sizeof(buff);
    // buff = getchar();
    int size = sprintf(buff,"%d\t%d\n",r,f);
    err = write(fd,buff,size);
    if(err > 0){
        lg << "release_time finish_time" << std::endl;
        for(int i = 0;i < err;i++){
        // std::cout << buff[i] << std::flush;
        lg << buff[i] << std::flush;
        }
        std::cout << endl;
    }
    //tcflush(fd,TCOFLUSH);
   // std::cout <<"write : "<< buff << std::endl;
   // err = write(fd, &buff, 1);
   // if(err < 0){
   //     // std::cout << "error: write" << std::endl;
   //     return(-1);
   // }
 
}
 
// int Serial::write_ini(int i){
//     ssize_t err = 0;
//     char buff[16];
//     // int time_rel = 2000;
//     // ssize_t cnt = sizeof(buff);
//     // buff = getchar();
//     int size = sprintf(buff,"%d\n",i);
//     err = write(fd,buff,size);
//     std::cout <<"write : "<< buff << std::endl;
//     // err = write(fd, &buff, 1);
//     if(err < 0){
//         std::cout << "error: write" << std::endl;
//         return(-1);
//     }
// }
 
int Serial::close_s(){
   close(fd);
   return 0;
}
 
 
class FIFOBuf {
 private:
   char array[SIZE][7];
   int front = 0, data_num = 0;
 
 public:
   int push(char *data)
   {
     if (data_num == SIZE) //full
       return false;
 
     memcpy(array[(front + data_num) % SIZE], data, 4);
     data_num++;
 
     return true;
   }
 
   int pop(char *data)
   {
     if (data_num == 0) //empty
       return false;
 
     memcpy(data, array[front], 4);
     data_num--;
     front = (front + 1) % SIZE;
 
     return true;
   }
};
 
//  int main(){
//     std::string file_name;
//     auto now = std::chrono::system_clock::now();
//     auto now_c = std::chrono::system_clock::to_time_t(now);
//     std::stringstream ss;
//     ss << std::put_time(localtime(&now_c), "%Y%m%d_%H%M%S");
//     file_name = FILE_DIR + ss.str() + ".csv";
//     std::cout << file_name << std::endl;
//     Serial arduino;
//     lg.open(file_name, std::ios::trunc);
//     arduino.init();
//     while(1){
//         //arduino.write_one();
//         double time_1 = 1000;
//         double time_2 = 2000;
//         arduino.write_one(time_1,time_2);
//         arduino.read_one();
//         // arduino.read_two();
//         //arduino.write_ini();
//         // arduino.write_one();
//         // usleep(10000);
//         //arduino.read_two();
//         // arduino.read_one();
//         //sleep(5);
//     }
//     arduino.close_s();                              // デバイスのクローズ
//     return 0;
// }
 
