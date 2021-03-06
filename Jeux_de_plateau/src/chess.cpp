#include "chess.h"

///
/// \brief chess.cpp
/// \file chess.cpp
/// \author Oudin Victor
///

Chess::Chess()
{
  PieceChess pi;
  /*for(int i=0; i<8; i++){
    pi=PieceChess(WHITE,PAWN);
    pla[1][i]=pi;
    pi=PieceChess(BLACK,PAWN);
    pla[6][i]=pi;
    }*/
  pi=PieceChess(WHITE,ROOK);
  pla[0][0]=pi;
  pi=PieceChess(WHITE,KNIGHT);
  pla[0][1]=pi;
  pi=PieceChess(WHITE,BISHOP);
  pla[0][2]=pi;
  pi=PieceChess(WHITE,QUEEN);
  pla[0][3]=pi;
  pi=PieceChess(WHITE,KING);
  pla[0][4]=pi;
  posWhiteKing[0]=0;
  posWhiteKing[1]=4;
  pi=PieceChess(WHITE,BISHOP);
  pla[0][5]=pi;
  pi=PieceChess(WHITE,KNIGHT);
  pla[0][6]=pi;
  pi=PieceChess(WHITE,ROOK);
  pla[0][7]=pi;

  /*pi=PieceChess(BLACK,ROOK);
  pla[7][0]=pi;
  pi=PieceChess(BLACK,KNIGHT);
  pla[7][1]=pi;
  pi=PieceChess(BLACK,BISHOP);
  pla[7][2]=pi;
  pi=PieceChess(BLACK,QUEEN);
  pla[7][3]=pi;*/
  pi=PieceChess(BLACK,KING);
  pla[7][4]=pi;
  posBlackKing[0]=7;
  posBlackKing[1]=4;
  /*pi=PieceChess(BLACK,BISHOP);
  pla[7][5]=pi;
  pi=PieceChess(BLACK,KNIGHT);
  pla[7][6]=pi;
  pi=PieceChess(BLACK,ROOK);
  pla[7][7]=pi;*/


}

void Chess::print()
{
  cout << "   a  b  c  d  e  f  g  h" << endl;
  for(int i=8;i>0;i--){
    cout << i << " ";
    for(int j=0;j<8;j++){
      cout << "[" << pla[i-1][j].toString() << "]";
    }
    cout << " \n";
  }
}

bool Chess::isEmpty(int x, int y)
{
  if(pla[x][y].getColor()==NUL and x>=0 and x<=7 and y>=0 and y<=7){
    return true;
  }
  return false;
}

Color Chess::getColor(int x, int y)
{
  if(!isEmpty(x,y)){
    return pla[x][y].getColor();
  }
  return NUL;
}

bool Chess::canMovePawn(int x1, int y1, int x2, int y2)
{
  if( pla[x1][y1].getColor()==WHITE ){
    if(y2==y1 and (x2==x1+1 or (x2==x1+2 and x1==1)) and isEmpty(x2,y2))
      {return true;}
    else if((y2==y1+1 or y2==y1-1) and x2==x1+1 and !isEmpty(x2,y2) and pla[x2][y2].getColor()==BLACK)
      {return true;}
    else {return false;}
  }
  if( pla[x1][y1].getColor()==BLACK ){
    if(y2==y1 and (x2==x1-1 or (x2==x1-2 and x1==6)) and isEmpty(x2,y2))
      {return true;}
    else if((y2==y1+1 or y2==y1-1) and x2==x1-1 and !isEmpty(x2,y2) and pla[x2][y2].getColor()==WHITE)
      {return true;}
    else {return false;}
  }
  return false;
}

bool Chess::canMoveRook(int x1, int y1, int x2, int y2)
{
  if(isEmpty(x2,y2) or (!isEmpty(x2,y2) and pla[x1][y1].getColor()!=pla[x2][y2].getColor())){
    if(x1==x2 and y1!=y2){
      if(y1-y2<0){
	for(int i=y1+1; i<y2; i++){
	  if(!isEmpty(x1,i)) {return false;}
	}
	return true;
      }
      else{
	for(int i=y1-1; i>y2; i--){
	  if(!isEmpty(x1,i)) {return false;}
	}
	return true;
      }
    }
    else if(x1!=x2 and y1==y2){
      if(x1-x2<0){
	for(int i=x1+1; i<x2; i++){
	  if(!isEmpty(i,y1)) {return false;}
	}
	return true;
      }
      else{
	for(int i=x1-1; i>x2; i--){
	  if(!isEmpty(i,y1)) {return false;}
	}
	return true;
      }
    }
    return false;
  }
  return false;
}

