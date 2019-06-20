#include "lESP.hpp"

wifiAdapter::wifiAdapter(Serial& n_bridge): bridge(n_bridge)
{
    is_connected=false;
    bridge.attach(callback(this, &wifiAdapter::rx_interrupt), Serial::RxIrq);
    bridge.printf("AT+RST\r\n");
    
    correspondance_table.push_back(ligne_panier("STM32F7","A1","4a9acc",1));
    correspondance_table.push_back(ligne_panier("Aston Martin de James Bond","D8","f65eb4",1));
    correspondance_table.push_back(ligne_panier("Generateur dd alcool infini","C9","cb091d",1));
    correspondance_table.push_back(ligne_panier("Kebab sans oignon","B4","db2915",1));
    correspondance_table.push_back(ligne_panier("Robe de soiree","C3","aecd97",1));
    correspondance_table.push_back(ligne_panier("Guitare electro acoustique","C8","cbab97",1));
    correspondance_table.push_back(ligne_panier("Pc MSI","C1","db6793",1));
    correspondance_table.push_back(ligne_panier("Jus de fruit","B2","65dd17",1));
    correspondance_table.push_back(ligne_panier("Saturne 5","E7","0e3120",1));
    correspondance_table.push_back(ligne_panier("Pompe a vide 12V","A2","9d74b9",1));
    
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
    while((buffer.find("OK") == string::npos) && (buffer.find("ERROR") == string::npos));
    wait(0.5);
    buffer.clear();
}

void wifiAdapter::connect_to_tcp_server(std::string ip_adress, int port)
{
    buffer.clear();
    ostringstream output_buffer;
    output_buffer << "AT+CIPSTART=\"TCP\",\"" << ip_adress << "\"," << port << "\r\n";
    bridge.printf(output_buffer.str().c_str());
    while((buffer.find("OK") == string::npos) && (buffer.find("ERROR") == string::npos));
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

void wifiAdapter::get_card()
{
 extern std::queue<ligne_panier> panier;
 buffer.clear();
 std::string output_command_buffer;
 output_command_buffer.push_back(0x10);
    output_command_buffer.push_back(0x01);
    output_command_buffer.append("SENT FROM STM32");
        Send(output_command_buffer);   
        size_t recv_pos = string::npos;
        while(recv_pos == string::npos)recv_pos = buffer.find("+IPD");
        wait(0.4);
        printf("card receipted : %s\n",buffer.substr(recv_pos+8).c_str());
        
        while(!panier.empty())
            panier.pop();
        
        // //
        std::string id_card(buffer.substr(recv_pos+10,2));
        printf("id panier : %s\n",id_card.c_str());
        std::string qt_card(buffer.substr(recv_pos+12,2));
        printf("nombre article : %s\n",qt_card.c_str());
        int corresp(0);
        for(int i=0;i<atoi(qt_card.c_str());i++)
        {
            printf("code article %d : %s / qt : %d\n",i,buffer.substr(recv_pos+14+(i*10),6).c_str(),atoi(buffer.substr(recv_pos+21+(i*10)).c_str()));
            corresp = find_in_table(buffer.substr(recv_pos+14+(i*10),6));
            panier.push(ligne_panier(correspondance_table.at(corresp).article,correspondance_table.at(corresp).emplacement,correspondance_table.at(corresp).unique_id,atoi(buffer.substr(recv_pos+21+(i*10)).c_str())));
        }
        // //
        
        output_command_buffer.clear();
        output_command_buffer.push_back(0x10);
        output_command_buffer.push_back(0x01);
        output_command_buffer.append(qt_card);
        Send(output_command_buffer);
        
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
        
int wifiAdapter::find_in_table(std::string key)
{
 int ret = 0;
 for(int i=0; i< correspondance_table.size();i++)
 {
    if(!(correspondance_table.at(i).unique_id.compare(key)))
        ret = i;    
 }   
 return ret;
}
