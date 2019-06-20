# ifndef LESP_HPP
# define LESP_HPP

#include <string>
#include <sstream>
#include <mbed.h>
#include <vector>
#include <cstdlib>
#include <queue>

extern Serial pc;

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

class wifiAdapter
{
    public :
    
        wifiAdapter(Serial& n_bridge); // tx, rx
        virtual ~wifiAdapter();
        
        void connect_to_AP(std::string SSID, std::string password);
        void connect_to_tcp_server(std::string ip_adress, int port);
        void Send(std::string);
        void get_card();
        
        void attach_callback();
        
    private :
    
        int find_in_table(std::string key);
        void rx_interrupt();
    
        Serial& bridge;
        bool is_connected;
        std::string buffer;
        
        std::vector<ligne_panier> correspondance_table;
        
};

# endif //LESP_HPP
