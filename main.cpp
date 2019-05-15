#include "mbed.h"
 
Serial pc(USBTX, USBRX); // tx, rx
Serial device(PA_9, PA_10);  // tx, rx
DigitalOut led1(LED1);
 
int main() {
    //device.printf("AT+GMR");
    //wait(2);
    device.printf("AT+GMR\n");
    wait(2);
    device.printf("AT+GMR\r\n");
    while(1) {
        if(pc.readable()) {
            char buff = pc.getc();
            //pc.printf(" buff = %X",buff);
            if(buff == '\r')
            {
                device.putc('\r');
                device.putc('\n');
                led1 = !led1;
            }
            else
                device.putc(buff);
        }
        if(device.readable()) {
            pc.putc(device.getc());
        }
    }
}
