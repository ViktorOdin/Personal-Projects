#include "mine.h"

string intToString(int i){
    switch(i){
        case 1:return "\E[36;1m1\E[m"; // 30:black 31:red 32:green 33:yellow 34:blue 35:pink 36:cyan 37:white
        case 2:return "\E[34;1m2\E[m";
        case 3:return "\E[33;1m3\E[m";
        case 4:return "\E[35;1m4\E[m";
        case 5:return "\E[31;1m5\E[m";
        case 6:return "\E[31;1m6\E[m";
        case 7:return "\E[30;1m7\E[m";
        case 8:return "\E[37;1m8\E[m";
        default:return " ";
    }
}

string Mine::toString(){
    if(hyde){
        return "\E[30;1m×\E[m";
    }
    else{
        if(mine==0){
            return " ";
        }
        else if(mine==9){
            return "\E[31;1m*\E[m"; // red mines
        }
        else{
            return intToString(mine);
        }
    }
}

bool Mine::reveal(){
    if(hyde){
        hyde=false;
        return true;
    }
    return false;
}

