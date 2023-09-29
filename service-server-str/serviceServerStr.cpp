/*******************************************************************
* author: gun-charlie
* date. : 2023/06/27
* ref.  : https://drive.google.com/file/d/1vgPmEcOCBbkmtCJrukcfLsusCXM-gzej/view
*******************************************************************/

#include "serviceServerStr.h"
#include <cstdint>
SPI spi(COM_XLINK_MOSI, COM_XLINK_MISO, COM_XLINK_CLK);
DigitalOut cs(COM_XLINK_CS);
U8* xlk_rbuf_ptr = new U8[2200];

void ss_str_init(void){
    spi.frequency(1000000);
    spi.format(8, 1);
    cmnStr.product_key[0] = PRODUCT_KEY_A;
    cmnStr.product_key[1] = PRODUCT_KEY_B;
    cmnStr.product_key[2] = PRODUCT_KEY_C;
    cmnStr.product_key[3] = PRODUCT_KEY_D;
    cmnStr.service_event  = 0x00000000;
}

U8* xlk_empty_send(void){
    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    xlk_rbuf_ptr[4] = spi.write(0x00);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(0x00);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 0; i < 2188; i++) {
        xlk_rbuf_ptr[12+i] = spi.write(0);
    }
    cs = 1;
    wait_ms(2);
    return xlk_rbuf_ptr;
}

U8* xlk_empty_send_test(void){
    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    xlk_rbuf_ptr[4] = spi.write(0x18);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(0x0A);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 0; i < 2188; i++) {
        xlk_rbuf_ptr[12+i] = spi.write(0);
    }
    cs = 1;
    wait_ms(2);
    return xlk_rbuf_ptr;
}

U8* xlk_init(struct cmnStr){
    //sending empty data to synchronize w/ XLink
    while(1){
        xlk_rbuf_ptr = xlk_empty_send();
        if(xlk_rbuf_ptr[0] == uint8_t(START_SEQUENCE >> 24)){
            break;
        }
    }
    return xlk_rbuf_ptr;
}

U8* xlk_service_idle(struct cmnStr){
    cmnStr.service_event  = SERVICE_SET | SERVICE_IDLE;
    U16 length = 20;
    int flame_length = length + 16; 

    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    // SPI_MSG_DATA_STRUCT Header
    // uint8_t length_bottom = length >> 8;
    // uint8_t length_upper  = length;
    // xlk_rbuf_ptr[4] = spi.write(length_bottom);
    // xlk_rbuf_ptr[5] = spi.write(length_upper);
    xlk_rbuf_ptr[4] = spi.write(0x14);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(SPI_SERVICE);
    // xlk_rbuf_ptr[6] = spi.write(0x0A);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(cmnStr.product_key[0]));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(cmnStr.product_key[0] >> 8));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(cmnStr.product_key[0] >> 16));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(cmnStr.product_key[0] >> 24));

    xlk_rbuf_ptr[12] = spi.write(uint8_t(cmnStr.product_key[1]));
    xlk_rbuf_ptr[13] = spi.write(uint8_t(cmnStr.product_key[1] >> 8));
    xlk_rbuf_ptr[14] = spi.write(uint8_t(cmnStr.product_key[1] >> 16));
    xlk_rbuf_ptr[15] = spi.write(uint8_t(cmnStr.product_key[1] >> 24));

    xlk_rbuf_ptr[16] = spi.write(uint8_t(cmnStr.product_key[2]));
    xlk_rbuf_ptr[17] = spi.write(uint8_t(cmnStr.product_key[2] >> 8));
    xlk_rbuf_ptr[18] = spi.write(uint8_t(cmnStr.product_key[2] >> 16));
    xlk_rbuf_ptr[19] = spi.write(uint8_t(cmnStr.product_key[2] >> 24));

    xlk_rbuf_ptr[20] = spi.write(uint8_t(cmnStr.product_key[3]));
    xlk_rbuf_ptr[21] = spi.write(uint8_t(cmnStr.product_key[3] >> 8));
    xlk_rbuf_ptr[22] = spi.write(uint8_t(cmnStr.product_key[3] >> 16));
    xlk_rbuf_ptr[23] = spi.write(uint8_t(cmnStr.product_key[3] >> 24));

    xlk_rbuf_ptr[24] = spi.write(uint8_t(cmnStr.service_event));
    xlk_rbuf_ptr[25] = spi.write(uint8_t(cmnStr.service_event >> 8));
    xlk_rbuf_ptr[26] = spi.write(uint8_t(cmnStr.service_event >> 16));
    xlk_rbuf_ptr[27] = spi.write(uint8_t(cmnStr.service_event >> 24));

    // SPI_MSG_DATA_STRUCT footer
    xlk_rbuf_ptr[28] = spi.write(0x00);
    xlk_rbuf_ptr[29] = spi.write(0x00);
    xlk_rbuf_ptr[30] = spi.write(0x00);
    xlk_rbuf_ptr[31] = spi.write(0x00);

    xlk_rbuf_ptr[32] = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[33] = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[34] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[35] = spi.write(uint8_t(END_SEQUENCE));
    for (int i = 1; i < (2200 - flame_length); i++) {
        xlk_rbuf_ptr[flame_length+i] = spi.write(0);
    }
    cs = 1;

    uint8_t current_ptr12 = xlk_rbuf_ptr[12];
    while(1){
        if(current_ptr12 == uint8_t(cmnStr.product_key[1])){
            break;
        }
        xlk_rbuf_ptr  = xlk_empty_send();
        current_ptr12 = xlk_rbuf_ptr[12];
    }
    return xlk_rbuf_ptr;
}

