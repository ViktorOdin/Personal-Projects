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
import java.util.Scanner;

public class Terminal implements Runnable{
	
	private Client clientTCP;
	private ClientConfig client;
	private Scanner sn;
	private ArrayList<ClientGroupe> listegroupe;
	private ArrayList<ClientConfig> listeclient;
	private ArrayList<Acquitement> listeconfig;
	
	/*** Terminal du client ***/
	public Terminal(Client clientTCP, ArrayList<ClientGroupe> groupclient,
			ClientConfig client,ArrayList<ClientConfig> listeclient, ArrayList<Acquitement> listeconfig){
		this.client=client;
		this.listeclient=listeclient;
		this.listeconfig=listeconfig;
		sn =new Scanner(System.in);
		this.clientTCP=clientTCP;
		listegroupe = groupclient;
	}
	
	public Scanner getSn(){
		return sn;
	}
	
	public void run(){
		while(!clientTCP.isfin()){
			System.out.println("Pour voir les options tapez 'o'");
						
			String mes = sn.nextLine();
			String[] message=mes.split(":");
			if(message[0].equalsIgnoreCase("lc")==true){   
				if(clientTCP.getList().size()>0)
					for(ClientConfig nom:clientTCP.getList())
						System.out.println("Pseudo: "+nom.getPseudo()+" Port: "+nom.getPort());
				else
					System.out.println("Aucun client connecté");

			}
			else if (message[0].equalsIgnoreCase("lg")==true){
				if(listegroupe.size()>0)
					for(ClientGroupe nom:listegroupe){
						System.out.println("Groupe nom:"+nom.getNom());
						System.out.println("       Membres:");
						for(ClientConfig cli:nom.getList())
							System.out.println("            membre:"+cli.getPseudo());
					}
				else
					System.out.println("Veuillez commencer par créer un groupe!!");
			}
			else if(message[0].equalsIgnoreCase("q")){	
				clientTCP.setfin(true);
				clientTCP.quitter();
				break;
			}
			else if(message[0].equalsIgnoreCase("s")==true){
				String mes_envoie;
				mes_envoie="CHAT:"+client.getPseudo()+":"+message[2];
				for(ClientConfig c:clientTCP.getList()){
					if(c.getPseudo().equalsIgnoreCase(message[1])){
						System.out.println(c.getPort()+" "+c.getAdresse());
						new Thread(new EnvoiUDP(mes_envoie,c.getPort(),c.getAdresse())).start();
						break;
					}
				}


			}
			else if(message[0].equalsIgnoreCase("sg")==true){
				String mes_envoie;
				mes_envoie="CHATGROUPE:"+message[1]+":"+client.getPseudo()+":"+message[2];
				for(ClientGroupe gr:listegroupe){
					if(gr.getNom().equalsIgnoreCase(message[1])){
						for(ClientConfig c:gr.getList()){
							if(!c.getPseudo().equalsIgnoreCase(clientTCP.getClient().getPseudo()))
								new Thread(new EnvoiUDP(mes_envoie,c.getPort(),c.getAdresse())).start();
						}
						break;
					}
				}
			}
			else if(message[0].equalsIgnoreCase("eg")){
				boolean existe=false;
				for(ClientGroupe nom:listegroupe){
					if(message[1].equalsIgnoreCase(nom.getNom())){
						existe=true;
						if(nom.getList().size()==0){
							nom.getList().add(client);
							for(ClientConfig cli:listeclient)
								new Thread(new EnvoiUDP("RENTRERGROUPE:"+message[1]+":"+client.getPseudo()
										,cli.getPort(),cli.getAdresse())).start();
							
							System.out.println("Le groupe"+message[1]+"vient d'être créée");
						}
						else{
							listeconfig.add(new Acquitement(message[1], nom.getList().size()));
							for(ClientConfig cli:nom.getList()){
								//System.out.println(mes);
								if(!cli.equals(client))
									new Thread(new EnvoiUDP("DEMANDEGROUPE:"+message[1]+":"+client.getPseudo()
											,cli.getPort(),cli.getAdresse())).start();
							}
							break;
						}
					}
				}
				if(!existe){
					listegroupe.add(new ClientGroupe(message[1]));
					listegroupe.get(listegroupe.size()-1).getList().add(client);
					for(ClientConfig cli:listeclient)
						new Thread(new EnvoiUDP("RENTRERGROUPE:"+message[1]+":"+client.getPseudo()
								,cli.getPort(),cli.getAdresse())).start();
				}
			}
			else if(message[0].equalsIgnoreCase("qg")){
				for(ClientGroupe nom:listegroupe){
					if(nom.getNom().equalsIgnoreCase(message[1])){
						nom.getList().remove(client);
						break;
					}
				}
				for(ClientConfig c: listeclient)
					new Thread(new EnvoiUDP("QUITTERGROUPE:"+message[1]+":"+client.getPseudo()
							,c.getPort(),c.getAdresse())).start();
			}
			else if(message[0].equalsIgnoreCase("rg")){
				for(ClientConfig c:listeclient){
					if(c.getPseudo().equalsIgnoreCase(message[2])){
						if(!c.equals(client)){
							new Thread(new EnvoiUDP("REPONSEGROUPE:"+message[1]+":"+message[2]+":"+message[3]
									,c.getPort(),c.getAdresse())).start();
							break;
						}
					}
				}
			}else if(message[0].equalsIgnoreCase("o")){
				System.out.println("'lc'                             -> Afficher la liste des clients");
				System.out.println("'lg'                             -> Afficher la liste des groupe");
				System.out.println("'q'                              -> Quitter");
				System.out.println("'s:pseudo_destinataire:message'  -> Envoyer message");
				System.out.println("'sg:groupe:message'              -> Envoyer message au groupe");
				System.out.println("'eg:groupe'                      -> Création ou demande d'entrer un groupe ");
				System.out.println("'qg:groupe'                      -> Quitter un groupe");
				System.out.println("'rg:groupe:pseudo:ACCEPT/REFUSE' -> Accepter/Refuser lentrer dans le groupe");
			}
		}
		
	}

}
