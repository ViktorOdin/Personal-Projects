/**
* TP n°:1
*
* Titre du TP :TCP/UDP
*
*
* Noms :Guillet, Oudin
*
* Prenoms: Antony, Victor
*
* email :antonyguillet@hotmail.fr, victor.oudin@etu.univ-paris-diderot.fr
*
*/ 

package Client;

public class Acquitement{
	
	private String nom;
	private int debut;
	private int max;
	
	/*** Acquitement après demande d'entré dans un groupe ***/
	public Acquitement(String nom,int max){
		this.nom=nom;
		this.max=max;
	}
	
	public String getNom(){
		return nom;
	}
	
	public int getDebut(){
		return debut;
	}
	
	public int getMax(){
		return max;
	}
	
	public void increment(){
		debut++;
	}
	
}
