#ifndef CARD_EXCHANGE_HPP
#define CARD_EXCHANGE_HPP

#include "card.hpp"
#include "easyTcpServer.hpp"

#include <exception>
#include <sstream>
#include <ctime>


using easyTCP::connectedClient;
using  Card::card;

class io_interface
{
  public:
    io_interface(){};
    void card_exchange();
    void card_exchange(connectedClient& client, tcp_server* serv);
    virtual ~io_interface();

    int detect_protocole(connectedClient& client);

  protected:
    //

  private:
    card panier;
};



#endif //CARD_EXCHANGE_HPP
