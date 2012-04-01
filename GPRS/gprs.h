#ifndef _SIM300_H_
#define _SIM300_H_

#define DEBUG_SIM300CTL

#define SERNUM "0000001"
#define PROTOTOCOL "TCP"
#define IPADDR "111.164.239.141"
#define PORTNUM "1001"
#define TELNUM "13920931477"
#define CACHESIZE 5

#define GPRS_SIMPLE 1

typedef enum {
    INIT0,
    ECHO0,
    SIMCARD,
    CREG,
    INIT_FINISH_OK,

    GPRS_JH_S,
    GPRS_DK_S ,
    GPRS_QD_S ,
    GPRS_OPEN_FINISH,       /// GPRS 打开成功了

    TCPIP_IO_MODE,
    TCPIP_BJ_ADDR,
    TCPIP_CONNECT,
    TCP_IP_OK,
    TCP_IP_NO,
    TCP_IP_CONNECTING,

    GPRS_DEF_PDP_S,         /// 定义PDP场景
    GPRS_ACT_PDP_S,         /// 激活场景
    GPRS_ACT_PDP_S_RET,     /// 激活反馈
    GPRS_CMNET_APN_S,       /// 接入模式

    TRANSPARENT_MODE_START,
    TRANSPARENT_MODE_DEF,
    TRANSPARENT_MODE_ACT,
    TRANSPARENT_MODE_IOMODE,
    TRANSPARENT_MODE_TYPE,
    TRANSPARENT_MODE_CONFIG,
    TRANSPARENT_MODE_CONNECT,
    TRANSPARENT_MODE_CONNECT_RET,
    TRANSPARENT_MODE_TPS,
    TM_CON_OK,
    TM_CON_NO,
    TM_OK,
    TM_NO,

    TCPIP_CLOSE_START,
    TCPIP_CLOSE_DEF,
    TCPIP_CLOSE_ETCPIP,
    TCP_IP_CLOSE_OK,
    TCP_IP_CLOSE_NO,

    MG323_TP_START_S,
    MG323_TP_NETTYPE_S,
    MG323_TP_APNTYPE_S,
    MG323_TP_CONID_S,
    MG323_TP_SERTYPE_S,
    MG323_TP_CONNECT_S,
    MG323_TP_NETOPEN_S,
    MG323_TP_TRANS_S,
    MG323_TP_TRANS_RET,
    MG323_TP_OK_S,
    MG323_TP_NO_S,

    GSM_SEND_TEXT_S,
    GSM_MSG_MODE_S,
    GSM_CHAR_MODE_S,
    GSM_TEXT_MODE_FINISH,


    GSM_SEND_TEXT_CN_S,
    GSM_MSG_MODE_CN_S,
    GSM_CHAR_MODE_CN_S,

    SMS_READ_MESSAGE_START_S,
    SMS_READ_CONTENT_S,
    READ_SMS_FINISH,

    GSM_AT_MODE_START,
    GSM_AT_MODE_CMD,
    GSM_AT_MODE_FINISH,

    READ_SOCKET_DATA_START,
    READ_SOCKET_DATA_CMD,
    READ_SOCKET_DATA_FINISH

}SIM300_STATUS ;

extern void init_gprs_mode(void);
extern unsigned char is_gprs_mode_status_ok(void);

extern void open_gprs_simple(void);//gprs
extern void creat_tcp(void);

extern SIM300_STATUS current_status ;
extern void send_gprs_data(char * buf ,unsigned int size);
extern void send_gsm_text(unsigned char * buf , unsigned int count);
extern void read_the_index_sms(void);
extern int check_num_of_sms(void);
extern void start_gprs_mode(void);

///tp
extern void creat_tp_mode(void);
extern void creat_mg323_tp_mode(void);
extern void send_tp_data(char * buf , unsigned int count);
extern void send_tp_data_to_service(void);
extern void send_message(char *msg);
//read data


extern void read_data(void);

//chinese
extern void send_gsm_text_chinese(unsigned char * buf, int lenstr , unsigned int count);
extern void init_sms_to_send_chinese(void);
extern void send_key_status(void);

extern char msg1[];
extern char msg2[];
extern char msg3[];
extern char msg4[];
extern unsigned char is_gprs_mode_ok;                  //gprs start succeed


//妈妈:我着凉了，快盖上小被子，别凉着肚肚噢!《电脑小保姆，宝宝健康的保护神》90
#define msg11 "0891683108200205F011000B815129021411F50008A74C59885988003A6211774051C94E86FF0C5FEB76D64E0A5C0F88AB5B50FF0C522B51C97740809A809A56620021300A753581115C0F4FDD59C6FF0C5B9D5B9D50655EB776844FDD62A4795E300B"
//妈妈:快来呀，宝宝想妈妈了！电脑小保姆在陪我玩噢！64
#define msg22 "0891683108200205F011000B815129021411F50008A73259885988003A5FEB67655440FF0C5B9D5B9D60F3598859884E86FF01753581115C0F4FDD59C65728966A621173A95662FF01"
//妈妈:我翻身了，小心宝宝掉下床噢！   ～～为宝宝昼夜值班永不休息！（智能电脑小保姆）100
#define msg33 "0891683108200205F011000B815129021411F50008A75659885988003A62117FFB8EAB4E86FF0C5C0F5FC35B9D5B9D63894E0B5E8A5662FF01002000200020FF5EFF5E4E3A5B9D5B9D663C591C503C73ED6C384E0D4F11606FFF01FF08667A80FD753581115C0F4FDD59C6FF09"
//妈妈:我尿尿了，好湿阿，快给宝宝换窝啦！   －－－电脑小保姆，时刻呵护宝宝的健康！ 98
#define msg44 "0891683108200205F011000B815129021411F50008A75459885988003A62115C3F5C3F4E86FF0C597D6E7F963FFF0C5FEB7ED95B9D5B9D63627A9D5566FF01002000200020FF0DFF0DFF0D753581115C0F4FDD59C6FF0C65F6523B547562A45B9D5B9D768450655EB7FF01"

extern int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
#endif