U8*  xlk_service_reset(struct cmnStr){
    cmnStr.service_event  = SERVICE_SET | SERVICE_RESET;
    U16 length = 20;
    int flame_length = length + 16; 
    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    xlk_rbuf_ptr[4] = spi.write(0x14);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(SPI_SERVICE);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(cmnStr.product_key[0]));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(cmnStr.product_key[0] >> 8));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(cmnStr.product_key[0] >> 16));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(cmnStr.product_key[0] >> 24));

    xlk_rbuf_ptr[12] = spi.write(uint8_t(cmnStr.product_key[1]));
    xlk_rbuf_ptr[13] = spi.write(uint8_t(cmnStr.product_key[1] >> 8));
    xlk_rbuf_ptr[14] = spi.write(uint8_t(cmnStr.product_key[1] >> 16));
    xlk_rbuf_ptr[15] = spi.write(uint8_t(cmnStr.product_key[1] >> 24));

    xlk_rbuf_ptr[16] = spi.write(uint8_t(cmnStr.product_key[2]));
    xlk_rbuf_ptr[17] = spi.write(uint8_t(cmnStr.product_key[2] >> 8));
    xlk_rbuf_ptr[18] = spi.write(uint8_t(cmnStr.product_key[2] >> 16));
    xlk_rbuf_ptr[19] = spi.write(uint8_t(cmnStr.product_key[2] >> 24));

    xlk_rbuf_ptr[20] = spi.write(uint8_t(cmnStr.product_key[3]));
    xlk_rbuf_ptr[21] = spi.write(uint8_t(cmnStr.product_key[3] >> 8));
    xlk_rbuf_ptr[22] = spi.write(uint8_t(cmnStr.product_key[3] >> 16));
    xlk_rbuf_ptr[23] = spi.write(uint8_t(cmnStr.product_key[3] >> 24));

    xlk_rbuf_ptr[24] = spi.write(uint8_t(cmnStr.service_event));
    xlk_rbuf_ptr[25] = spi.write(uint8_t(cmnStr.service_event >> 8));
    xlk_rbuf_ptr[26] = spi.write(uint8_t(cmnStr.service_event >> 16));
    xlk_rbuf_ptr[27] = spi.write(uint8_t(cmnStr.service_event >> 24));

    xlk_rbuf_ptr[28] = spi.write(0x00);
    xlk_rbuf_ptr[29] = spi.write(0x00);
    xlk_rbuf_ptr[30] = spi.write(0x00);
    xlk_rbuf_ptr[31] = spi.write(0x00);

    xlk_rbuf_ptr[32] = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[33] = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[34] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[35] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 1; i < (2200 - flame_length); i++) {
        xlk_rbuf_ptr[flame_length+i] = spi.write(0);
    }
    cs = 1;

    uint8_t current_ptr12 = xlk_rbuf_ptr[12];
    while(1){
        if(current_ptr12 == uint8_t(cmnStr.product_key[1])){
            break;
        }
        xlk_rbuf_ptr  = xlk_empty_send();
        current_ptr12 = xlk_rbuf_ptr[12];
    }

    return xlk_rbuf_ptr;
}

