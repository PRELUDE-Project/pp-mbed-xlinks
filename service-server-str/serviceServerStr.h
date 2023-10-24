
/*******************************************************************
* author: gun-charlie
* date. : 2023/06/27
* ref.  : https://drive.google.com/file/d/1vgPmEcOCBbkmtCJrukcfLsusCXM-gzej/view
*******************************************************************/

#ifndef SERVICE_SERVER_STR_H
#define SERVICE_SERVER_STR_H
/*******************************************************************/
/* Including Files                                                 */
/*******************************************************************/
#include "mbed.h"

/*******************************************************************/
/* Local pre-processor symbols/macros ('#define')                  */
/*******************************************************************/
// 1.4 Terms and Definitions
#define U32 uint32_t //!< define a unsigned 32 bit datatype
#define U16 uint16_t //!< define a unsigned 16 bit datatype
#define U8  uint8_t  //!< define a unsigned 8 bit datatype
#define S32 int32_t  //!< define a signed 32 bit datatype
#define S16 int16_t  //!< define a signed 16 bit datatype
#define S8  int8_t   //!< define a signed 8 bit datatype

#define COM_XLINK_MOSI p5
#define COM_XLINK_MISO p6
#define COM_XLINK_CLK  p7
#define COM_XLINK_CS   p8

//3.1 Overview on the Service Server Structures
#define SERVICE_SERVER_PORT 51003
#define SERVICE_LIVE_TIME   600

/* service requests and response flags */
#define SERVICE_XMT_DATA_STOP 0x04000000 /* Stop the XmtData Transfer */
#define SERVICE_XMT_DATA_RUN  0x08000000 /* Run the XmtData Transfer after stop */
#define SERVICE_GET           0x10000000 /* read data request (GET) */
#define SERVICE_SET           0x20000000 /* write data request (SET) */
#define SERVICE_ACK           0x40000000 /* acknowledged request response (ACK) */
#define SERVICE_NAK           0x80000000 /* not acknowledged request */
#define SERVICE_BUSY          0xC0000000 /* BUSY response */
/* service events */
/* read only = r; write only = w; read & write = rw */
#define SERVICE_IDLE           0x00000000 /* w */
#define SERVICE_DEVICE_CONF    0x00000001 /* rw */
#define SERVICE_FEATURE_SELECT 0x00000002 /* r */
#define SERVICE_VERSION        0x00000003 /* rw */
#define SERVICE_HOUSE_KEEP     0x00000004 /* r */
#define SERVICE_IP_ADR         0x00000009 /* w */
#define SERVICE_IP_ADR_SAVE    0x0000000A /* w */
#define SERVICE_START_RF_RCV   0x0000000E /* w */
#define SERVICE_RF_XMT_DATA    0x0000000F /* w */
#define SERVICE_RESET          0x009AE41B /* w */

#define START_SEQUENCE 0x53504953 // SPIS
#define END_SEQUENCE   0x53504945 // SPIE
#define PRODUCT_KEY_A  0x8a75ed7b
#define PRODUCT_KEY_B  0x6cd6cfb8
#define PRODUCT_KEY_C  0x81552300
#define PRODUCT_KEY_D  0x42cc7396

#define SPI_NODATA  0x00 // 0
#define SPI_SERVICE 0x0A // 10
#define SPI_DATA    0x0B // 11
/*******************************************************************/
/* Global variables declared here                                  */
/*******************************************************************/
extern U8 xlk_rbuf[2200];
extern U8* xlk_rbuf_ptr;
/*******************************************************************/
/* Local type definition                                           */
/*******************************************************************/
/* define the common SERVICE STRUCTURE */
typedef struct cmnStr
{
/*----------------------- U32 boundary [0] ------------------------ Header start */
    U32 product_key[4]; //!< XLINK device specific key
    U32 service_event; //!< type of the service request
/*----------------------- U32 boundary [5] ------------------------ Header end */
    U32 data[2056]; //!< max. 2056 byte */
} SERVICE_COMMON_STRUCT;

