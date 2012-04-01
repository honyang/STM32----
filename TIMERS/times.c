#include "times.h"
#include "stm32f10x_lib.h"
#include "../USART/usart.h"
#include "../GPRS/gprs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
TIME2_T g_time2;

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void Delay(unsigned long  nCount)
{
    for(; nCount != 0; nCount--);
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void delay_GSM(unsigned int i)          //延时函数
{
    unsigned int i_delay,j_delay;
    for(i_delay=0;i_delay<i;i_delay++)
    {for(j_delay=0;j_delay<3000;j_delay++)
        {;}}
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void delay_1ms(unsigned int i)
{
    unsigned int i_delay,j_delay;
    for(i_delay=0;i_delay<i;i_delay++)
    {for(j_delay=0;j_delay<1000;j_delay++)
        {;}}
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void TIM2_IRQHandler(void)          //定时器中断10ms
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        g_time2.time_overflow_10ms++;
        g_time2.time_overflow_ms++;
        if (g_time2.time_overflow_ms == 50)  //100ms  read gps signal
        {
            g_time2.read_gps_flag = 1;
            g_time2.time_overflow_ms = 0;
        }

        if(g_time2.time_overflow_10ms == 100)   //1s GPRS send infomation
        {
            g_time2.send_gps_position ++;
            g_time2.time_overflow_10ms = 0;
        }
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void clean_time2_flags(void)      
{
    g_time2.read_gps_flag = 0;
    g_time2.time_overflow_10ms = 0;
    g_time2.time_overflow_ms = 0;
    g_time2.send_gps_position = 0;
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
unsigned char is_enable_read_gps(void)          //100ms判断是否读取GPS信号
{
    if(g_time2.read_gps_flag)
    {
        g_time2.read_gps_flag = 0;
        return 1 ;
    }

    return 0 ;
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
unsigned char is_enable_send_gps_position(void)     //允许发送GPRS数据
{
    if(g_time2.send_gps_position > 5)          //5s
    {
        g_time2.send_gps_position = 0;

        return 1 ;
    }

    return 0 ;
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
unsigned char GetComma(unsigned char num,char *str)
{
    unsigned char i,j = 0;
    int len=strlen(str);
    for(i = 0;i < len;i ++)
    {
        if(str[i] == ',')
            j++;
        if(j == num)
            return i + 1;
        if(j > MAX_COMMA)         //如果超时
            return 0;
    }

    return 0;
}