U8* xlk_service_ip_adr(struct cmnStr){
    cmnStr.service_event  = SERVICE_GET | SERVICE_IP_ADR;
    U16 length = 20;
    int flame_length = length + 16; 

    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    // SPI_MSG_DATA_STRUCT Header
    // uint8_t length_bottom = length >> 8;
    // uint8_t length_upper  = length;
    // xlk_rbuf_ptr[4] = spi.write(length_bottom);
    // xlk_rbuf_ptr[5] = spi.write(length_upper);
    xlk_rbuf_ptr[4] = spi.write(0x14);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(SPI_SERVICE);
    // xlk_rbuf_ptr[6] = spi.write(0x0A);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(cmnStr.product_key[0]));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(cmnStr.product_key[0] >> 8));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(cmnStr.product_key[0] >> 16));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(cmnStr.product_key[0] >> 24));

    xlk_rbuf_ptr[12] = spi.write(uint8_t(cmnStr.product_key[1]));
    xlk_rbuf_ptr[13] = spi.write(uint8_t(cmnStr.product_key[1] >> 8));
    xlk_rbuf_ptr[14] = spi.write(uint8_t(cmnStr.product_key[1] >> 16));
    xlk_rbuf_ptr[15] = spi.write(uint8_t(cmnStr.product_key[1] >> 24));

    xlk_rbuf_ptr[16] = spi.write(uint8_t(cmnStr.product_key[2]));
    xlk_rbuf_ptr[17] = spi.write(uint8_t(cmnStr.product_key[2] >> 8));
    xlk_rbuf_ptr[18] = spi.write(uint8_t(cmnStr.product_key[2] >> 16));
    xlk_rbuf_ptr[19] = spi.write(uint8_t(cmnStr.product_key[2] >> 24));

    xlk_rbuf_ptr[20] = spi.write(uint8_t(cmnStr.product_key[3]));
    xlk_rbuf_ptr[21] = spi.write(uint8_t(cmnStr.product_key[3] >> 8));
    xlk_rbuf_ptr[22] = spi.write(uint8_t(cmnStr.product_key[3] >> 16));
    xlk_rbuf_ptr[23] = spi.write(uint8_t(cmnStr.product_key[3] >> 24));

    xlk_rbuf_ptr[24] = spi.write(uint8_t(cmnStr.service_event));
    xlk_rbuf_ptr[25] = spi.write(uint8_t(cmnStr.service_event >> 8));
    xlk_rbuf_ptr[26] = spi.write(uint8_t(cmnStr.service_event >> 16));
    xlk_rbuf_ptr[27] = spi.write(uint8_t(cmnStr.service_event >> 24));

    // SPI_MSG_DATA_STRUCT footer
    xlk_rbuf_ptr[28] = spi.write(0x00);
    xlk_rbuf_ptr[29] = spi.write(0x00);
    xlk_rbuf_ptr[30] = spi.write(0x00);
    xlk_rbuf_ptr[31] = spi.write(0x00);

    xlk_rbuf_ptr[32] = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[33] = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[34] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[35] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 1; i < (2200 - flame_length); i++) {
        xlk_rbuf_ptr[flame_length+i] = spi.write(0);
    }
    cs = 1;

    uint8_t current_ptr12 = xlk_rbuf_ptr[12];
    uint8_t loop_count = 0;
    while(1){
        if(current_ptr12 == uint8_t(cmnStr.product_key[1])){
            break;
        }else if(loop_count > 100){
            break;
        }
        xlk_rbuf_ptr  = xlk_empty_send();
        current_ptr12 = xlk_rbuf_ptr[12];
        loop_count++;
    }
    return xlk_rbuf_ptr;
}

