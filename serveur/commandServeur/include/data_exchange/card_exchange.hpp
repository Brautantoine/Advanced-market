#ifndef CARD_EXCHANGE_HPP
#define CARD_EXCHANGE_HPP

#include "card.hpp"
#include "easyTcpServer.hpp"

#include <thread>
#include <sstream>

using easyTCP::connectedClient;
using  Card::card;

class io_interface
{
  public:
    io_interface(){};
    void card_exchange();
    void card_exchange(connectedClient client, tcp_server* serv);
    virtual ~io_interface();

    int detect_protocole(connectedClient& client);

  protected:
    //

  private:
    void _exchange_task();
    //connectedClient _client;
    //tcp_server* _serv;
    card panier;

    std::thread _exchange_thread;
};



#endif //CARD_EXCHANGE_HPP
