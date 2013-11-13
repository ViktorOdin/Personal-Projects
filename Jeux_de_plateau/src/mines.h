#ifndef MINES_HPP_
#define MINES_HPP_

#include <iostream>

using namespace std;
///
/// \brief mines.h
/// \file mines.h
/// \author Oudin Victor
///


class Mines
{
private:
  int mine;
  bool hyde;

public:
  Mines(int m=0, bool h=true){ mine=m; hyde=h; }
  string toString();

  /**
   * \fn int Mines::getMine ()
   * \brief Fonction renvoyant l'entier "mine" de l'objet courant.
   *
   * \return "mine" de l'objet courant.
   */
  int getMine(){ return mine; }
  bool isHyde(){ return hyde; }
  bool reveal();
  bool isMine(){ return mine==9; }
};


#endif

