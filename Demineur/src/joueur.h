#ifndef JOUEUR_HPP_
#define JOUEUR_HPP_

#include <iostream>

using namespace std;

class Joueur{
private:
    string name;
    int points;
public:
    Joueur();
    string getName(){ return name; }
    int getPoints(){ return points; }

};

#endif
