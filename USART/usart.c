#include "usart.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_flash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UART_BUF buf_uart1;     //pc
UART_BUF buf_uart2;
UART_BUF buf_uart3;     //gprs
TP_BUF buf_tp;          //tp

GPS_FLAG g_gps_flag;
CONFIG_FLAG g_config_flag;
TP_FLAG g_tp_flag;	// 透传测试

char temp_line[BUFLEN+1];
char isFULL = 0;

/*! pc receive port*/
void clear_buf_uart1(void)
{
    unsigned int i = 0 ,c ;
    c = BUFLEN +1 ;
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

    for( i = 0 ; i < c ; i ++)
    {
        buf_uart1.buf[i] = 0x0 ;
    }

    buf_uart1.index = 0 ;

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/*! gps receive port*/
void clear_buf_uart2(void)
{
    unsigned int i = 0 ,c ;
    c = BUFLEN +1 ;
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);

    for( i = 0 ; i < c ; i ++)
    {
        buf_uart2.buf[i] = 0x0 ;
    }

    buf_uart2.index = 0 ;

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/*! gprs receive port*/
void clear_buf_uart3(void)
{
    unsigned int i = 0 ,c ;
    c = BUFLEN +1 ;
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	 //取消USART3的接收中断

    for( i = 0 ; i < c ; i ++)
    {
        buf_uart3.buf[i] = 0x0 ;
    }

    buf_uart3.index = 0 ;

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //打开USART3的接收中断
}

void send_string_uart1(char * S)
{
    while(*S)
    {
        send_data_uart1(*S++);
    }

}

void send_string_uart2(char * S)
{
    while(*S)
    {
        send_data_uart2(*S++);
    }
}

void send_string_uart3(char * S)
{
    while(*S)
    {
        send_data_uart3(*S++);
    }

}


void debug_put_word(unsigned char word)
{
    send_data_uart2('[');
    send_data_uart2(word);
    send_data_uart2(']');
}

void send_data_uart1(unsigned char Data)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, Data);
}

void send_data_uart2(unsigned char Data)
{
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, Data);
}

void send_data_uart3(unsigned char Data)
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3, Data);
}

unsigned char is_gps_analysising(void)
{
    return g_gps_flag.gps_analysis;
}

unsigned char is_gps_receivering(void)  //receiving data
{
    return g_gps_flag.gps_receiver;
}

unsigned char is_gps_stoping(void)      //a line data finish
{
    return g_gps_flag.gps_stop;
}

void clean_gps_flag(void)
{
    g_gps_flag.gps_analysis = 0;        //数据分析-头字符
    g_gps_flag.gps_receiver = 0;        //数据接收ing
    g_gps_flag.gps_stop = 0;            //接收完毕
}


unsigned char is_config_analysising(void)
{
    return g_config_flag.config_analysis;
}

unsigned char is_config_receivering(void)  //receiving data
{
    return g_config_flag.config_receiver;
}

unsigned char is_config_stoping(void)      //a line data finish
{
    return g_config_flag.config_stop;
}

void clean_config_flag(void)
{
    g_config_flag.config_analysis = 0;  //数据分析-头字符
    g_config_flag.config_receiver = 0;  //数据接收ing
    g_config_flag.config_stop = 0;      //接收完毕
}


unsigned char is_tp_analysising(void)
{
    return g_tp_flag.tp_analysis;
}

unsigned char is_tp_receivering(void)  //receiving data
{
    return g_tp_flag.tp_receiver;
}

unsigned char is_tp_stoping(void)      //a line data finish
{
    return g_tp_flag.tp_stop;
}

void clean_tp_flag(void)
{
    g_tp_flag.tp_analysis = 0;  //数据分析-头字符
    g_tp_flag.tp_receiver = 0;  //数据接收ing
    g_tp_flag.tp_stop = 0;      //接收完毕
}

/*! 串口1 PC   对串口一接收的数据进行处理*/
void usart1_receive_process_event(unsigned char ch )   //这里可以看出串口一是用来配置的，配置IP TCP/UDP 电话号码 目标端口号	目标IP等等
{
    /// 特殊配置数据
    if ((ch == '$') && (g_config_flag.config_analysis == 0))    //如果收到字符'$'，便开始接收
    {
        g_config_flag.config_receiver = 1;                      //开始接收第一个数据，接收完清零
        g_config_flag.config_stop  = 0;                         //开始接收，接收完置位，表明停止接收了
    }

    if (g_config_flag.config_receiver == 1)                     //标志位为1，开始接收
    {
        buf_uart1.buf[buf_uart1.index++] = ch;                  //字符存到数组中

        if (ch == '\n')                                         //如果接收到换行，则一行接收完毕
        {
            buf_uart1.buf[buf_uart1.index] = '\0';
            g_config_flag.config_receiver = 0; //接收完毕
            g_config_flag.config_stop  = 1;	   //停止接收
            g_config_flag.config_analysis = 1;
        }
    }

    if(!g_config_flag.config_receiver)    //普通数据接收
    {
        if(current_status == TM_OK)
            send_data_uart3(ch);  //此时，ch==换行
    }

#if 0	//这里表示注释掉该段代码
    if(!g_config_flag.config_receiver)    //普通数据接收
    {
        if(buf_tp.index >= CACHESIZE-1)
        {
            buf_tp.index = 0 ;
        }
        else
        {
            buf_tp.buf[buf_tp.index++] = ch;
        }
    }
#endif
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)              //接收到数据
    {
        usart1_receive_process_event(USART_ReceiveData(USART1));  //对接收到的每个数据进行处理
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }

    //溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
    {
        usart1_receive_process_event(USART_ReceiveData(USART1));
        USART_ClearFlag(USART1,USART_FLAG_ORE);
    }
}




/*! 串口2 GPS*/
void usart2_receive_process_event(unsigned char ch )
{
    if ((ch == '$') && (g_gps_flag.gps_analysis == 0))  //如果收到字符'$'，便开始接收
    {
        g_gps_flag.gps_receiver = 1;                    //开始接收第一个数据，接收完清零
        g_gps_flag.gps_stop  = 0;                       //开始接收，接收完置位，表明停止接收了
    }

    if (g_gps_flag.gps_receiver == 1)                   //标志位为1，开始接收
    {
        buf_uart2.buf[buf_uart2.index++] = ch;        //字符存到数组中

        if (ch == '\n')                                 //如果接收到换行，则一行接收完毕
        {
            buf_uart2.buf[buf_uart2.index] = '\0';
            g_gps_flag.gps_receiver = 0;
            g_gps_flag.gps_stop  = 1;
            g_gps_flag.gps_analysis = 1;
        }
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
    {

        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }

    //溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
}

/*! 串口3 GPRS*/
void usart3_receive_process_event(unsigned char ch )   //对从GPRS来的数据进行逐个处理
{
    if(buf_uart3.index >= BUFLEN-1)	 //若溢出则覆盖
    {
        clear_buf_uart3();
        buf_uart3.index = 0 ;
    }
    else
    {
        buf_uart3.buf[buf_uart3.index++] = ch;
    }
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        usart3_receive_process_event(USART_ReceiveData(USART3));
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }

    //溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题[牛人说要这样]
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)
    {
        usart3_receive_process_event(USART_ReceiveData(USART3));
        USART_ClearFlag(USART3,USART_FLAG_ORE);
    }
}

void init_data_cache(int size) //透传测试缓存器空间大小设定
{
    if(size <= 0 && size >= 4097)
    {
        PUT("cache error!");
        return;
    }

    memset(buf_tp.buf,0,size);
    buf_tp.index = 0;
}


