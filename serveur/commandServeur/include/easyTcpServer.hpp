#ifndef EASYTCPSERVER
#define EASYTCPSERVER

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include <iostream>
#include <exception>

namespace easyTCP
{
  struct connectedClient
  {
    int socket;
    std::string ip_addr;
    int readable;                                                               // Pseudo Semaphore
    std::queue<std::string> msg;
    connectedClient(int _socket, std::string _ip)
    {
      socket = _socket;
      ip_addr = _ip;
      readable = 0;
    }
  };

  const int MAXCONNECTIONS = 5;
}

using namespace easyTCP;

class tcp_server
{

  public:
    tcp_server();
    tcp_server(std::string ip_addr,int port_number);
    virtual ~tcp_server();

    void start_server(std::string ip_addr,int port_number);
    void stop_server();

    void send_to_client(int socket, std::string msg);
    void send_to_client(connectedClient client, std::string msg);

    std::vector<connectedClient>* get_client(){ return &clientTable;}
    std::queue<connectedClient>* get_new_client_fifo(){ return &new_client_fifo;}

    void lock_client();
    void unlock_client();

  protected:
    //

  private:

    void _start_listening();
    void _listen_for_client();

    std::vector<connectedClient> clientTable;
    std::queue<connectedClient> new_client_fifo;

    std::string server_addr;
    int server_port;
    int server_socket;

    std::thread listener_task;
    bool listener_task_alive;

    struct sockaddr_in _server;
    int length_sockadr;

    std::mutex disconnect_lock;
    std::unique_lock<std::mutex> _disconnect_lock;

};

#endif // EASYTCPSERVER
