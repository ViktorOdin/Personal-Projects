#ifndef CHESS_HPP_
#define CHESS_HPP_

#include "piecechess.h"

///
/// \brief chess.h
/// \file chess.h
/// \author Oudin Victor
///

using namespace pChess;

class Chess
{
protected:
  PieceChess pla[8][8];
  int posWhiteKing[2];
  int posBlackKing[2];
  bool canMovePawn(int x1, int y1, int x2, int y2);
  bool canMoveRook(int x1, int y1, int x2, int y2);
  bool canMoveKnight(int x1, int y1, int x2, int y2);
  bool canMoveBishop(int x1, int y1, int x2, int y2);
  bool canMoveQueen(int x1, int y1, int x2, int y2);
  bool canMoveKing(int x1, int y1, int x2, int y2);
  bool checkWhite(int x, int y);
  bool checkBlack(int x, int y);
  bool checkmateWhite(int x, int y);
  bool checkmateBlack(int x, int y);
  bool canNotMoveWhiteKing();
  bool canNotMoveBlackKing();
public:
  Chess ();
  void print();
  bool isEmpty(int x, int y);
  Color getColor(int x, int y);
  bool canMove(int x1, int y1, int x2, int y2);
  bool move(int x1, int y1, int x2, int y2);
  Color check();
  bool checkmate();
};

#endif

