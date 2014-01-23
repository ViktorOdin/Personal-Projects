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
package Serveur;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

public class Serveur extends Thread{
	
	private ArrayList<ServeurClientDonnee> listeClient; 
	private ServerSocket serveursocket;
	private ArrayList<ServeurClientConnecte> listeClientConnecte;
	private final int port=1234;
	
	/*** Serveur ***/
	public Serveur(){
		System.out.println("Serveur TCP activé");
		setListeClient(new ArrayList<ServeurClientDonnee> ());
		this.listeClientConnecte = new ArrayList<ServeurClientConnecte>();
		try {
			this.serveursocket=new ServerSocket(port);
		} catch (IOException e) {
			System.out.println("Server Socket failed : "+e);
			e.printStackTrace();
		}
	}
	
	public void run(){
		while(true){
			try {
				Socket connection = serveursocket.accept();
				ServeurClientConnecte t = new ServeurClientConnecte(connection,
						this.listeClient,
						this.listeClientConnecte);
				this.listeClientConnecte.add(t);
				System.out.println("nouveau client");

				t.start();
			} catch (IOException e) {
				System.out.println("Server Socket accept failed : "+e);
				e.printStackTrace();
			}
		}
	}
	
	public ArrayList<ServeurClientConnecte> getListeClientConnecte() {
		return this.listeClientConnecte;
	}
	
	public ArrayList<ServeurClientDonnee> getListeClient() {
		return listeClient;
	}
	
	public void setListeClient(ArrayList<ServeurClientDonnee> liste) {
		this.listeClient = liste;
	}

	public static void main(String[] args){
		Serveur s = new Serveur();
		s.start();
	}
}
