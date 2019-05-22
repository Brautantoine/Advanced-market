# LibESP #

LibESP est une librairie implémentant une class pour interfacer l'ESP avec le uc en automatisant l'envoie de commande AT sur une liaison serie.

## Comment l'utiliser ? ##

Pour utiliser le module Wi-Fi il faut d'abord instancier une classe wifiAdapter avec un liaison serie `Serial`

```c++

wifiAdapter esp(bridge); // Instancie l'interface (bridge est un Serial)

esp.connect_to_AP("SSID","PSSWD"); // Connexion au point d'accés

esp.connect_to_tcp_server("192.168.1.102",4242); // Connexion au serveur en TCP

esp.Send("SENT FROM STM32"); // Envoie d'un message au serveur

```

## Comment ça marche ? ##

> ça marche 
