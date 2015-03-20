# HTCPCP
Implémentation de la RFC 2324 / Implementation of RFC 2324

Serveur : 

	- Ecoute sur le port 80
	- Traite les requêtes GET / PROPFIND / BREW / WHEN
	- Communique avec l'arduino via le port serie

Client : 
	
	- Donne la commande de café au serveur.
	- Possibilité d'envoyer une requête GET / PROPFIND / BREW / WHEN

Arduino : 

	- Par défaut : Relay à OFF 
	- Ecoute sur le port serie en attente du commande de café.

RFC :

	- La methode WHEN a été modifié.
	- Il manque de nombreux détails à implémenter.

Bug à fixer :

	- La gestion du timer pour la methode WHEN rencontre des erreurs.
	- Communication avec les clients à revoir. (pour deco / reco des clients.)

Le projet a été fait le temps d'un week end, le code est assez sale et manque encore de travail. En attendant de trouver le temps de le reprendre, je le laisse à disposition pour inspirer/aider de potentiels amateurs de café !
