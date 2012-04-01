#ifndef __STM32_USART_H
#define __STM32_USART_H
#include "../GPRS/gprs.h"
extern int err;

typedef unsigned char  u8;
typedef unsigned short u16;

#define BUFLEN 100
#define PUT_GPRS send_string_uart3
#define PUT send_string_uart1
#define PUT_PC send_string_uart1
#define MAX_ERROR_COUNT 30000

extern char temp_line[BUFLEN+1];
extern char isFULL;
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];  //长度101    多申请一个字符保证最后一个字符永远是'0'
    unsigned int index ;  //做计数器用
}UART_BUF;

typedef struct _TP_BUF
{
    char buf[CACHESIZE];                // 多申请一个字符保证最后一个字符永远是'0'
    unsigned int index ;
}TP_BUF;

typedef struct _GPS_FLAG
{
    unsigned char gps_analysis;         //GPS 数据分析中
    unsigned char gps_receiver;         //接收中
    unsigned char gps_stop;             //接收完毕
}GPS_FLAG;


typedef struct _CONFIG_FLAG
{
    unsigned char config_analysis;     //配置数据 数据分析中
    unsigned char config_receiver;     //接收中
    unsigned char config_stop;         //接收完毕
}CONFIG_FLAG;


typedef struct _TP_FLAG
{
    unsigned char tp_analysis;     //透传数据 数据分析中
    unsigned char tp_receiver;     //接收中
    unsigned char tp_stop;         //接收完毕
}TP_FLAG;


extern UART_BUF buf_uart1;
extern UART_BUF buf_uart2;
extern UART_BUF buf_uart3;
extern TP_BUF buf_tp;

extern void clear_buf_uart1(void);
extern void clear_buf_uart2(void);
extern void clear_buf_uart3(void);


extern void send_data_uart1(unsigned char);
extern void send_data_uart2(unsigned char);
extern void send_data_uart3(unsigned char);


extern void send_string_uart1(char * S);
extern void send_string_uart2(char * S);
extern void send_string_uart3(char * S);

extern unsigned char is_gps_analysising(void);
extern unsigned char is_gps_receivering(void);
extern unsigned char is_gps_stoping(void);
extern void clean_gps_flag(void);

extern unsigned char is_config_analysising(void);
extern unsigned char is_config_receivering(void);
extern unsigned char is_config_stoping(void);
extern void clean_config_flag(void);

extern unsigned char is_tp_analysising(void);
extern unsigned char is_tp_receivering(void);
extern unsigned char is_tp_stoping(void);
extern void clean_tp_flag(void);

extern void debug_put_word(unsigned char word);

extern void init_data_cache(int size);
#endif/* __STM32_USART_H */
