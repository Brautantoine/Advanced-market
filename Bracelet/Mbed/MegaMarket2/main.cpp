#include "mbed.h"
#include "F746_GUI.hpp"
#include "lESP.hpp"
#include <string>
#include <queue>
#include <vector>
#include <sstream>


DigitalOut myled(LED1);
Serial bluetooth(PC_6,PC_7);
Serial bridge(PF_7, PF_6);
Serial pc(USBTX,USBRX);
wifiAdapter esp(bridge);

std::queue<ligne_panier> panier;

void connexion_menu()
{
 bool alive(true);
 
 GuiBase::GetLcd().Clear(0x00ff0000);
 GuiBase::GetLcd().SetBackColor(0xff6600);
 
 Label welcome(20,20,"Bienvenue sur MegaMarketV2 Operateur 210");
 Button test_co(350,20,100,50,"Obtenir panier");
 Button new_panier(20,220,150,50,"Nouveau panier");
 Button new_table(300,220,180,50,"Mettre a jour la table");
    
 while(alive)
 {
    if(new_panier.Touched())
    {
        alive=false;
    }
    if(test_co.Touched())
    {
        esp.connect_to_AP("MSI 6606","fruitetlegumes1");
        //wait(10);
        esp.connect_to_tcp_server("192.168.43.125",4242);
        //wait(10);
        /*std::string command;
        command.push_back(0x10);
        command.push_back(0x01);
        command.append("SENT FROM STM32");
        esp.Send(command);*/
        esp.get_card();
        //esp.Send("SENT FROM STM32");
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
 
 std::vector<std::string> card_list_text;
 std::vector<Label*> card_list_label;
 GuiBase::GetLcd().Clear(0x00ff0000);
 for(int i=0;i<8;i++)
 {
     card_list_label.push_back(new Label(300,50+(20*i),""));
 }

 //LCD_DISCO_F746NG lcd(GuiBase::GetLcd());
 
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
          if(panier.front().article.size()>17)
            next_article.Draw("Article suivant : "+panier.front().article.substr(0,17)+".");
          else
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
          if(panier.front().qt>1)
          {
           Button confirm(20,220,200,50,"Confirme quantite");
           Label warning(20,160,"Attention : Quantite requise - "+cast_int.str());
           while(!confirm.Touched());
           confirm.Erase();
           warning.Draw("");
          }
          cast_int.str("");
          cast_int << panier.front().qt;
          if(panier.front().article.size()>17)
            card_list_text.push_back("- "+cast_int.str()+" "+panier.front().article.substr(0,17)+".");
          else
            card_list_text.push_back("- "+cast_int.str()+" "+panier.front().article);
          for(int i=0;i<card_list_text.size();i++)
          {
           card_list_label.at(i)->Draw(card_list_text.at(i));   
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
 for(int i=0;i>8;i++)
 {
     free(card_list_label.at(i));
 }
}

int main() 
{      
    
    panier.push(ligne_panier("STM32F7","A1","4a9acc",1));
    panier.push(ligne_panier("Jus de fruits","B2","65dd17",2));
    panier.push(ligne_panier("Generateur d alcool infini","C9","cb091d",5));
    panier.push(ligne_panier("Guitare electro acoustique","C8","cbab97",1));
    while(1) {
        connexion_menu();
        command_view(panier);
    }
}

