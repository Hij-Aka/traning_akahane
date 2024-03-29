#ifndef _SERIAL_HPP_
#define _SERIAL_HPP_
 
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include<vector>
class Serial{
    private:
        int fd;
        unsigned char buf[255];//buf[255];
        struct termios tio;                 // シリアル通信設定
        int baudRate = B115200;
 
    public:
        int init();
        int log();
        int read_s();
        int write_s();
        void read_one();
        int* read_two();
        // std::vector<int> read_one();
        // std::vector<int> read_two();
        int* read_three();
        int* read_pre();
        void write_one(int r,int f);
        int write_ini(int i);
        int close_s();
        int get_para[4];
};
 
#endif