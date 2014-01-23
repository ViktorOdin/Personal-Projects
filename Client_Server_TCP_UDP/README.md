Projet de Protocole Internt de Master Ingénierie Informatique.
Victor Oudin & Antony Guillet.

But : créer un Serveur TCP qui maintient une liste de Clients
Et créer des Clients, qui grâce à la liste de Clients du Serveur, peuvent communiquer par UDP.

Utilisation : importer les sources dans un IDE Java (ex: Eclipse)

Mail : victor.oudin@etu.univ-paris-diderot.fr

Protocol TCP:
	Client:
		- CONNECT:pseudo:port 	// connexion au serveur.
		- DECONNECT		// deconnexion.
	Serveur:
		- ACK_CONNECT						// serveur rend acte de connexion.
		- LISTE:pseudo:adresse:port: ... :pseudo:adresse:port	// liste des Clients déjà connecté au moment de la connexion.
		- NEW:pseudo:adresse:port				// nouveau Client connecté au serveur.
		- QUIT:pseudo						// Client plus connecté au serveur. 
Protocol UDP:
	- DEMANDEGROUPE:groupe:pseudo			// demande pour rentrer dans un groupe.
	- REPONSEGROUPE:groupe:pseudo:ACCEPT/REFUSE	// reponse à un demande d'entrer dans un groupe.
	- RENTRERGROUPE:groupe:pseudo			// entrer dans un groupe.
	- QUITTERGROUPE:groupe:pseudo			// quitter un groupe.
	- CHAT:pseudo_expéditeur:message		// envoi d'un message à un Client.
	- CHATGROUPE:groupe:pseudo_expéditeur:message	// envoi d'un message à un groupe.

