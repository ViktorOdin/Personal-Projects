#include "jouer.h"
#include <limits>


Jouer::Jouer(){
    cout << "Choissisez un nombre de Joueurs (minimum 2) :";
    while (!( cin >> nbrPlayer ) || nbrPlayer<2 ){
        if ( cin.eof() ){
            // ^D Fin du flux d'entree !
            cout << "Fin du flux d'entree !" << endl;
            break;
        }
        else if ( cin.fail() ){
            cout << "Saisie incorrecte, recommencez : ";
            cin.clear();
            cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        }
        else{
            cout << "nombre de joueurs invalide, recommencez: ";
        }
    }
    int i=0;
    while(i<nbrPlayer){
        Joueur j;
        players.push_back(j);
        i++;
    }
}
