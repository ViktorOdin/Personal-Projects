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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Scanner;

public class Client extends Thread{
	
	private Socket socket;
	private ClientConfig client;
	private ArrayList<ClientConfig> clientconfig;
	private ArrayList<ClientGroupe> clientgroup;
	private ArrayList<Acquitement> list_conf;
	private PrintWriter sortie;
	private BufferedReader entree;
	boolean fin = false;
	private Thread control;
	private Thread recep;
	private RecoiUDP rep_udp;
	
	/*** Client ***/
	public Client(ClientConfig cli, String adresseServeur, int portServeur){
		setClient(cli);
		setList(new ArrayList<ClientConfig>());
		clientgroup= new ArrayList<ClientGroupe>();
		list_conf = new ArrayList<Acquitement>();
		try{
			socket = new Socket(adresseServeur, portServeur); //information serveur distant
			sortie=new PrintWriter(socket.getOutputStream());
			entree = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			sortie.write("CONNECT:"+client.getPseudo()+":"+client.getPort()+"\n");
		}catch (UnknownHostException e){
			System.out.println("Unknow Host : "+e);
			e.printStackTrace();
		}catch (IOException e){
			System.out.println("Client Socket : "+e);
			e.printStackTrace();
		}
		sortie.flush();
		rep_udp=new RecoiUDP(client.getPort(),clientgroup,clientconfig,list_conf,client);
		recep=new Thread(rep_udp);
		recep.start();
		control=new Thread(new Terminal(this,clientgroup,client,clientconfig,list_conf));
		control.start();

	}

	@SuppressWarnings("deprecation")
	public void run(){
		while(!fin){
			String mes = null;
			try{
				mes= entree.readLine();
			}catch(IOException e){
				System.out.println("ReadlLine failed : "+e);
				e.printStackTrace();
			}
			String[] message=mes.split(":");
			System.out.println(mes);
			if(message[0].equalsIgnoreCase("LISTE")){
				for(int i =1; i<message.length;i=i+3){
					System.out.println("Pseudo:"+message[i]+" adresse: "+message[i+1]+" Port:"+message[i+2]);
					clientconfig.add(new ClientConfig(message[i+1],message[i],Integer.valueOf(message[i+2])));
				}
			}
			else if(message[0].equalsIgnoreCase("NEW")){
				clientconfig.add(new ClientConfig(message[2],message[1],Integer.valueOf(message[3])));
				try{
					sleep(0,5);
				}catch (InterruptedException e){
					System.out.println("Sleep failed : "+e);
					e.printStackTrace();
				}
				for(ClientGroupe g: clientgroup){
					for(ClientConfig c: g.getList()){
						new Thread(new EnvoiUDP("RENTRERGROUPE:"+g.getNom()+":"+c.getPseudo()
								,Integer.valueOf(message[3]),message[2])).start();
					}
				}
			}
			else if(message[0].equalsIgnoreCase("QUIT")){
				for(int j=0; j<clientgroup.size(); j++){
					for(int i =0; i<clientgroup.get(j).getList().size(); i++){
						if(clientgroup.get(j).getList().get(i).getPseudo().equalsIgnoreCase(message[1])){
							clientgroup.get(j).getList().remove(i);
						}
					}
				}
				for(ClientConfig c : clientconfig)
					if(c.getPseudo().equalsIgnoreCase(message[1])){
						clientconfig.remove(c);break;
					}
			}
		}
		System.out.println("TCP");
		
		sortie.close();
		
		try{
			socket.close();
			entree.close();
			recep.stop();
			rep_udp.interrupt();
		}catch (IOException e){
			System.out.println("Close Socket failed : "+e);
                        e.printStackTrace();
		}
	}
	
	public boolean isfin(){
		return fin;
	}
	
	public void setfin(boolean fin){
		this.fin = fin;
	}
	
	@SuppressWarnings("deprecation")
	public void quitter(){
		stop();
		sortie.write("DECONNECT");
		sortie.flush();
		sortie.close();
		
		try{
			entree.close();
			socket.close();
			recep.stop();
			rep_udp.interrupt();
			control.stop();
		}catch (IOException e){
			System.out.println("Close failed : "+e);
                        e.printStackTrace();
		}
	}
	public ClientConfig getClient(){
		return client;
	}
	
	public void setClient(ClientConfig client){
		this.client = client;
	}
	
	public ArrayList<ClientConfig> getList(){
		return clientconfig;
	}
	
	public void setList(ArrayList<ClientConfig> clientconfig){
		this.clientconfig = clientconfig;
	}
	
	public ArrayList<ClientGroupe> getGroupclient(){
		return clientgroup;
	}
	
	public void setGroupclient(ArrayList<ClientGroupe> groupclient){
		this.clientgroup = groupclient;
	}
	
	public static void main(String[] args){
		String pseudo,adresseClient,adresseServeur;
		int portClient,portServeur;
		Scanner scan =new Scanner(System.in);
		System.out.println("Entrez pseudo client (6 lettres)");
		pseudo = scan.nextLine();
		System.out.println("Entrez adresse client");
		adresseClient = scan.nextLine();
		System.out.println("Entrez port client");
		portClient = Integer.parseInt(scan.nextLine());
		System.out.println("Entrez adresse serveur (6 lettres)");
		adresseServeur = scan.nextLine();
		System.out.println("Entrez port serveur");
		portServeur = Integer.parseInt(scan.nextLine());
		
		new Client(new ClientConfig(adresseClient, pseudo, portClient),adresseServeur,portServeur).start(); //information perso
	}
}
