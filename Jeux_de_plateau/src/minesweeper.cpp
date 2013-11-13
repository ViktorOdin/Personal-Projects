#include "minesweeper.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

///
/// \brief minesweeper.cpp
/// \file minesweeper.cpp
/// \author Oudin Victor
///

using namespace std;

Minesweeper::Minesweeper(int op)
{
  srand(time(NULL));
  option=op;
  int i,j,m,b;
  switch(option){
  case 1: size=10;m=9;break;
  case 2: size=16;m=39;break;
  case 3: size=24;m=109;break;
  default: size=16;option=2; cout << "Mauvaise choix, medium se sera alors." << endl;
  }
  Mines mi;
  for(i=0; i<size; i++){
    for(j=0; j<size; j++){
      switch(option){
      case 1: small[i][j]=mi;break;
      case 2: medium[i][j]=mi;break;
      case 3: large[i][j]=mi;break;
      }
    }
  }
  for(int k=0; k<m; k++){
    i=1+(int)((double)(rand())/((double)(RAND_MAX)+1)*(double)size);
    j=1+(int)((double)(rand())/((double)(RAND_MAX)+1)*(double)size);
    switch(option){
    case 1: b=small[i][j].getMine();break;
    case 2: b=medium[i][j].getMine();break;
    case 3: b=large[i][j].getMine();break;
    }
    if(b==0){
      mi=Mines(9,true);
      switch(option){
      case 1: small[i][j]=mi;break;
      case 2: medium[i][j]=mi;break;
      case 3: large[i][j]=mi;break;
      }
    }
    else { k--; }
  }
  initialize();
}

void Minesweeper::print()
{
  cout << "    ";
  for(int i=1; i<=size; i++){
    if(i<9){
      cout << i << "  ";
    }
    else{
      cout << i << " ";
    }
  }
  cout << " \n";
  for(int i=size;i>0;i--){
    if(i<10){
      cout << i << "  ";
    }
    else{
      cout << i << " ";
    }
    for(int j=0;j<size;j++){
      switch(option){
      case 1: cout << "[" << small[i-1][j].toString() << "]";break;
      case 2: cout << "[" << medium[i-1][j].toString() << "]";break;
      case 3: cout << "[" << large[i-1][j].toString() << "]";break;
      }
    }
    cout << " \n";
  }
}

void Minesweeper::initialize()
{
  int count;
  Mines m;
  bool m1;
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      switch(option){
      case 1: m1=!small[i][j].isMine();break;
      case 2: m1=!medium[i][j].isMine();break;
      case 3: m1=!large[i][j].isMine();break;
      }
      if(m1){
	count=0;
	for(int k=-1; k<2; k++){
	  for(int l=-1; l<2; l++){
	    if(i+k>=0 and i+k<size and j+l>=0 and j+l<size){
	      switch(option){
	      case 1: if(small[i+k][j+l].isMine()){count++;}break;
	      case 2: if(medium[i+k][j+l].isMine()){count++;}break;
	      case 3: if(large[i+k][j+l].isMine()){count++;}break;
	      }
	    }
	  }
	}
	m=Mines(count,true);
	switch(option){
	case 1: small[i][j]=m;break;
	case 2: medium[i][j]=m;break;
	case 3: large[i][j]=m;break;
	}
      }
    }
  }
}

int Minesweeper::click(int x, int y)
{
  bool h;
  int m;
  if(x>=0 and x<size and y>=0 and y<size){
    switch(option){
    case 1: h=small[x][y].isHyde(); m=small[x][y].getMine();break;
    case 2: h=medium[x][y].isHyde(); m=medium[x][y].getMine();break;
    case 3: h=large[x][y].isHyde(); m=large[x][y].getMine();break;
    }
    if(h){
      switch(option){
      case 1: small[x][y].reveal();break;
      case 2: medium[x][y].reveal();break;
      case 3: large[x][y].reveal();break;
      }
      if(m==9){
	return 1;
      }
      if(m==0){
	for(int i=-1; i<2; i++){
	  for(int j=-1; j<2; j++){
	    click(x+i,y+j);
	  }
	}

      }
      return 0;
    }
    return -1;
  }
  return -1;
}

bool Minesweeper::isFinish()
{
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      switch(option){
      case 1: if(small[i][j].isHyde()){return false;}break;
      case 2: if(medium[i][j].isHyde()){return false;}break;
      case 3: if(large[i][j].isHyde()){return false;}break;
      }
    }
  }
  return true;
}


