#include "data_exchange/card_exchange.hpp"

void io_interface::card_exchange()
{

}
void io_interface::card_exchange(connectedClient client, tcp_server* server)
{
  // Test Zone //
  std::cerr << "start constructor of card_exchange" << '\n';
  panier = card("2B");
  panier.add_article("STM32F7","4a9acc","A1",2);
  panier.add_article("Jus de fruit","65dd17","B1",7);

  //\ End Test Zone //\
  //_exchange_thread = std::thread(&card_exchange::_exchange_task,this);

  std::cerr << "start exchange sequence" << '\n';
  std::ostringstream tx_buffer;

  tx_buffer << (char)0X10 << char(0x10) << panier.card_id << panier.qt;
  for(int i=0;i<panier.article_list.size();i++)
  {
    tx_buffer << panier.article_list.at(i).unique_id << "." << panier.article_list.at(i).qt << "/";
  }
  tx_buffer << char(0XFF);
  std::cerr << "J'envoi un panier : " << tx_buffer.str() << std::endl;
  system(("echo "+tx_buffer.str()+" | hexdump").c_str());
  server->send_to_client(client,tx_buffer.str());
}

io_interface::~io_interface()
{
  std::cerr << "start destructor " << '\n';
  //_exchange_thread.join();

}

void io_interface::_exchange_task()
{
  /*std::cerr << "start exchange sequence" << '\n';
  std::ostringstream tx_buffer;

  tx_buffer << 0X1001 << panier.card_id << (int)panier.qt;
  for(int i=0;i<panier.article_list.size();i++)
  {
    tx_buffer << panier.article_list.at(i).unique_id << "." << panier.article_list.at(i).qt << "/";
  }
  tx_buffer << 0XFF;
  std::cerr << "J'envoi un panier : " << tx_buffer.str() << std::endl;
  _serv->send_to_client(_client,tx_buffer.str());*/

}

int io_interface::detect_protocole(connectedClient& client)
{
  std::cerr << "INFO : Je cherche un protocole" << '\n';
  int ret(0);
  if(client.msg.front().size()>1)
  {
    std::cerr << "INFO : La longueur est ok" << '\n';
    if(client.msg.front().at(0) == 0x10 && client.msg.front().at(1) == 0x01)
    {
      std::cerr << "INFO : j'ai trouve un echange de panier" << '\n';
      ret = 1;
    }
  }

  return ret;
}
