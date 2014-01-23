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

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;

public class RecoiUDP implements Runnable{
	
	private ArrayList<ClientGroupe> groupconfig;
	private ArrayList<ClientConfig> clientconfig;
	private ArrayList<Acquitement> confirmation;
	private ClientConfig client;
	private DatagramSocket socketUDP;

	/*** reception de message par UDP ***/
	public RecoiUDP(int port, ArrayList<ClientGroupe> groupclient, ArrayList<ClientConfig> clientconfig,
			ArrayList<Acquitement> confirmation,ClientConfig client){
		this.client=client;
		this.confirmation=confirmation;
		this.clientconfig=clientconfig;
		this.groupconfig=groupclient;
		
		try{
			socketUDP = new DatagramSocket(port);
		}catch(SocketException e){
			System.out.println("Datagram Socket failed : "+e);
			e.printStackTrace();
		}
	}
	
	
	public void interrupt(){
		this.socketUDP.close();
	}
	
	
	public void run(){	
		while(true){
			byte[] buffer = new byte[500];
			DatagramPacket p = new DatagramPacket(buffer, buffer.length);
			try{
				socketUDP.receive(p);
			}catch(IOException e){
                System.out.println("Datagram Socket receive failed : "+e);
				e.printStackTrace();
			}
			
			String chaine = new String(p.getData());
			chaine=chaine.trim();				
			String str[]=chaine.split(":");
			System.out.println("RECOIS par UDP: "+chaine);
			if(str[0].equalsIgnoreCase("CHAT")){
				/** CHAT **/
				if(str.length!=3)
					System.out.println("mauvais message CHAT");
				else
					System.out.println(str[1]+" :"+str[2]);
			}
			else if(str[0].equalsIgnoreCase("CHATGROUPE")){
				/** CHATGROUPE **/
				if(str.length!=4)
					System.out.println("mauvais message CHATGROUPE");
				else
					System.out.println("groupe "+str[1]+":"+str[2]+":"+str[3]);
			}
			else if(str[0].equalsIgnoreCase("RENTRERGROUPE")){
				/** RENTRERGROUPE **/
				if(str.length!=3)
					System.out.println("mauvais message RENTRERGROUPE");
				boolean existe=false;
				for(ClientGroupe nom:groupconfig){
					if(str[1].equalsIgnoreCase(nom.getNom())){
						existe=true;

						for(ClientConfig c:clientconfig)
							if(c.getPseudo().equalsIgnoreCase(str[2])){
								nom.getList().add(c);
							}

						break;
					}
				}
				if(!existe){

					groupconfig.add(new ClientGroupe(str[1]));
					for(ClientConfig c:clientconfig)
						if(c.getPseudo().equalsIgnoreCase(str[2])){
							groupconfig.get(groupconfig.size()-1).getList().add(c);
						}
					System.out.println("Groupe"+groupconfig.size());

				}
			}
			else if(str[0].equalsIgnoreCase("REPONSEGROUPE")){
				/** REPONSEGROUPE **/
				if(str.length!=4)
					System.out.println("mauvais message REPONSEGROUPE");
				for(Acquitement c:confirmation){
					if(c.getNom().equalsIgnoreCase(str[1]))
						if(str[3].equalsIgnoreCase("ACCEPT")){
							c.increment();
							if(c.getDebut()==c.getMax()){
								for(ClientGroupe gr:groupconfig){
									if(gr.getNom().equals(str[1])){
										gr.getList().add(client);
										System.out.println("ajout");
									}
								}
								for(ClientConfig cli:clientconfig)
									if(!cli.equals(client))
										new Thread(new EnvoiUDP("RENTRERGROUPE:"+str[1]+":"+client.getPseudo()
												,cli.getPort(),cli.getAdresse())).start();
							}
						}
						else
							confirmation.remove(c);

				}
			}
			else if(str[0].equalsIgnoreCase("DEMANDEGROUPE")){
				/** DEMANDEGROUPE **/
				if(str.length!=3)
					System.out.println("mauvais message DEMANDEGROUPE");
				for(ClientConfig c:clientconfig)
					if(c.getPseudo().equalsIgnoreCase(str[1])){
						for(ClientGroupe gr: groupconfig){
							for(ClientConfig cli:gr.getList()){
								new Thread(new EnvoiUDP("RENTRERGROUPE:"+gr.getNom()+":"+cli.getPseudo()
										,c.getPort(),c.getAdresse())).start();
							}
						} 
					}
			}
			else if(str[0].equalsIgnoreCase("QUITTERGROUPE")){
				/** QUITTERGROUPE **/
				if(str.length!=3)
					System.out.println("mauvais message QUITTERGROUPE");
				for(ClientGroupe gr: groupconfig)
					if(gr.getNom().equalsIgnoreCase(str[1])){
						for(int i=0; i<gr.getList().size(); i++){
							if(gr.getList().get(i).getPseudo().equalsIgnoreCase(str[2]))
								gr.getList().remove(i);
						} 
					}
			}
			else{
				/** ERREUR PROTOCOLE **/
				System.out.println("ERREUR PROTOCOLE");
			}
		}
	}
}
