#include "mbed.h"
#include "F746_GUI.hpp"
#include <string>
#include <queue>
#include <vector>
#include <sstream>


DigitalOut myled(LED1);
Serial bluetooth(PC_6,PC_7);

struct ligne_panier
{
 std::string article;
 std::string emplacement;
 std::string unique_id;
 int qt;   
 ligne_panier(std::string _article, std::string _emplacement, std::string _unique_id, int _qt)
 {
  article = _article;
  emplacement = _emplacement;
  unique_id = _unique_id;
  qt = _qt;   
 }
};

void connexion_menu()
{
 bool alive(true);
 
 GuiBase::GetLcd().Clear(0x00ff0000);
 GuiBase::GetLcd().SetBackColor(0xff6600);
 
 Label welcome(20,20,"Bienvenue sur MegaMarket Operateur 210");
 Button new_panier(20,220,150,50,"Nouveau panier");
 Button new_table(300,220,180,50,"Mettre a jour la table");
    
 while(alive)
 {
    if(new_panier.Touched())
    {
        alive=false;
    }
 }
}

void command_view(std::queue<ligne_panier> panier)
{
 bool alive(true);
 ostringstream cast_int;
 std::string scan;
 char buff('a');
 
 
 cast_int << panier.front().qt;
 
 std::vector<Label> card_list;
 //LCD_DISCO_F746NG lcd(GuiBase::GetLcd());
 GuiBase::GetLcd().Clear(0x00ff0000);
 //lcd.clear();
 Label id_panier(20,20,"ID du panier : 0xT4E5SaT2");
 Label card_view(300,20,"Panier actuel");
 Label next_article(20,50,"Article suivant : "+panier.front().article);
 Label where_next_article(20,70,"Emplacement prochaine article : "+panier.front().emplacement);
 Label qt_next_article(20,90,"Quantite requise : "+cast_int.str());
 
 Label scanned_string(20,140,"scanned");
 
 Button retour(300,220,150,50,"retour au menu",Font12,GuiBase::ENUM_TEXT,0xcecece);
 retour.Inactivate();
 
 
 while(alive)
 {     
      while(!panier.empty())
      {
          next_article.Draw("Article suivant : "+panier.front().article);
          where_next_article.Draw("Emplacement prochaine article : "+panier.front().emplacement);
          cast_int.str("");
          cast_int << panier.front().qt;
          qt_next_article.Draw("Quantite requise : "+cast_int.str());
          while(scan.compare(panier.front().unique_id))
          {
              scan.clear();
              while(buff!='\n')
              {
               buff = bluetooth.getc();
               scan.push_back(buff);
              }
              buff='a';   
              scan = scan.substr(0,scan.size()-2);
              scanned_string.Draw(scan);
          }
      
          panier.pop();
          
      
      }
     scanned_string.Draw("All scanned");
     retour.Activate();
     if(retour.Touched())
     {
      alive=false;   
     }
 }   
}

int main() 
{      
    std::queue<ligne_panier> panier;
    panier.push(ligne_panier("STM32F7","A1","4a9acc",1));
    panier.push(ligne_panier("Jus de fruits","B2","65dd17",2));
    panier.push(ligne_panier("Generateur d alcool infini","C9","cb091d",5));
    panier.push(ligne_panier("Guitare electro acoustique","C8","cbab97",1));
    while(1) {
        connexion_menu();
        command_view(panier);
    }
}

