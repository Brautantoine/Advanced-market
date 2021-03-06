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

## 2. Récupération et traitement des paniers ##

Les paniers sont créés en amont (site de e-commerce, borne de commande ...) puis envoyé indépendament du serveur dans un dossier spécifique. 
>Les paniers sont décris dans un fichier .json ou .xml ( les fichiers .xml sont pour l'instant une idée plus qu'autre chose ...) 

Le serveur scrute en permanence le dossier pour détecter l'apparition d'un nouveau fichier de panier. Lors de l'apparition d'un fichier de panier, le serveur le récupére, le traite et le place en attente de traitement. Le fichier de panier est alors déplacer vers un autre dossier.

## 3. Envoyer un panier à un opérateur ##

>Toutes les trames échangées lors de cette opération commence avec le header ID_NPANIER - 0X1001

Lorsqu'un opérateur est prêt pour un nouveau panier, il se connecte au serveur et envoie une requête de nouveau panier. Le Trame de requête se compose uniquement de l'en tête de l'échange `ID_NPANIER, 0X1001`. Le serveur lui répond en lui envoyant le nouveau panier à préparer ( voir la figure ci dessous pour le format de la trame). Une fois le panier obtenue, le terminal renvoie une trame pour valider la réception `ID_NPANIER,ID\_PANIER,QT\_PANIER`. Le serveur renvoie alors une derniere réponse pour valider ou non l'échange `ID_NPANIER,DATA\_VALIDATION?`.

<img src="https://github.com/Brautantoine/projet_du_chemar/blob/master/gestionDeProjet/Schema/Trame/Nouveau_panier.png"  alt="Trame\_panier" width="720"/>
