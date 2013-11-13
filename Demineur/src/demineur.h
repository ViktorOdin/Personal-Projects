#ifndef DEMINEUR_HPP_
#define DEMINEUR_HPP_

#include "mine.h"

class Demineur{
private:
    int option,size,mines;
    Mine small[10][10];
    Mine medium[16][16];
    Mine large[24][24];
    void initialize();

public:
    Demineur(int op=2);
    int getSize(){ return size; }
    void print();
    bool isHyde(int X, int y);
    int click(int x, int y);
    bool isFinish();
};


#endif
