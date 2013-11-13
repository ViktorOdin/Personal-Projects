#include <iostream>
#include "demineur.h"
#include "jouer.h"

using namespace std;

int playDemineur(){
    cout << "Quelle taille de grille voulez vous?" << endl;
    cout << "Small:1  Medium:2  Large:3" << endl;
    int op;
    cin >> op;
    Demineur m(op);
    m.print();
    int turn=0;
    int player1=0;
    int player2=0;
    int x,y,point,size=m.getSize();
    bool ok=false;
    while(!m.isFinish()){
        if(turn%2==0){ // player1 turn
            cout << "Joueur 1, c'est ton tour." << endl;
            cout << "choisi une case." << endl;
            ok=false;
            while(!ok){
                cin >> x >> y;
                if(x-1>=0 and x-1<size and y-1>=0 and y-1<size and m.isHyde(x-1,y-1)){
                    ok=true;
                    point=m.click(x-1,y-1);
                    if(point==0){
                        turn++;
                    }
                    else{
                        player1++;
                    }
                    cout << "Joueur 1 : " << player1 << " points" << endl;
                }
                else{
                    cout << "Mais tu vas choisir une case possible, Oui?" << endl;
                }
            }
        }
        else{ // player2 turn
            cout << "Joueur 2, c'est ton tour." << endl;
            cout << "choisi une case." << endl;
            ok=false;
            while(!ok){
                cin >> x >> y;
                if(x-1>=0 and x-1<size and y-1>=0 and y-1<size and m.isHyde(x-1,y-1)){
                    ok=true;
                    point=m.click(x-1,y-1);
                    if(point==0){
                        turn++;
                    }
                    else{
                        player2++;
                    }
                    cout << "Joueur 2 : " << player2 << " points" << endl;
                }
                else{
                    cout << "Mais tu vas choisir une case possible, Oui?" << endl;
                }
            }
        }
        m.print();
    }
    cout << "Partie terminée." << endl;
    if(player1>player2){
        cout << "Joueur 1 gagne avec " << player1 << " à " << player2 << endl;
    }
    else{
        cout << "Joueur 2 gagne avec " << player2 << " à " << player1 << endl;
    }
    return 1;
}

int main()
{
    //playDemineur();
    Jouer j;
    return 0;
}
