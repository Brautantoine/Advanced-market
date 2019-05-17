#include "lESP.hpp"

wifiAdapter::wifiAdapter(Serial& n_bridge): bridge(n_bridge)
{
    is_connected=false;
    bridge.attach(callback(this, &wifiAdapter::rx_interrupt), Serial::RxIrq);
    bridge.printf("AT+RST\r\n");
    
}

wifiAdapter::~wifiAdapter()
{
        
}
        
void wifiAdapter::connect_to_AP(std::string SSID, std::string password)
{
    buffer.clear();
    //Serial pc(USBTX, USBRX);
    ostringstream output_buffer;
    output_buffer << "AT+CWJAP_CUR=\"" << SSID << "\",\"" << password << "\"\r\n";
    bridge.printf(output_buffer.str().c_str());
    while(buffer.find("OK") == string::npos);
    wait(0.5);
    buffer.clear();
}

void wifiAdapter::connect_to_tcp_server(std::string ip_adress, int port)
{
    buffer.clear();
    ostringstream output_buffer;
    output_buffer << "AT+CIPSTART=\"TCP\",\"" << ip_adress << "\"," << port << "\r\n";
    bridge.printf(output_buffer.str().c_str());
    while(buffer.find("OK") == string::npos);
    wait(0.5);
    buffer.clear();
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

void wifiAdapter::rx_interrupt()
{
        if(bridge.readable()) 
        {
            char _buff(bridge.getc());
            pc.putc(_buff);
            buffer.push_back(_buff);
        }
}
        
