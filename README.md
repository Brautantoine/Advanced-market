# Advanced marché de Cachan <img src="http://www.iut-cachan.u-psud.fr/skins/newcachan/resources/img/xLogo-UPSud-Saclay_horizontal_IUT-CACHAN.jpg.pagespeed.ic.cqHiSmvYr4.jpg" alt="logo-iut-cachan" width="180" align="right" />

Copyright Advanced marché de Cachan (C) 2019 Antoine Braut - Bastien Duprey for IUT de Cachan 

## Qu'est ce qu'Advanced marché de Cachan ? ##

Advanced marché de Cachan est un projet ayant pour but de créer une solution pour faciliter la préparation de panier. Il se décompose en plusieurs parties :

1. [Le serveur de gestion de commande](https://github.com/Brautantoine/projet_du_chemar/tree/master/serveur/commandServeur)
	Il a pour but de récupérer le panier d'un client ( en provenance d'un site de e-commerce, d'une borne de commande ...) et de les répartir entre les différents préparateurs
2. [La tablette terminal](https://github.com/Brautantoine/projet_du_chemar/tree/master/Tablette)
	Elle a pour but de récupérer un panier depuis le serveur puis de la valider en scannant chacun des articles avec un scanner bluetooth
3. [Le Bracelet terminal](https://github.com/Brautantoine/projet_du_chemar/tree/master/Bracelet/)
	Il a pour but de récupérer un panier depuis le serveur puis de le valider en scannant chacun des articles avec un scanner bluetooth

<img src="https://github.com/Brautantoine/projet_du_chemar/blob/master/gestionDeProjet/Schema/Synoptique/SynoptiqueV1.png"  alt="drawing" width="720"/>

>Les terminaux communiquent avec le serveur en wi-fi, puis sont capable d'opérer hors-ligne jusqu'à la validation du panier afin de pouvoir fonctionner au fin fond d'un entrepôt.

## Et pour la liste des produits ? ##

La liste des produits est stockée sur une base de données MySQL avec laquelle communique le serveur de gestion des paniers. Tous les fichiers relatifs à la liste des produits sont disponibles dans ce [dossier](https://github.com/Brautantoine/projet_du_chemar/tree/master/productTable) ainsi que le dossier du serveur pour la partie base de données.

