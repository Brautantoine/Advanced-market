# ifndef LESP_HPP
# define LESP_HPP

#include <string>
#include <sstream>
#include <mbed.h>

extern Serial pc;

class wifiAdapter
{
    public :
    
        wifiAdapter(Serial& n_bridge); // tx, rx
        virtual ~wifiAdapter();
        
        void connect_to_AP(std::string SSID, std::string password);
        void connect_to_tcp_server(std::string ip_adress, int port);
        void Send(std::string);
        
        void attach_callback();
        
    private :
    
        void rx_interrupt();
    
        Serial& bridge;
        bool is_connected;
        std::string buffer;
};

# endif //LESP_HPP
