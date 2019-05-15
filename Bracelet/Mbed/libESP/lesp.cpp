#include "lESP.hpp"

wifiAdapter::wifiAdapter(Serial& n_bridge): bridge(n_bridge)
{
    is_connected=false;
    bridge.printf("AT+RST\r\n");
}

wifiAdapter::~wifiAdapter()
{
        
}
        
void wifiAdapter::connect_to_AP(std::string SSID, std::string password)
{
    //Serial pc(USBTX, USBRX);
    ostringstream output_buffer;
    output_buffer << "AT+CWJAP_CUR=\"" << SSID << "\",\"" << password << "\"\r\n";
    bridge.printf(output_buffer.str().c_str());
    //pc.printf(output_buffer.str().c_str());
}

void wifiAdapter::connect_to_tcp_server(std::string ip_adress, int port)
{
    ostringstream output_buffer;
    output_buffer << "AT+CIPSTART=\"TCP\",\"" << ip_adress << "\"," << port << "\r\n";
    bridge.printf(output_buffer.str().c_str());
    //printf(output_buffer.str().c_str());
}

void wifiAdapter::Send(std::string msg)
{
    ostringstream output_command_buffer;
    output_command_buffer << "AT+CIPSEND=" << msg.size() << "\r\n";
    bridge.printf(output_command_buffer.str().c_str());
    wait(2);
    ostringstream output_data_buffer;
    output_data_buffer << msg << "\r\n";
    bridge.printf(output_data_buffer.str().c_str());
    
}
        
