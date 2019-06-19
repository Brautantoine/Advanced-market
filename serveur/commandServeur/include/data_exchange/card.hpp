#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <vector>

namespace Card
{
  struct article
  {
    std::string name;
    std::string unique_id;
    std::string emplacement;
    int qt;
    article(std::string _name, std::string _unique_id, std::string _emplacement, int _qt)
    {
      name = _name;
      unique_id = _unique_id;
      emplacement = _emplacement;
      qt = _qt;
    }
  };

  struct card
  {
    std::string card_id;
    int qt;
    std::vector<article> article_list;
    card(){qt=0;}
    card(std::string card_id)
    {
      this->card_id = card_id;
      qt=0;
    }
    void add_article(const article& new_article)
    {
      article_list.emplace_back(new_article);
      this->qt++;
    }
    void add_article(std::string name, std::string unique_id, std::string emplacement, int qt)
    {
      article_list.emplace_back(name,unique_id,emplacement,qt);
      this->qt++;
    }
  };
}

#endif //CARD_HPP
