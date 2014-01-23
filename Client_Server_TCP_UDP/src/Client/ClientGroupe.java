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

import java.util.ArrayList;

public class ClientGroupe{
	
	private ArrayList<ClientConfig> list;
	private String nom;
	
	/*** groupe de clients ***/
	public ClientGroupe(String nom){
		this.nom=nom;
		list=new ArrayList<ClientConfig>();
	}
	
	public ArrayList<ClientConfig> getList(){
		return list;
	}
	
	public void setList(ArrayList<ClientConfig> list){
		this.list = list;
	}
	
	public String getNom(){
		return nom;
	}
	
	public void setNom(String nom){
		this.nom = nom;
	}
}
