/*
* author: Ryusuke Iwata
* date  : 2023/06/27
* note  : 
*/

#include "mbed.h"
#include "serviceServerStr.h"
RawSerial pc(USBTX, USBRX, 9600);
SERVICE_COMMON_STRUCT cmnStr;

char cmd;
int main(){
    pc.printf("\r\n>> Checking XLink Start! \r\n");
    ss_str_init();
    while(1){
        pc.printf("\r\n>> - Select commands below -\r\n");
        pc.printf("a: XLink initialize\r\n");
        pc.printf("b: SERVICE_IDLE\r\n");
        pc.printf("c: SERVICE_RESET\r\n");
        pc.printf("d: SERVICE_IP_ADR\r\n");
        pc.printf("e: SERVICE_VERSION\r\n");
        pc.printf("x: SERVICE_HOUSE_KEEPING\r\n");
        pc.printf(">> Select the command: ");
        cmd = pc.getc();
        switch(cmd){
            case 'a':
                pc.printf("a\r\n");
                xlk_rbuf_ptr = xlk_init(cmnStr);
                ptr_printf(xlk_rbuf_ptr, 12);
                break;
            case 'b':
                pc.printf("b\r\n");
                xlk_rbuf_ptr = xlk_service_idle(cmnStr);
                ptr_printf(xlk_rbuf_ptr, 40);
                break;
            case 'c':
                pc.printf("c\r\n");
                xlk_rbuf_ptr = xlk_service_reset(cmnStr);
                ptr_printf(xlk_rbuf_ptr, 40);
                pc.printf(">>>> Reset Now");
                for(int i = 0; i < 15; i++){
                    pc.printf(".");
                    wait_ms(1000);
                }
                pc.printf("\r\n");
                break;
            case 'd':
                pc.printf("d\r\n");
                xlk_rbuf_ptr = xlk_service_ip_adr(cmnStr);
                ptr_printf(xlk_rbuf_ptr, 80);
                break;
            case 'e':
                pc.printf("e\r\n");
                xlk_rbuf_ptr = xlk_service_version(cmnStr);
                ptr_printf(xlk_rbuf_ptr, 240);
                break;
            case 'x':
                pc.printf("e\r\n");
                xlk_rbuf_ptr = service_request_get(cmnStr, SERVICE_GET, SERVICE_HOUSE_KEEP);
                ptr_printf(xlk_rbuf_ptr, 120);
                break;
        }
    }
}