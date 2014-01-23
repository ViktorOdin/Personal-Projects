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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;

public class ServeurClientConnecte extends Thread{
	
	private Socket connection;
	private BufferedReader input;
	private PrintWriter output;
	
	private ArrayList<ServeurClientDonnee> listeClient;
	private ArrayList<ServeurClientConnecte> listeClientConnecte;
	private ServeurClientDonnee client;

	/*** connexion avec les clients ***/
	public ServeurClientConnecte(Socket connection,
			ArrayList<ServeurClientDonnee> liste1,
			ArrayList<ServeurClientConnecte> liste2) {
		this.connection=connection;
		this.listeClient=liste1;
		this.listeClientConnecte=liste2;
		try{
			input =new BufferedReader(new InputStreamReader(connection.getInputStream()));
			output=new PrintWriter(connection.getOutputStream());
		}catch(IOException e){
			System.out.println("Input Output failed : "+e);
			e.printStackTrace();
		}
	}
	
	public void writeMessage(String message){
		output.write(message);
		output.flush();
	}
	
	public boolean verification(String[] mes){
		for(int i=0 ;i<listeClient.size() ;i++ ){
			if(listeClient.get(i).getPort()==Integer.valueOf(mes[2])
					|| listeClient.get(i).getPseudo().length()!=6
					|| listeClient.get(i).getPseudo().equalsIgnoreCase(mes[1])){
				return true;
			}
		}
		return false;
	}
	
	public void listeClient(){
		String messageListe="LISTE";
		for(int i=0 ;i<listeClient.size()-1 ;i++ ){
			messageListe=messageListe+":"+listeClient.get(i).getPseudo()+
					":"+listeClient.get(i).getAdresse()+
					":"+listeClient.get(i).getPort();
		}
		messageListe=messageListe+"\n";
		output.write(messageListe);
		output.flush();
	}
	
	public void notification(){
		for(ServeurClientConnecte clientConnecte:listeClientConnecte)
			if(!clientConnecte.equals(this))
				clientConnecte.writeMessage(
						"NEW:"+listeClient.get(listeClient.size()-1).getPseudo()+
						":"+listeClient.get(listeClient.size()-1).getAdresse()+
						":"+listeClient.get(listeClient.size()-1).getPort()+"\n");
		
	}
	
	public void suppresion(){
		if(client!=null){
			for(ServeurClientConnecte clientConnecte:listeClientConnecte)
				if(!clientConnecte.equals(this))
					clientConnecte.writeMessage("QUIT:"+client.getPseudo()+"\n");

			System.out.println("deconnexion : "+client.getPseudo());
			listeClient.remove(client);
		}
		listeClientConnecte.remove(this);
	}
	
	public void run() {
		String message=null;
		boolean boucle=true;
		while(boucle){
			try{
				message=input.readLine();
			}catch(IOException e){
				System.out.println("ReadLine failed : "+e);
				e.printStackTrace();
			}
			System.out.println(message);
			if(message!=null){
				String[] mes=message.split(":");
				if(mes[0].equalsIgnoreCase("CONNECT")){
					if(verification(mes)){
						output.write("ACKFALSE");
						output.flush();
					}else{
						client= new ServeurClientDonnee(connection.getInetAddress().getHostAddress(),
								mes[1],Integer.valueOf(mes[2]), connection );
						listeClient.add(client);
						output.write("ACK_CONNECT\n");
						output.flush();
						listeClient();
						notification();
					}
				}else if(mes[0].equalsIgnoreCase("ACKFALSE")){
					listeClient();
				}else if(mes[0].equalsIgnoreCase("DECONNECT")){
					output.write("ACK_DECONNECT\n");
					output.flush();
					boucle=false;
				}
			}else 
				boucle=false;
		}
		suppresion();
		output.close();
		try{
			input.close();
			connection.close();
		}catch(IOException e){
			System.out.println("Input Output close failed : "+e);
			e.printStackTrace();
		}
	}
}
