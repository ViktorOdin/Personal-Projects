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
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class EnvoiUDP implements Runnable{
	
	private String mes,adr;
	private int port;
	
	/*** class d'envoi d'un message par UPD ***/
	public EnvoiUDP(String mes,int port,String adr){
		this.mes=mes;
		this.port=port;
		this.adr=adr;
	}

	public void run(){
		System.out.println("Envoi par UDP vers les aux autres clients");	
		//System.out.println(mes);
		int ch = mes.length();
		byte[] buffer = new byte[500];
		buffer = mes.getBytes();
		
		DatagramPacket paquetUDP = null;
		try{
			paquetUDP = new DatagramPacket(buffer ,ch ,InetAddress.getByName(adr),port);
		}catch (NumberFormatException e){
			System.out.println("Number Format exception : "+e);
			e.printStackTrace();
		}catch (UnknownHostException e){
			System.out.println("Unknow Host exception : "+e);
			e.printStackTrace();
		}
		DatagramSocket s = null;
		
		try{
			s = new DatagramSocket();
			s.send(paquetUDP);
		}catch (SocketException e){
			System.out.println("Socket exception : "+e);
			e.printStackTrace();
		}catch (IOException e){
			System.out.println("Datagram Socket failed : "+e);
			e.printStackTrace();
		}
		s.close();
	}
}