U8*  xlk_service_version(struct cmnStr){
    cmnStr.service_event  = SERVICE_GET | SERVICE_VERSION;
    U16 length = 20;
    int flame_length = length + 16; 
    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    xlk_rbuf_ptr[4] = spi.write(0x14);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(0x0A);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(cmnStr.product_key[0]));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(cmnStr.product_key[0] >> 8));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(cmnStr.product_key[0] >> 16));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(cmnStr.product_key[0] >> 24));

    xlk_rbuf_ptr[12] = spi.write(uint8_t(cmnStr.product_key[1]));
    xlk_rbuf_ptr[13] = spi.write(uint8_t(cmnStr.product_key[1] >> 8));
    xlk_rbuf_ptr[14] = spi.write(uint8_t(cmnStr.product_key[1] >> 16));
    xlk_rbuf_ptr[15] = spi.write(uint8_t(cmnStr.product_key[1] >> 24));

    xlk_rbuf_ptr[16] = spi.write(uint8_t(cmnStr.product_key[2]));
    xlk_rbuf_ptr[17] = spi.write(uint8_t(cmnStr.product_key[2] >> 8));
    xlk_rbuf_ptr[18] = spi.write(uint8_t(cmnStr.product_key[2] >> 16));
    xlk_rbuf_ptr[19] = spi.write(uint8_t(cmnStr.product_key[2] >> 24));

    xlk_rbuf_ptr[20] = spi.write(uint8_t(cmnStr.product_key[3]));
    xlk_rbuf_ptr[21] = spi.write(uint8_t(cmnStr.product_key[3] >> 8));
    xlk_rbuf_ptr[22] = spi.write(uint8_t(cmnStr.product_key[3] >> 16));
    xlk_rbuf_ptr[23] = spi.write(uint8_t(cmnStr.product_key[3] >> 24));

    xlk_rbuf_ptr[24] = spi.write(uint8_t(cmnStr.service_event));
    xlk_rbuf_ptr[25] = spi.write(uint8_t(cmnStr.service_event >> 8));
    xlk_rbuf_ptr[26] = spi.write(uint8_t(cmnStr.service_event >> 16));
    xlk_rbuf_ptr[27] = spi.write(uint8_t(cmnStr.service_event >> 24));

    xlk_rbuf_ptr[28] = spi.write(0x00);
    xlk_rbuf_ptr[29] = spi.write(0x00);
    xlk_rbuf_ptr[30] = spi.write(0x00);
    xlk_rbuf_ptr[31] = spi.write(0x00);

    xlk_rbuf_ptr[32] = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[33] = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[34] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[35] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 1; i < (2200 - flame_length); i++) {
        xlk_rbuf_ptr[flame_length+i] = spi.write(0);
    }
    cs = 1;

    uint8_t current_ptr12 = xlk_rbuf_ptr[12];
    uint8_t loop_count = 0;
    while(1){
        if(current_ptr12 == uint8_t(cmnStr.product_key[1])){
            break;
        }else if(loop_count > 100){
            break;
        }
        xlk_rbuf_ptr  = xlk_empty_send();
        current_ptr12 = xlk_rbuf_ptr[12];
        loop_count++;
    }
    return xlk_rbuf_ptr;
}

void ptr_printf(U8* ptr, U32 ptr_length){
    printf("           1    2    3    4    5    6    7    8    9   10   11   12");
    for (int i = 0; i < ptr_length; i++) {
        if(i%12 == 0){
            printf("\r\nptr[%02d] ",i);
        }
        printf("0x%02x ", ptr[i]);
    }
    printf("\r\n");
}

