#include "easyTcpServer.hpp"

tcp_server::tcp_server()
{

}
tcp_server::tcp_server(std::string ip_addr,int port_number)
{
  server_addr = ip_addr;
  server_port = port_number;

  _start_listening();
}

tcp_server::~tcp_server()
{

}

void tcp_server::start_server(std::string ip_addr,int port_number)
{
  server_addr = ip_addr;
  server_port = port_number;

  _start_listening();
}

void tcp_server::_start_listening()
{
  server_socket = socket(AF_INET,SOCK_STREAM,0);

  memset(&_server,0,sizeof(_server));

  _server.sin_family=AF_INET;
	_server.sin_addr.s_addr=htonl(INADDR_ANY);
  _server.sin_port=htons(server_port);

  bind(server_socket,(struct sockaddr *)&_server, sizeof(_server));
  listen(server_socket,easyTCP::MAXCONNECTIONS);

  listener_task = std::thread(&tcp_server::_listen_for_client,this);


}

void tcp_server::_listen_for_client()
{
  fd_set readfds;
  int max_sd, event;
  listener_task_alive = true;
  int sd(0);
  char buffer[1024];
  int valread;

  memset(buffer,0,sizeof(buffer));
  length_sockadr = sizeof(_server);
  //std::cerr << "start listen thread" << std::endl;
  while(listener_task_alive)
  {
    //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        //add child sockets to set
        for (int i = 0 ; i < clientTable.size() ; i++)
        {
            //socket descriptor
            sd = clientTable.at(i).socket;

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
        //std::cerr << "start select" << std::endl;
        event = select(max_sd+1, &readfds, NULL, NULL, NULL);
        //std::cerr << "event occur\n";
        if (FD_ISSET(server_socket, &readfds))
        {
          int new_client;
          //std::cerr << "new client\n";
            if ((new_client = accept(server_socket, (struct sockaddr *)&_server, (socklen_t*)&length_sockadr))<0)
            {
                // Throw
            }

            //inform user of socket number - used in send and receive commands
            //std::cerr << "New connection , socket port is" << inet_ntoa(_server.sin_addr) <<  '\n';

            //send new connection greeting message // /* Test phase */
          /**///send(new_client, "Welcome on echo server V2", strlen("Welcome on echo server V2"), 0);

            //add new socket to array of sockets
            clientTable.emplace_back(new_client,inet_ntoa(_server.sin_addr));
            new_client_fifo.emplace(new_client,inet_ntoa(_server.sin_addr));
        }

        for (int i = 0; i < clientTable.size(); i++)
        {
            sd = clientTable.at(i).socket;

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                //valread = read(sd, buffer, 1024);
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                  try
                  {
                  std::cerr << "client disconnected : " << i << std::endl;
                    //Somebody disconnected , get his details and print;

                    //Close the socket and mark as 0 in list for reuse
                    _disconnect_lock = std::unique_lock<std::mutex> (disconnect_lock);
                    close(sd);
                    clientTable.erase(clientTable.begin()+i);
                    _disconnect_lock.unlock();
                    //clientTable.shrink_to_fit();
                  }
                  catch(const std::exception& e)
                  {
                    std::cerr << "error disconnect" << std::endl;
                    std::cerr << e.what() << '\n';
                  }
                }

                //Echo back the message that came in
                else if (valread > 0)
                {
                  try
                  {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    //std::cerr << "echo" << '\n';
                    //std::cerr << "receive from client : " << buffer << std::endl;
                    buffer[valread] = '\0';
                    clientTable.at(i).readable ++;
                    clientTable.at(i).msg.emplace(buffer);
                    //send(sd , buffer , strlen(buffer) , 0 );
                  }
                  catch(const std::exception& e)
                  {
                    std::cerr << "error 2" << std::endl;
                    std::cerr << e.what() << '\n';
                  }
                }
            }
        }

  }
}

void tcp_server::stop_server()
{

}

void tcp_server::send_to_client(int socket, std::string msg)
{
  send(socket, msg.c_str(), sizeof(msg), 0);
}
void tcp_server::send_to_client(connectedClient client, std::string msg)
{
  send(client.socket, msg.c_str(), sizeof(msg), 0);
}

void tcp_server::lock_client()
{
  _disconnect_lock = std::unique_lock<std::mutex> (disconnect_lock);
}

void tcp_server::unlock_client()
{
  _disconnect_lock.unlock();
}
