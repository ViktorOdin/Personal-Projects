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

public class ClientConfig{
	
	private String adresse;
	private String pseudo;
	private int port;
	
	/*** Informations d'un client ***/
	public ClientConfig(String adresse,String pseudo,int port){
		this.adresse=adresse;
		this.pseudo=pseudo;
		this.port=port;
	}
	
	public String getAdresse(){
		return adresse;
	}

	public String getPseudo(){
		return pseudo;
	}

	public int getPort(){
		return port;
	}
	
	public void setAdr(String adresse){
		this.adresse = adresse;
	}
	
	public void setPseudo(String pseudo){
		this.pseudo = pseudo;
	}
	
	public void setPort(int port){
		this.port = port;
	}
}
