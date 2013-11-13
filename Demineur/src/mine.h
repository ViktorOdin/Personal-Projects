#ifndef MINE_HPP_
#define MINE_HPP_

#include <iostream>

using namespace std;

class Mine{
private:
    int mine;
    bool hyde;

public:
    Mine(int m=0, bool h=true){ mine=m; hyde=h; }
    string toString();
    int getMine(){ return mine; }
    bool isHyde(){ return hyde; }
    bool reveal();
    bool isMine(){ return mine==9; }
};


#endif