U8* service_request_get(struct cmnStr, U32 se_1, U32 se_2){
    cmnStr.service_event  = se_1 | se_2;
    int length = 36; 
    cs = 0;
    xlk_rbuf_ptr[0] = spi.write(uint8_t(START_SEQUENCE >> 24));
    xlk_rbuf_ptr[1] = spi.write(uint8_t(START_SEQUENCE >> 16));
    xlk_rbuf_ptr[2] = spi.write(uint8_t(START_SEQUENCE >> 8));
    xlk_rbuf_ptr[3] = spi.write(uint8_t(START_SEQUENCE));

    xlk_rbuf_ptr[4] = spi.write(0x14);
    xlk_rbuf_ptr[5] = spi.write(0x00);
    xlk_rbuf_ptr[6] = spi.write(0x0A);
    xlk_rbuf_ptr[7] = spi.write(0x00);

    xlk_rbuf_ptr[8]  = spi.write(uint8_t(cmnStr.product_key[0]));
    xlk_rbuf_ptr[9]  = spi.write(uint8_t(cmnStr.product_key[0] >> 8));
    xlk_rbuf_ptr[10] = spi.write(uint8_t(cmnStr.product_key[0] >> 16));
    xlk_rbuf_ptr[11] = spi.write(uint8_t(cmnStr.product_key[0] >> 24));

    xlk_rbuf_ptr[12] = spi.write(uint8_t(cmnStr.product_key[1]));
    xlk_rbuf_ptr[13] = spi.write(uint8_t(cmnStr.product_key[1] >> 8));
    xlk_rbuf_ptr[14] = spi.write(uint8_t(cmnStr.product_key[1] >> 16));
    xlk_rbuf_ptr[15] = spi.write(uint8_t(cmnStr.product_key[1] >> 24));

    xlk_rbuf_ptr[16] = spi.write(uint8_t(cmnStr.product_key[2]));
    xlk_rbuf_ptr[17] = spi.write(uint8_t(cmnStr.product_key[2] >> 8));
    xlk_rbuf_ptr[18] = spi.write(uint8_t(cmnStr.product_key[2] >> 16));
    xlk_rbuf_ptr[19] = spi.write(uint8_t(cmnStr.product_key[2] >> 24));

    xlk_rbuf_ptr[20] = spi.write(uint8_t(cmnStr.product_key[3]));
    xlk_rbuf_ptr[21] = spi.write(uint8_t(cmnStr.product_key[3] >> 8));
    xlk_rbuf_ptr[22] = spi.write(uint8_t(cmnStr.product_key[3] >> 16));
    xlk_rbuf_ptr[23] = spi.write(uint8_t(cmnStr.product_key[3] >> 24));

    xlk_rbuf_ptr[24] = spi.write(uint8_t(cmnStr.service_event));
    xlk_rbuf_ptr[25] = spi.write(uint8_t(cmnStr.service_event >> 8));
    xlk_rbuf_ptr[26] = spi.write(uint8_t(cmnStr.service_event >> 16));
    xlk_rbuf_ptr[27] = spi.write(uint8_t(cmnStr.service_event >> 24));

    xlk_rbuf_ptr[28] = spi.write(0x00);
    xlk_rbuf_ptr[29] = spi.write(0x00);
    xlk_rbuf_ptr[30] = spi.write(0x00);
    xlk_rbuf_ptr[31] = spi.write(0x00);

    xlk_rbuf_ptr[32] = spi.write(uint8_t(END_SEQUENCE >> 24));
    xlk_rbuf_ptr[33] = spi.write(uint8_t(END_SEQUENCE >> 16));
    xlk_rbuf_ptr[34] = spi.write(uint8_t(END_SEQUENCE >> 8));
    xlk_rbuf_ptr[35] = spi.write(uint8_t(END_SEQUENCE));

    for (int i = 0; i < (2200 - length); i++) {
        xlk_rbuf_ptr[length+i] = spi.write(0);
    }
    cs = 1;
    return xlk_rbuf_ptr;
}