#!/usr/bin/python

import socket
import time

TCP_IP = '192.168.1.102'
TCP_PORT = 4242
BUFFER_SIZE = 2

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

print 'Lancement du serveur d\'echo [V 1.0.0]\n En attente de connection ...'

client1 , addr = s.accept()

print 'Connection address:',addr
client1.send("Bienvenue sur le serveur d\'echo")
while(1):
	Buff = client1.recv(1000)
	if (len(str(Buff))) :
		print 'Received from client :',Buff
		client1.send(str(Buff))
client1.close() 