#define MAX_DATA_SPI_MSG 2192
typedef struct spiMsgStr
{
    U16 lengh;
    U8  ident;
    U8  from;
    U8  messageBuffer[MAX_DATA_SPI_MSG];
} SPI_MSG_DATA_STR;

typedef struct idleStr
{
/*----------------------- U32 boundary [0] -------------------------*/
    U32 product_key[4]; //!< XLINK device specific key
    U32 service_event; //!< type of the service request
/*----------------------- U32 boundary [5] -------------------------*/
} SERVICE_IDLE_STRUCT; /* size: 20 bytes (5 WORDs) */

typedef struct rfXmtDataStr
{
/*----------------------- U32 boundary [0] -------------------------*/
    U32 product_key[4]; //!< XLINK device specific key
    U32 service_event; //!< type of the service request Here: 0x2000000F
/*----------------------- U32 boundary [5] -------------------------*/
    U32 PktType : 2; //!< type of Packet:
    #define PKT_TRANSFER_FRM 2
    U32 ErrInf : 5; //!< error information, not used here
    U32 PktLen :12; //!< number of bytes in Payload field, must be equal to Transfer Frame Size: SERVICE_DEVICE_CONF_STRUCT.XMT_CTR3.TrFrmSz
    U32 EthFirst : 1; //!< not used
    U32 EthLast : 1; //!< not used
    U32 EthNumByte:11; //!< not used
    U32 ControlBits;
/*----------------------- U32 boundary [6] -------------------------*/
    U32 MoreInfo; //!< Magic Number(TCP) or UDP packet counter
    #define SERVICE_XMT_MAGIC 0xDEADAFFE
/*----------------------- U32 boundary [7] -------------------------*/
    U8 Payload[2048]; //!< Payload Data (1...2048 depending on PktLen)
} SERVICE_RF_XMT_DATA_STRUCT; /* Service Server Request */
/*min- size: 32 bytes (8 WORDs) */

typedef struct rfXmtDataFifoInitStr
{
/*----------------------- U32 boundary [0] -------------------------*/
    U32 product_key[4]; //!< XLINK device specific key
    U32 service_event; //!< type of the service request Here: 0x2000000F
/*----------------------- U32 boundary [5] -------------------------*/
    U32 ControlBits; //!< TxFiFo init/flush:
    #define XMT_FIFO_INIT 0xBABEDADE
/*----------------------- U32 boundary [6] -------------------------*/
    U32 MoreInfo; //!< Magic Number:
    #define SERVICE_XMT_MAGIC 0xDEADAFFE
/*----------------------- U32 boundary [7] -------------------------*/
    U8 Payload[2048]; //!< Payload Data (1...2048 depending on PktLen)
} SERVICE_RF_XMT_DATA_FIFO_INIT_STRUCT; /* Service Server Request FIFO init */
/* size: 28 bytes (7 WORDs) */
/*******************************************************************/
/* function prototypes                                             */
/*******************************************************************/
void ss_str_init(void);
U8* xlk_empty_send(void);
U8* xlk_init(void);
U8* xlk_send_request(U32 service_request, U32 service_event, U16 spi_msg_length);
U8* xlk_service_idle(struct cmnStr);
U8* xlk_service_reset(struct cmnStr);
U8* xlk_service_ip_adr(struct cmnStr);
U8* xlk_service_version(struct cmnStr);
void ptr_printf(U8* ptr, U32 ptr_length);
U8* xlk_service_rf_xmt_data_fifoInit(void);
U8* xlk_service_rf_xmt_dummydata(void);
extern struct cmnStr cmnStr;
extern struct spiMsgStr spiMsgStr;
extern struct rfXmtDataFifoInitStr rfXmtDataFifoInitStr;
extern struct rfXmtDataStr rfXmtDataStr;
extern RawSerial pc;
#endif