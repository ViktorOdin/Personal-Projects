#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include "mines.h"

///
/// \brief minesweeper.h
/// \file minesweeper.h
/// \author Oudin Victor
///

class Minesweeper
{
private:
  int option,size;
  Mines small[10][10];
  Mines medium[16][16];
  Mines large[24][24];
  void initialize();
  
public:
  Minesweeper(int op=2);
  int getSize(){ return size; }
  void print();
  int click(int x, int y);
  bool isFinish();
};


#endif
