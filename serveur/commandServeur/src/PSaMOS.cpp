#include "easyTcpServer.hpp"

int main(int argc, char** argv)
{
  tcp_server serv(argv[1],4242);
  std::cout << "Starting PSaMOS V0, Have a good day Operator." << '\n';
  while(1)
  {
    if(!serv.get_new_client_fifo()->empty())
    {
      std::cout << "welcome to the new client from " << serv.get_new_client_fifo()->front().ip_addr.c_str() << '\n';
      serv.send_to_client(serv.get_new_client_fifo()->front(),"Welcome, I'm PsaMOS V0.0.0");
      serv.get_new_client_fifo()->pop();
    }
    serv.lock_client();
    for(int i=0; i<serv.get_client()->size();i++)
    {
      if(serv.get_client()->at(i).readable)
      {
        std::cout << "echo to client : " << i << " - " << serv.get_client()->at(i).msg.front().c_str() << '\n';
        serv.send_to_client(serv.get_client()->at(i),serv.get_client()->at(i).msg.front().c_str());
        serv.get_client()->at(i).readable --;
        serv.get_client()->at(i).msg.pop();
      }
    }
    serv.unlock_client();
  }
  return 0;
}