bool Chess::canMoveKnight(int x1, int y1, int x2, int y2)
{
  if(isEmpty(x2,y2) or (!isEmpty(x2,y2) and pla[x1][y1].getColor()!=pla[x2][y2].getColor())){
    if(((x1-x2==-1 or x1-x2==1) and (y1-y2==-2 or y1-y2==2)) or ((x1-x2==-2 or x1-x2==2) and (y1-y2==-1 or y1-y2==1)))
      {return true;}
    else {return false;}
  }
  return false;
}

bool Chess::canMoveBishop(int x1, int y1, int x2, int y2)
{
  if(isEmpty(x2,y2) or (!isEmpty(x2,y2) and pla[x1][y1].getColor()!=pla[x2][y2].getColor())){
    if(x1<x2){
      if(y1<y2 and x1-y1==x2-y2){
	for(int i=1; i<x2-x1; i++){
	  if(!isEmpty(x1+i,y1+i)) {return false;}
	}
	return true;
      }
      else if(y1>y2 and x1+y1==x2+y2){
	for(int i=1; i<x2-x1; i++){
	  if(!isEmpty(x1+i,y1-i)) {return false;}
	}
	return true;
      }
      return false;
    }
    else if(x1>x2){
      if(y1<y2 and x1+y1==x2+y2){
	for(int i=1; i<x1-x2; i++){
	  if(!isEmpty(x1-i,y1+i)) {return false;}
	}
	return true;
      }
      else if(y1>y2 and x1-y1==x2-y2){
	for(int i=1; i<x1-x2; i++){
	  if(!isEmpty(x1-i,y1-i)) {return false;}
	}
	return true;
      }
      return false;
    }
    return false;
  }
  return false;
}

bool Chess::canMoveQueen(int x1, int y1, int x2, int y2)
{
  return (canMoveRook(x1,y1,x2,y2) or canMoveBishop(x1,y1,x2,y2));
}

bool Chess::canMoveKing(int x1, int y1, int x2, int y2)
{
  if(isEmpty(x2,y2) or (!isEmpty(x2,y2) and pla[x1][y1].getColor()!=pla[x2][y2].getColor())){
    if(((x1-x2==1 or x1-x2==-1) and (y1-y2==1 or y1-y2==0 or y1-y2==-1)) or (x1-x2==0 and (y1-y2==1 or y1-y2==-1))){
      return true;
    }
    return false;
  }
  return false;
}

bool Chess::canMove(int x1, int y1, int x2, int y2)
{
  if(x1==x2 and y1==y2){ return false; }
  if(!isEmpty(x1,y1) and x1>=0 and x1<=7 and y1>=0 and y1<=7 and x2>=0 and x2<=7 and y2>=0 and y2<=7){
    switch(pla[x1][y1].getType()){
    case PAWN : return canMovePawn(x1,y1,x2,y2);
    case ROOK : return canMoveRook(x1,y1,x2,y2);
    case KNIGHT : return canMoveKnight(x1,y1,x2,y2);
    case BISHOP : return canMoveBishop(x1,y1,x2,y2);
    case QUEEN : return canMoveQueen(x1,y1,x2,y2);
    case KING : return canMoveKing(x1,y1,x2,y2);
    default : return false;
    }
  }
  return false;
}

bool Chess::move(int x1, int y1, int x2, int y2)
{
  PieceChess p1=pla[x1][y1];
  PieceChess p2=pla[x2][y2];
  if(canMove(x1,y1,x2,y2)){
    pla[x2][y2]=pla[x1][y1];
    PieceChess p;
    pla[x1][y1]=p;
    if(pla[x2][y2].getType()==KING){
      if(p1.getColor()==WHITE){
	posWhiteKing[0]=x2;
	posWhiteKing[1]=y2;
      }
      else{
	posBlackKing[0]=x2;
	posBlackKing[1]=y2;
      }
    }
    if(check()==pla[x2][y2].getColor()){
      pla[x1][y1]=p1;
      pla[x2][y2]=p2;
      cout << "protège ton roi, baka" << endl;
      return false;
    }

    return true;
  }
  else{
    cout << "forbidden movment" << endl;
    return false;
  }
}

bool Chess::checkWhite(int x, int y)
{
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(not(i==x and j==y) and pla[i][j].getColor()==BLACK and canMove(i,j,x,y)){
	return true;
      }
    }
  }
  return false;
}

bool Chess::checkBlack(int x, int y)
{
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(not(i==x and j==y) and pla[i][j].getColor()==WHITE and canMove(i,j,x,y)){
	return true;
      }
    }
  }
  return false;
}

