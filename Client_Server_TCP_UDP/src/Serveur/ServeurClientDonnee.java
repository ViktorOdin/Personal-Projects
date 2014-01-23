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

import java.net.Socket;

public class ServeurClientDonnee {
	
	private String adresse;
	private String pseudo;
	private int port;
	private Socket socket;
	
	/*** donnée d'un client ***/
	public ServeurClientDonnee(String adr,String pseudo,int port,Socket socket){
		this.adresse=adr;
		this.pseudo=pseudo;
		this.port=port;
		this.socket=socket;
	}
	
	public String getAdresse() {
		return adresse;
	}
	
	public String getPseudo() {
		return pseudo;
	}
	
	public int getPort() {
		return port;
	}
	
	public Socket getSocket() {
		return socket;
	}
	
	public void setAdresse(String adr) {
		this.adresse = adr;
	}
	
	public void setPseudo(String pseudo) {
		this.pseudo = pseudo;
	}
	
	public void setPort(int port) {
		this.port = port;
	}
	
	public void setSocket(Socket socket) {
		this.socket = socket;
	}
}
