#include "piecechess.h"

///
/// \brief piecechess.cpp
/// \file piecechess.cpp
/// \author Oudin Victor
///


using namespace pChess;

string typeInToString(Type t)
{
  switch(t){
  case PAWN : return "p";
  case ROOK : return "r";
  case KNIGHT : return "n";
  case BISHOP : return "b";
  case QUEEN : return "q";
  case KING : return "k";
  default : return ""; // impossible case
  }
}

string PieceChess::toString()
{
  string s=typeInToString(typ);
  if(col==WHITE){
    return "\E[37;1m"+s+"\E[m"; /* \E[37;1m white in console  \E[m */
  }
  else if(col==BLACK){
    return "\E[30;1m"+s+"\E[m"; /* \E[30;1m black in console  \E[m */
  }
  else{
    return " ";
  }
}
