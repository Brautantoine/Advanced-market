IUT DE CACHAN		                                   Braut Antoine
Personnal Request for Comments: 1                               
      	                                                     May 5, 2019
References: PRFC 1

# Protocole du serveur de gesion des paniers #

> Ce document à pour but de définir les protocole associés aux divers échanges entre le serveur de gestion des paniers et les terminaux des opérateurs (Tablette et Bracelet). Ce document n'a pas pour but d'être clair ou succint, il tente juste de donner une lignes directrices.

## 1. Définition du serveur de gestion des paniers ##

Le serveur de gestion des paniers à pour but :

1. Récuperer des paniers (fichier au format `.json` dans un dossier spécifique) et les mettres en attente
2. Envoyer vers un terminal un panier selon le protocole défini
3. Valider un panier préparé par un opérateur
4. Mettre à jour la table d'un terminal
