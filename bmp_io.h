#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Bmp_io
{
    struct BGR
    {
        uint8_t B,G,R;
    };
    std::ifstream bmp_in;
    std::ofstream bmp_out;
    std::string file_in,file_out;
    uint8_t head[54]; //nagłówek
    uint8_t szary, ch;
    int i,j; //liczniki
    int width, height, data_start; //szerokość, wysokość, początek danych
    short add, newwidth; //pomocnicze
    BGR * pixmap;

public:
    Bmp_io(const char*);
    Bmp_io(const char*,const char*);
    ~Bmp_io();
    void err(const char *);
    void open_in();
    void open_out();
    void read_in();
    void write_out();
    void in_fo();
    void go();
};
