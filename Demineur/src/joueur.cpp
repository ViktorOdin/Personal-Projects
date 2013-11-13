#include "joueur.h"
#include <limits>

Joueur::Joueur(){
    cout << "Nouveau Joueur, entrez son nom: ";
    while (!( cin >> name )){
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
    }
    points=0;
    cout << name << " entre dans le jeu, bonne chance" << endl;
}
