#include "easyTcpServer.hpp"
#include <chrono>
#include <ctime>

#include "data_exchange/card_exchange.hpp"

int main(int argc, char** argv)
{
  tcp_server serv(argv[1],4242);
  io_interface ioservice;
  std::time_t t = std::time(0);
  std::tm* now = std::localtime(&t);
  std::cout << "Starting PSaMOS V0, Have a good day Operator." << '\n';
  //std::vector<card_exchange> v;
  while(1)
  {
    t = std::time(0);
    now = std::localtime(&t);
    if(!serv.get_new_client_fifo()->empty())
    {
      std::cout << "[" << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "] welcome to the new client from " << serv.get_new_client_fifo()->front().ip_addr.c_str() << '\n';
      serv.send_to_client(serv.get_new_client_fifo()->front(),"Welcome, I'm PSaMOS V0.0.0");
      serv.get_new_client_fifo()->pop();
    }
    serv.lock_client();
    for(int i=0; i<serv.get_client()->size();i++)
    {
      if(serv.get_client()->at(i).readable)
      {
        try
        {
          switch(ioservice.detect_protocole(serv.get_client()->at(i)))
          {
            case 1:
              ioservice.card_exchange(serv.get_client()->at(i),&serv);
              std::cerr << "INFO : restart listening" << '\n';
              //serv.get_client()->at(i).readable --;
              //serv.get_client()->at(i).msg.pop();
              break;

            default:
            std::cout << "[" << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "] echo to client : " << i << " - " << serv.get_client()->at(i).msg.front().c_str() << '\n';
            serv.send_to_client(serv.get_client()->at(i),serv.get_client()->at(i).msg.front().c_str());
            serv.get_client()->at(i).readable --;
            serv.get_client()->at(i).msg.pop();
          }
      }
      catch(const std::exception& e)
      {
        std::cerr << "[" << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << "] Exception occured : " << e.what() << '\n';
      }
      }
    }
    serv.unlock_client();
  }
  return 0;
}