Color Chess::check()
{
  if(checkWhite(posWhiteKing[0],posWhiteKing[1])){
    return WHITE;
  }
  if(checkBlack(posBlackKing[0],posBlackKing[1])){
    return BLACK;
  }
  return NUL;
}

bool Chess::canNotMoveWhiteKing()
{
  PieceChess tmp=pla[posWhiteKing[0]][posWhiteKing[1]];
  PieceChess p;
  pla[posWhiteKing[0]][posWhiteKing[1]]=p;
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(posWhiteKing[0]+i>=0 and posWhiteKing[0]+i<=7 and posWhiteKing[1]+j>=0 and posWhiteKing[1]+j<=7){
	if(!checkWhite(posWhiteKing[0]+i,posWhiteKing[1]+j)){
	  pla[posWhiteKing[0]][posWhiteKing[1]]=tmp;
	  return false;
	}
      }
    }
  }
  pla[posWhiteKing[0]][posWhiteKing[1]]=tmp;
  return true;
}

bool Chess::canNotMoveBlackKing()
{
  PieceChess tmp=pla[posBlackKing[0]][posBlackKing[1]];
  PieceChess p;
  pla[posBlackKing[0]][posBlackKing[1]]=p;
  for(int i=-1; i<2; i++){
    for(int j=-1; j<2; j++){
      if(posBlackKing[0]+i>=0 and posBlackKing[0]+i<=7 and posBlackKing[1]+j>=0 and posBlackKing[1]+j<=7){
	if(!checkBlack(posBlackKing[0]+i,posBlackKing[1]+j)){
	  pla[posBlackKing[0]][posBlackKing[1]]=tmp;
	  return false;
	}
      }
    }
  }
  pla[posBlackKing[0]][posBlackKing[1]]=tmp;
  return true;
}

bool Chess::checkmateWhite(int x, int y)
{
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(not(i==x and j==y) and pla[i][j].getColor()==BLACK and canMove(i,j,x,y)){
	if(pla[i][j].getType()==ROOK or pla[i][j].getType()==QUEEN){
	  if(i==x){
	    if(j<y){
	      for(int k=j+1; k<y; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i,k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=j-1; k>y; k--){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i,k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	  if(j==y){
	    if(i<x){
	      for(int k=i+1; k<x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,k,j)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(i>x){
	      for(int k=i-1; k>x; k--){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,k,j)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	}
	if(pla[i][j].getType()==BISHOP or pla[i][j].getType()==QUEEN){
	  if(i<x){
	    if(j<y){
	      for(int k=1; k<x-i; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i+k,j+k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=1; k<x-i; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i+k,j-k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	  if(i>x){
	    if(j<y){
	      for(int k=1; k<i-x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i-k,j+k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=1; k<i-x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==WHITE and canMove(l,m,i-k,j-k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	}
	return true;
      }
    }
  }
  return false;
}

bool Chess::checkmateBlack(int x, int y)
{
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(pla[i][j].getColor()==WHITE and canMove(i,j,x,y)){
	if(pla[i][j].getType()==ROOK or pla[i][j].getType()==QUEEN){
	  if(i==x){
	    if(j<y){
	      for(int k=j+1; k<y; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i,k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=j-1; k>y; k--){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i,k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	  if(j==y){
	    if(i<x){
	      for(int k=i+1; k<x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,k,j)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(i>x){
	      for(int k=i-1; k>x; k--){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,k,j)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	}
	if(pla[i][j].getType()==BISHOP or pla[i][j].getType()==QUEEN){
	  if(i<x){
	    if(j<y){
	      for(int k=1; k<x-i; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i+k,j+k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=1; k<x-i; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i+k,j-k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	  if(i>x){
	    if(j<y){
	      for(int k=1; k<i-x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i-k,j+k)){
			return false;
		      }
		  }
		}
	      }
	    }
	    if(j>y){
	      for(int k=1; k<i-x; k++){
		for(int l=0; l<8; l++){
		  for(int m=0; m<8; m++){
		    if(l!=x and m!=y)
		      if(pla[l][m].getColor()==BLACK and canMove(l,m,i-k,j-k)){
			return false;
		      }
		  }
		}
	      }
	    }
	  }
	}
	return true;
      }
    }
  }
  return false;
}

bool Chess::checkmate()
{
  if(checkmateWhite(posWhiteKing[0],posWhiteKing[1]) and canNotMoveWhiteKing()){
    return true;
  }
  if(checkmateBlack(posBlackKing[0],posBlackKing[1]) and canNotMoveBlackKing()){
    return true;
  }
  return false;
}
