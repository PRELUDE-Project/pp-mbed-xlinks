/*
* author: Ryusuke Iwata
* date  : 2023/06/27
* note  : 
*/

#include "mbed.h"
#include "serviceServerStr.h"
RawSerial pc(USBTX, USBRX, 9600);
SERVICE_COMMON_STRUCT cmnStr;
SPI_MSG_DATA_STR spiMsgStr;
SERVICE_RF_XMT_DATA_FIFO_INIT_STRUCT rfXmtDataFifoInitStr;
SERVICE_RF_XMT_DATA_STRUCT rfXmtDataStr;
int cmd = 0,cmdflag = 0; //command variable
//getting command and flag
void commandget()
{
    cmd=pc.getc();
    cmdflag = 1;
}
//interrupting 
void receive(int cmd, int cmdflag)
{
    pc.attach(commandget,Serial::RxIrq);
}
//initialzing
void initialize()
{
    cmd = 0;
    cmdflag = 0;
}

void print_cmdList(){
    pc.printf("\r\n>> - Select commands below -\r\n");
    pc.printf("a: XLink initialize\r\n");
    pc.printf("b: 3.2.1 SERVICE_IDLE\r\n");
    pc.printf("c: 3.2.2 SERVICE_RESET\r\n");
    pc.printf("d: 3.2.3 SERVICE_IP_ADR\r\n");
    pc.printf("e: 3.2.4 SERVICE_IP_ADR_SAVE\r\n");
    pc.printf("f: 3.2.5 SERVICE_VERSION\r\n");
    pc.printf("g: 3.2.6 SERVICE_RF_XMT_DATA\r\n");
    pc.printf("h: 3.2.7 SERVICE_START_RF_RCV\r\n");
    pc.printf("i: 3.2.8 SERVICE_FEATURE_SELECT\r\n");
    pc.printf("j: 3.2.9 SERVICE_DEVICE_CONF\r\n");
    pc.printf("k: 3.2.10 SERVICE_HOUSE_KEEPING\r\n");
    pc.printf("l: 3.2.11 SERVICE_XPA_INF\r\n");
    pc.printf("m: 3.2.12 SERVICE_FLAGS_CONF\r\n");
    pc.printf("n: 3.2.13 SERVICE_DVBS2_CONF\r\n");
    pc.printf(">> Select the command:  ");
}

int main(){
    pc.printf("\r\n>> Checking XLink Start! \r\n");
    ss_str_init();
    print_cmdList();
    receive(cmd,cmdflag);
    while(1){
        // cmd = pc.getc();
        xlk_empty_send();
        if (cmdflag == 1) {
            switch(cmd){
                case 'a':
                    pc.printf("a\r\n");
                    xlk_rbuf_ptr = xlk_init();
                    ptr_printf(xlk_rbuf_ptr, 12);
                    break;
                case 'b':
                    // pc.printf("b\r\n");
                    // xlk_rbuf_ptr = xlk_service_idle(cmnStr);
                    // ptr_printf(xlk_rbuf_ptr, 40);
                    pc.printf("b\r\n");
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_SET, SERVICE_IDLE, 20);
                    ptr_printf(xlk_rbuf_ptr, 40);
                    break;
                case 'c':
                    pc.printf("c\r\n");
                    // xlk_rbuf_ptr = xlk_service_reset(cmnStr);
                    // ptr_printf(xlk_rbuf_ptr, 40);
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_SET, SERVICE_RESET, 20);
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
                    // xlk_rbuf_ptr = xlk_service_ip_adr(cmnStr);
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_GET, SERVICE_IP_ADR, 20);
                    ptr_printf(xlk_rbuf_ptr, 40);
                    break;
                case 'e':
                    break;
                case 'f':
                    pc.printf("f\r\n");
                    // xlk_rbuf_ptr = xlk_service_version(cmnStr);
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_GET, SERVICE_VERSION, 20);
                    ptr_printf(xlk_rbuf_ptr, 168);
                    break;
                case 'g':
                    pc.printf("g\r\n");
                    xlk_rbuf_ptr = xlk_service_rf_xmt_dummydata();
                    ptr_printf(xlk_rbuf_ptr, 44);
                    break;
                case 'h':
                    pc.printf("h\r\n");
                    xlk_rbuf_ptr = xlk_service_rf_xmt_data_fifoInit();
                    ptr_printf(xlk_rbuf_ptr, 80);
                    break;
                case 'i':
                    break;
                case 'j':
                    pc.printf("j\r\n");
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_GET, SERVICE_DEVICE_CONF, 20);
                    ptr_printf(xlk_rbuf_ptr, 136);
                    break;
                case 'k':
                    pc.printf("k\r\n");
                    xlk_rbuf_ptr = xlk_send_request(SERVICE_GET, SERVICE_HOUSE_KEEP, 20);
                    ptr_printf(xlk_rbuf_ptr, 138);
                    break;
                case 'l':
                    break;
                case 'm':
                    break;
                case 'n':
                    break;
                default:
                    break;
            }
            print_cmdList();
            initialize();
        }
    }
}