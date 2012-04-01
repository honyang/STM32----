#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x_lib.h"

#include "gprs.h"
#include "../USART/usart.h"
#include "../TIMERS/times.h"

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
int num_of_sms;
#define SMS_CMD_LEN 64
char at_readsms[SMS_CMD_LEN];
char tpy_sms_content[SMS_CMD_LEN];
char tpy_send_tel_num[SMS_CMD_LEN];
char tp_content[BUFLEN];
unsigned char is_gprs_mode_ok = 1;                  //gprs start succeed
unsigned char is_gprs_mode_start_succeed = 0;       //gprs connect ok

SIM300_STATUS current_status = INIT0;


#define PHONENAME "+8613512963352"
//REG
#define ATE0_CMD "ATE0\x00D\x00A"           /// 禁止回显
#define CREG_CMD "AT+CREG?\x00D\x00A"       /// 查询网络注册情况
#define SIMCARD_CMD "AT+CPIN?\x00D\x00A"
//LD GPRS
#define GPRS_QD "AT+CSTT\x00D\x00A"         /// GPRS启动
#define GPRS_JH "AT+CIICR\x00D\x00A"        /// 移动场景开启,激活
//LGL GPRS
#define GPRS_DEF_PDP "AT+CGDCONT=1,\"IP\",\"CMNET\"\x00D\x00A"  /// 接入点
//#define GPRS_ACT_PDP "AT+CGATT=1\x00D\x00A"                     /// 激活PDP
#define GPRS_ACT_PDP "AT%ETCPIP\x00D\x00A"
#define GPRS_CMNET_APN "AT+CIPCSGP=1,\"CMNET\"\x00D\x00A"       /// 设置GPRS模式

#define GPRS_DK1 "AT+CLPORT="               /// 配置默认端口指令
#define GPRS_DK2 PROTOTOCOL
#define GPRS_DK3 "60000"
//CONNECT... GPRS
//#define GPRS_BJ_ADDR "AT+CIFSR\x00D\x00A"   /// 获取本地IP
#define GPRS_BJ_ADDR "AT%ETCPIP?\x00D\x00A"   /// 获取本地IP
#define GPRS_SEND_TEXT_FORMAT "AT%IOMODE=0,1,1\x00D\x00A"
//#define GPRS_TCP1 "AT+CIPSTART="            /// 建立一个IP连接
#define GPRS_TCP1 "AT%IPOPEN="
#define GPRS_TCP2 PROTOTOCOL
#define GPRS_TCP3 IPADDR
#define GPRS_TCP4 PORTNUM

//SEND  GPRS
//#define GPRS_SEND_DATA "AT+CIPSEND\x00D\x00A"
#define GPRS_SEND_DATA "AT%IPSEND=\""

//SEND SMS
#define GSM_MSG_MODE "AT+CMGF=1\x00D\x00A"
#define GSM_MSG_MODE_CN "AT+CMGF=0\x00D\x00A"

//#define GSM_CENTER_NUM "AT+CSCA=\"+8613800512500\"\x00D\x00A"
#define GSM_CHAR_MODE "AT+CSCS=\"GSM\"\x00D\x00A"
#define GSM_CHAR_MODE_CN "AT+CMEE=0\x00D\x00A"
#define GSM_SEND_TEXT "AT+CMGS=\"+8613512963352\"\x00D\x00A"
//READ SMS
#define SMSCAT "CMTI: \"SM\","
#define SMSCATRHOS "+CMT:"
#define TPY "tpy+"
#define LED_CTRL "chance"
#define SEND_THE_MSG_NUM "tel="
#define SEND_TEMPLATE_DATA "tem=?"
#define NO_GPS_FIX "positon not locate"
#define NO_SENSOR "no sensor"

#define VER2_DEFAULT_SMS_NUM 0
/// tm
#define TM_TYPE_DEF "AT+CGDCONT=1,\"IP\",\"CMNET\"\x00D\x00A"
#define TM_TYPE_ACT "AT%ETCPIP\x00D\x00A"
#define TM_TYPE_IOMODE "AT%IOMODE=0,2,0\x00D\x00A"
#define TM_TYPE_TPSMODE "AT%TPS=2,1,3000,1024\x00D\x00A"

#define GPRS_TP1 "AT%IPOPENX="
#define GPRS_TP2 "1"
#define GPRS_TP3 PROTOTOCOL
#define GPRS_TP4 IPADDR
#define GPRS_TP5 PORTNUM

#define TCPIP_ETCPIP "AT%ETCPIP?\x00D\x00A"

/// MG323 TM
#define MG323_TP_NETTYPE "AT^SICS=0,conType,GPRS0\x00D\x00A"
#define MG323_TP_APNTYPE "AT^SICS=0,apn,cmnet\x00D\x00A"
#define MG323_TP_CONID "AT^SISS=0,conId,0\x00D\x00A"
#define MG323_TP_SERTYPE "AT^SISS=0,srvType,Socket\x00D\x00A"
char mg323_url[BUFLEN]; //at^siss=0,address,"socktcp://116.231.65.197:5100"
#define MG323_TP_NETOPEN "AT^SISO=0\x00D\x00A"
#define MG323_TP_TRANS "AT^IPENTRANS=0\x00D\x00A"

#define P_TCP "TCP"
#define P_UDP "UDP"
#define READ_SOCKET "AT%IPDR\x00D\x00A"
#define DEL_SOCKET "AT%IPDD=1\x00D\x00A"


//妈妈:我着凉了，快盖上小被子，别凉着肚肚噢!《电脑小保姆，宝宝健康的保护神》90
char msg1[] = "0891683108200205F011000B813107831385F70008A74C59885988003A6211774051C94E86FF0C5FEB76D64E0A5C0F88AB5B50FF0C522B51C97740809A809A56620021300A753581115C0F4FDD59C6FF0C5B9D5B9D50655EB776844FDD62A4795E300B";
//妈妈:快来呀，宝宝想妈妈了！电脑小保姆在陪我玩噢！64
char msg2[] = "0891683108200205F011000B813107831385F70008A73259885988003A5FEB67655440FF0C5B9D5B9D60F3598859884E86FF01753581115C0F4FDD59C65728966A621173A95662FF01";
//妈妈:我翻身了，小心宝宝掉下床噢！   ～～为宝宝昼夜值班永不休息！（智能电脑小保姆）100
char msg3[] = "0891683108200205F011000B813107831385F70008A75659885988003A62117FFB8EAB4E86FF0C5C0F5FC35B9D5B9D63894E0B5E8A5662FF01002000200020FF5EFF5E4E3A5B9D5B9D663C591C503C73ED6C384E0D4F11606FFF01FF08667A80FD753581115C0F4FDD59C6FF09";
//妈妈:我尿尿了，好湿阿，快给宝宝换窝啦！   －－－电脑小保姆，时刻呵护宝宝的健康！ 98
char msg4[] = "0891683108200205F011000B813107831385F70008A75459885988003A62115C3F5C3F4E86FF0C597D6E7F963FFF0C5FEB7ED95B9D5B9D63627A9D5566FF01002000200020FF0DFF0DFF0D753581115C0F4FDD59C6FF0C65F6523B547562A45B9D5B9D768450655EB7FF01";





//开机GTM900
void start_gprs_mode(void)
{
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
/// 服务器 AT+CLPORT="TCP","2020"
void __GPRS_COM()
{
}

/// 建立一个IP连接 AT+CIPSTART=”TCP”,”211.136.42.12”,”2020”
void __GPRS_TCPIP()
{//AT+CMGR=1

}

/// 建立一个TP连接 AT%IPOPENX=1,"TCP","111.164.240.194",1001
void __GPRS_TP()
{//AT+CMGR=1


}


/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
static void S_INIT0()                           //初始化函数 发送关闭回显
{

    PUT("S_INIT0 \r\n");

    clear_buf_uart3();
    send_string_uart3(ATE0_CMD);
    current_status = ECHO0 ;
}

static void S_ECHO0()                           //关闭回显函数 发送SIM卡检测
{
    if(buf_uart3.buf[2] == 'O' &&
       buf_uart3.buf[3] == 'K')
    {

        PUT("ECHO0 OK \r\n");
        delay_GSM(5000);
        is_gprs_mode_start_succeed = 1;         //说明GPRS模块存在

        clear_buf_uart3();
        current_status = SIMCARD ;
        send_string_uart3(SIMCARD_CMD);

    }else{
        PUT(buf_uart3.buf);
        PUT("ECHO0 ERR \r\n");

        clear_buf_uart3();
        send_string_uart3(ATE0_CMD);
    }
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
static void S_SIMCARD()                         //SIM卡检测函数 发送信号查询
{
    char *t = NULL ;

    PUT(buf_uart3.buf);
    t = strstr((const char*)buf_uart3.buf,"OK");
    if(t)
    {

        PUT("SIMCARD OK \r\n");
        delay_GSM(5000);

        clear_buf_uart3();
        current_status = CREG ;
        send_string_uart3(CREG_CMD);

    }else{
        PUT(buf_uart3.buf);
        PUT("SIMCARD ERR \r\n");

        clear_buf_uart3();
        send_string_uart3(SIMCARD_CMD);
    }
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/

static void S_CREG()
{
    if(((buf_uart3.buf[9]=='0')&&
        (buf_uart3.buf[11]=='1'))||
       ((buf_uart3.buf[9]=='0')&&
        (buf_uart3.buf[11]=='5')))
    {
        PUT("S_REG OK \r\n");
        current_status = INIT_FINISH_OK;
    }else{
        PUT(buf_uart3.buf);
        PUT("S_REG ERR \r\n");

        debug_put_word(buf_uart3.buf[9]);
        debug_put_word(buf_uart3.buf[11]);
        send_string_uart1("\r\n");

        clear_buf_uart3();
        current_status = CREG ;
        send_string_uart3(CREG_CMD);
    }
}

/*!
  *   @fn void init_sim300()
  *   @note  查询GSM网络状况
  *   @return
  *   @attention
  */
void init_gprs_mode(void)
{

    PUT("start init gprs mode\r\n");

    while(1)
    {
        err ++;
        if(err > MAX_ERROR_COUNT)         //error
        {
            err = 0 ;
            is_gprs_mode_ok = 0;
            PUT("GPRS MODE REGISTER [NO]");
            break;
        }
#if 0
        if(is_gprs_mode_start_succeed)
        {
            PUT("GPRS MODE REGISTER [OK]");
        }
#endif
        delay_GSM(5000);

        switch(current_status)
        {
        case INIT0:
            S_INIT0();      //ATE0
            break ;

        case ECHO0:
            S_ECHO0();      //SIMCARD
            break ;

        case SIMCARD:
            S_SIMCARD();    //REG
            break ;

        case CREG:
            S_CREG();       //FINISH
            break ;

        case INIT_FINISH_OK:
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
static void S_START_OPEN_GPRS_S()               //开始GPRS 检测SIM卡先
{
    PUT("S_START_OPEN_GPRS_S OK \r\n");

    clear_buf_uart3();
    current_status = GPRS_DEF_PDP_S ;           //定义PDP移动场景
    send_string_uart3(SIMCARD_CMD);
}


/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
static void S_GPRS_DEF_PDP_S()                  //检测SIM卡 定义PDP
{
    char *t = NULL ;

    PUT(buf_uart3.buf);
    t = strstr((const char*)buf_uart3.buf,"OK");

    if(t)
    {
        PUT("SIMCARD_CMD OK \r\n");
        clear_buf_uart3();
        current_status = GPRS_ACT_PDP_S ;
        send_string_uart3(GPRS_DEF_PDP);
    }
    else
    {
        PUT("SIMCARD_CMD NO \r\n");

        clear_buf_uart3();
        send_string_uart3(SIMCARD_CMD);
    }
}

static void S_GPRS_ACT_PDP_S()                  //判断定义PDP 激活PDP
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_GPRS_DEF_PDP_S OK \r\n");
        clear_buf_uart3();
        current_status = GPRS_ACT_PDP_S_RET ;       // 判断激活返回
        send_string_uart3(GPRS_ACT_PDP);
    }
    else
    {
        PUT("S_GPRS_DEF_PDP_S NO \r\n");

        clear_buf_uart3();
        send_string_uart3(GPRS_DEF_PDP);
    }
}

static void S_GPRS_ACT_PDP_S_RET()                  //判断定义PDP 激活PDP
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("GPRS_ACT_PDP_S OK \r\n");
        clear_buf_uart3();
        current_status = GPRS_OPEN_FINISH;
    }
    else
    {
        PUT("GPRS_ACT_PDP_S NO \r\n");
        clear_buf_uart3();
    }
}

#if 0
static void S_GPRS_CMNET_APN_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("GPRS_DEF_PDP OK \r\n");
        clear_buf_uart3();
        current_status = GPRS_CMNET_APN_S ;     // 选择接入点
        send_string_uart3(GPRS_ACT_PDP_HUAWEI);
    }
    else
    {
        PUT("GPRS_DEF_PDP NO \r\n");

        clear_buf_uart3();
        send_string_uart3(GPRS_DEF_PDP);
    }
}

static void S_GPRS_CMNET_APN_RET_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("GPRS_ACT_PDP_S OK \r\n");
        clear_buf_uart3();
        current_status = GPRS_OPEN_FINISH;
    }
    else
    {
        PUT("GPRS_ACT_PDP_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(GPRS_ACT_PDP_HUAWEI);
    }
}
#endif
void open_gprs_simple(void)
{

    if(!is_gprs_mode_ok)
        return ;

    while(1)
    {
        err ++;
        if(err>MAX_ERROR_COUNT)
        {
            err = 0 ;

            PUT("GPRS START [NO]");
            delay_GSM(100000);
            break;
        }

        delay_GSM(1000);

        switch(current_status)
        {
        case INIT_FINISH_OK:
            S_START_OPEN_GPRS_S();
            break ;

        case  GPRS_DEF_PDP_S:
            S_GPRS_DEF_PDP_S();
            break ;

        case  GPRS_ACT_PDP_S:
            S_GPRS_ACT_PDP_S();
            break ;
#if 0
        case GPRS_ACT_PDP_S:            //激活PDP
            S_GPRS_CMNET_APN_S();
            break ;

        case GPRS_CMNET_APN_S :
            S_GPRS_CMNET_APN_RET_S();
            break ;
#endif
        case GPRS_ACT_PDP_S_RET:
            S_GPRS_ACT_PDP_S_RET();
            break ;

        case GPRS_OPEN_FINISH:
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}


static void S_GPRS_OPEN_FINISH()                //初始化GPRS发送模式
{
    clear_buf_uart3();
    current_status = TCPIP_IO_MODE ;
}


static void S_TCPIP_IO_MODE()                   //设置发送GPRS格式
{
    clear_buf_uart3();
    current_status = TCPIP_BJ_ADDR ;
    send_string_uart3(GPRS_SEND_TEXT_FORMAT);
    PUT("S_TCPIP_IO_MODE \r\n");
}

#if 1
static void S_TCPIP_BJ_ADDR()                   //判断设置GPRS格式 获取IP 检查连接
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_TCPIP_IO_MODE OK \r\n");

        clear_buf_uart3();
        current_status = TCPIP_CONNECT ;       // 判断激活返回
        send_string_uart3(GPRS_BJ_ADDR);
    }
    else
    {
        PUT("S_TCPIP_IO_MODE NO \r\n");

        clear_buf_uart3();
        send_string_uart3(GPRS_SEND_TEXT_FORMAT);
    }
}
#endif

static void S_TCPIP_CONNECT()
{
    current_status = TCP_IP_CONNECTING ;
    __GPRS_TCPIP();
}

static void S_TCP_IP_CONNECTING()
{

    char *ret_val = NULL ;

    static unsigned int count = 0 ;

    ret_val = strstr((const char*)buf_uart3.buf,"CONNECT");
    PUT(buf_uart3.buf);
    PUT("S_TCP_IP_CONNECTING\r\n");
    if (ret_val)
    {
        current_status = TCP_IP_OK ;
    }
    else
    {
        count ++ ;
        delay_GSM(2000);
        if (count == 20)
        {
            current_status = TCP_IP_NO ;
            count = 0 ;
        }
    }
}

void creat_tcp(void)
{
    if(!is_gprs_mode_ok)
        return ;

    while(1)
    {
        err ++;
        if(err>MAX_ERROR_COUNT)
        {
            err = 0 ;
            PUT("SIM300_TCPIP_FALSE");
            delay_GSM(100000);
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case GPRS_OPEN_FINISH:
            S_GPRS_OPEN_FINISH();
            break ;

        case TCPIP_IO_MODE:             //get GPRS
            S_TCPIP_IO_MODE();
            break ;
#if 1
        case TCPIP_BJ_ADDR:             //get GPRS
            S_TCPIP_BJ_ADDR();
            break ;
#endif
        case TCPIP_CONNECT:
            S_TCPIP_CONNECT();
            break ;

        case TCP_IP_CONNECTING:
            S_TCP_IP_CONNECTING();
            break ;

        case TCP_IP_OK :
            PUT(buf_uart3.buf);
            return ;
        case  TCP_IP_NO:
            PUT(buf_uart3.buf);
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}


void send_tp_data(char * buf , unsigned int count)
{
    int i = 0;
    for( i = 0 ;i < count ; i ++)
    {
        send_string_uart3(buf);
        delay_GSM(1000);
    }
}


void send_tp_data_to_service(void)
{
    if(strlen(buf_tp.buf)>0)
    {
        isFULL = 1;

        send_tp_data(buf_tp.buf,1);
        PUT(buf_tp.buf);
        delay_GSM(3000);
        init_data_cache(CACHESIZE);
    }
    isFULL = 0;
}


void send_gprs_data(char * buf , unsigned int count)
{
    unsigned int i ;


    if (current_status != TCP_IP_OK)        // 如果协议没有连接成功直接返回
        return ;
#if 0
    send_string_uart3("AT");
    send_data_uart3('%');

    memset(temp_line,0,BUFLEN);
    for( i = 0 ;i < count ; i ++)
    {
        sprintf(temp_line,"IPSEND=\"%s\"\x00D\x00A",buf);
    }

    PUT(temp_line);
    send_string_uart3(temp_line);
#endif

#if 1
    send_string_uart3(GPRS_SEND_DATA);

    for( i = 0 ;i < count ; i ++)
    {
        for ( i = 0 ; i < strlen((const char*)buf) ; i ++)
        {
            if(buf[i] != 0x0d  && buf[i] != 0x0a)
            {
                send_data_uart3(buf[i]);
            }
        }
    }
    send_data_uart3(0X22);
    send_data_uart3(0x0D);  //CR
    send_data_uart3(0x0A);  //LF
#endif
    delay_GSM(5000);
}

/**
  ******************************************************************************
  *     透传开始 流程
  ******************************************************************************
  */
static void S_TM_GPRS_START()
{
    clear_buf_uart3();
    current_status = TRANSPARENT_MODE_DEF ;
    send_string_uart3(TM_TYPE_DEF);
}


static void S_TM_TRANSPARENT_MODE_DEF_S()                   //判断DEF 激活PDP
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_TM_TRANSPARENT_MODE_DEF_S OK \r\n");
        clear_buf_uart3();
        current_status = TRANSPARENT_MODE_ACT;
        send_string_uart3(TM_TYPE_ACT);
    }
    else
    {
        PUT("S_TM_TRANSPARENT_MODE_DEF_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(TM_TYPE_DEF);
    }
}

static void S_TM_TRANSPARENT_MODE_ACT_S()                   //判断PDP激活 改变IO模式
{
    PUT(buf_uart3.buf);
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_TM_TRANSPARENT_MODE_ACT_S OK \r\n");
        clear_buf_uart3();
        current_status = TRANSPARENT_MODE_IOMODE;
        send_string_uart3(TM_TYPE_IOMODE);
    }
    else
    {
        PUT("S_TM_TRANSPARENT_MODE_ACT_S NO \r\n");
        PUT(buf_uart3.buf);
        PUT("FUCK");
        clear_buf_uart3();
        send_string_uart3(TM_TYPE_ACT);
    }
}

static void S_TM_TRANSPARENT_MODE_IOMODE_S()                //判断IO MODE 连接网络
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_TM_TRANSPARENT_MODE_IOMODE_S OK \r\n");
        PUT(buf_uart3.buf);
        clear_buf_uart3();
        PUT(buf_uart3.buf);
        current_status = TRANSPARENT_MODE_CONNECT;
        __GPRS_TP();
    }
    else
    {
        PUT("S_TM_TRANSPARENT_MODE_IOMODE_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(TM_TYPE_IOMODE);
    }
}

static void S_TM_TRANSPARENT_MODE_CONNECT_S()
{
    char *ret_val = NULL ;
    static unsigned int count = 0 ;

    ret_val = strstr((const char*)buf_uart3.buf,"CONNECT");

    PUT(buf_uart3.buf);
    PUT("S_TM_TRANSPARENT_MODE_CONNECT_S \r\n");

    if (ret_val)
    {
        current_status = TM_CON_OK ;
    }
    else
    {
        count ++ ;
        delay_GSM(1000);
        if (count == 20)
        {
            current_status = TM_CON_NO ;
            count = 0 ;
        }
    }
}



static void S_TM_TRANSPARENT_MODE_TPSMODE_S()                //判断IO MODE 连接网络
{

    clear_buf_uart3();
    current_status = TM_OK;
    send_string_uart3(TM_TYPE_TPSMODE);
    PUT("TM_OK\r\n");
}


void creat_tp_mode(void)
{

    if(!is_gprs_mode_ok)
        return ;

    current_status = TRANSPARENT_MODE_START;

    while(1)
    {
        err ++;
        if(err > MAX_ERROR_COUNT)
        {
            err = 0 ;
            delay_GSM(100000);
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case TRANSPARENT_MODE_START:
            S_TM_GPRS_START();
            break ;

        case TRANSPARENT_MODE_DEF:
            S_TM_TRANSPARENT_MODE_DEF_S();
            break ;

        case TRANSPARENT_MODE_ACT:
            S_TM_TRANSPARENT_MODE_ACT_S();
            break ;

        case TRANSPARENT_MODE_IOMODE:
            S_TM_TRANSPARENT_MODE_IOMODE_S();
            break ;

        case TRANSPARENT_MODE_CONNECT:
            S_TM_TRANSPARENT_MODE_CONNECT_S();
            break ;

        case TM_CON_OK :
            PUT(buf_uart3.buf);
            clear_buf_uart3();
            S_TM_TRANSPARENT_MODE_TPSMODE_S();
            return ;
        case TM_CON_NO:
            PUT(buf_uart3.buf);
            clear_buf_uart3();
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}


/**
  ******************************************************************************
  *     关闭TCP IP
  ******************************************************************************
  */

static void S_CLOSE_GPRS_START()
{
    clear_buf_uart3();
    current_status = TCPIP_CLOSE_DEF;
    send_string_uart3(TM_TYPE_DEF);
}


static void S_CLOSE_TRANSPARENT_MODE_DEF_S()                   //判断DEF 激活PDP
{
    if((buf_uart1.buf[2]=='O')&&(buf_uart1.buf[3]=='K'))
    {
        PUT("S_CLOSE_TRANSPARENT_MODE_DEF_S OK \r\n");
        clear_buf_uart1();
        current_status = TCPIP_CLOSE_ETCPIP;
        send_string_uart1(TCPIP_ETCPIP);
    }
    else
    {
        PUT("S_CLOSE_TRANSPARENT_MODE_DEF_S NO \r\n");
        clear_buf_uart1();
        send_string_uart1(TM_TYPE_DEF);
    }
}

static void S_CLOSE_TRANSPARENT_MODE_ETCPIP_S()                   //判断PDP激活 改变IO模式
{
    static unsigned int count = 0;

    PUT(buf_uart1.buf);
    if (strlen(buf_uart1.buf) > 30)
    {
        current_status = TCP_IP_CLOSE_OK ;
    }
    else
    {
        count ++ ;

        if (count == 20)
        {
            current_status = TCP_IP_CLOSE_NO ;
            count = 0 ;
        }
    }
}


void close_tcpip(void)
{
    if(!is_gprs_mode_ok)
        return ;

    current_status = TCPIP_CLOSE_START;
    while(1)
    {
        err ++;
        if(err > MAX_ERROR_COUNT)
        {
            err = 0 ;
            delay_GSM(100000);
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case TCPIP_CLOSE_START:
            S_CLOSE_GPRS_START();
            break ;

        case TCPIP_CLOSE_DEF:
            S_CLOSE_TRANSPARENT_MODE_DEF_S();
            break ;

        case TCPIP_CLOSE_ETCPIP:
            S_CLOSE_TRANSPARENT_MODE_ETCPIP_S();
            break ;

        case TRANSPARENT_MODE_IOMODE:
            S_TM_TRANSPARENT_MODE_IOMODE_S();
            break ;

        case TCP_IP_CLOSE_OK :
            send_string_uart2(buf_uart1.buf);
            break ;
        case TCP_IP_CLOSE_NO:
            send_string_uart2(buf_uart1.buf);
            break ;
        default:
            clear_buf_uart1();
            break ;
        }

    }
}


static void S_GSM_MSG_MODE_S()
{

    PUT("S_GSM_MSG_MODE_S OK \r\n");
    clear_buf_uart3();
    current_status = GSM_MSG_MODE_S ;           //定义PDP移动场景
    send_string_uart3(GSM_MSG_MODE);

}

static void S_GSM_CHAR_MODE_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_GSM_CHAR_MODE_S OK \r\n");
        clear_buf_uart3();
        current_status = GSM_CHAR_MODE_S;
        send_string_uart3(GSM_CHAR_MODE);
    }
    else
    {
        PUT("S_GSM_CHAR_MODE_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(GSM_MSG_MODE);
    }
}

static void S_GSM_CHAR_MODE_RET_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_GSM_CHAR_MODE_RET_S OK \r\n");
        clear_buf_uart3();
        current_status = GSM_TEXT_MODE_FINISH;
    }
    else
    {
        PUT("S_GSM_CHAR_MODE_RET_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(GSM_CHAR_MODE);
    }
}


static void init_sms_to_send()
{
    current_status = GSM_SEND_TEXT_S;

    while(1)
    {
        err ++;
        if(err>MAX_ERROR_COUNT)
        {
            err = 0 ;
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case GSM_SEND_TEXT_S:
            S_GSM_MSG_MODE_S();
            break ;

        case GSM_MSG_MODE_S:
            S_GSM_CHAR_MODE_S();
            break ;

        case GSM_CHAR_MODE_S:
            S_GSM_CHAR_MODE_RET_S();
            break ;

        case GSM_TEXT_MODE_FINISH:
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}

void send_gsm_text(unsigned char * buf , unsigned int count)
{
    unsigned int i ;
    char send_sms_cmd_num[SMS_CMD_LEN];

    memset((void*)send_sms_cmd_num,0,SMS_CMD_LEN);

    init_sms_to_send();     //发短信前配置数据

    if(current_status != GSM_TEXT_MODE_FINISH)
        return ;



    sprintf(send_sms_cmd_num,"AT+CMGS=\"+86%s\"\x00D\x00A",TELNUM);

    PUT(send_sms_cmd_num);
    send_string_uart3(send_sms_cmd_num);

    delay_GSM(100);
    for( i = 0 ;i < count ; i ++)
    {
        for ( i = 0 ; i < strlen((const char*)buf) ; i ++)
        {
            send_data_uart3(buf[i]);
        }
    }

    delay_GSM(100);
    send_data_uart3(0X1A);
    delay_GSM(10000);

    PUT("SEND TEXT\r\n");
}

int check_num_of_sms(void)
{
    char *p = NULL;

    PUT(buf_uart3.buf);
    p=strstr((const char*)buf_uart3.buf,SMSCAT);
    if(p)
    {
        PUT("have msg \r\n");
        PUT("SIM300_ONE_MSG");
        delay_GSM(1000000);

        p += strlen(SMSCAT);
        PUT(p);

        num_of_sms = atoi(p);
        clear_buf_uart3();
        return num_of_sms;
    }

    p=strstr((const char*)buf_uart3.buf,SMSCATRHOS);
    if(p)       //version 2
    {
        PUT("have msg \r\n");
        PUT("SIM300_ONE_MSG");
        delay_GSM(1000000);

        PUT(p);
        num_of_sms = VER2_DEFAULT_SMS_NUM;
        //        clear_buf_uart1();
        return num_of_sms;
    }

    //    else
    {
        clear_buf_uart3();
        PUT("no msg \r\n");
        delay_GSM(5000);

        return -1;
    }
}

static void S_SMS_READ_CMD_S()
{
    if(num_of_sms == 0)
        return ;
    memset(at_readsms,0,SMS_CMD_LEN);
    sprintf(at_readsms,"AT+CMGR=%d\x00D\x00A",num_of_sms);
    PUT(at_readsms);

    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_SMS_READ_CMD_S OK \r\n");
        clear_buf_uart3();
        current_status = SMS_READ_CONTENT_S ;
        send_string_uart3(at_readsms);
    }
    else
    {
        PUT("S_SMS_READ_CMD_S NO \r\n");

        clear_buf_uart3();
        send_string_uart3(GSM_MSG_MODE);
    }
}

static void S_READ_SMS_CONTENT()
{
    char *p = NULL;

    if(num_of_sms < 0)
        return ;

    PUT(buf_uart3.buf);
    p=strstr((const char*)buf_uart3.buf,TPY);

    if(p)
    {
        num_of_sms = 0;                         //总条数
        memset(at_readsms,0,SMS_CMD_LEN);       //AT读取那条

        PUT(p);

        memset(tpy_sms_content,0,SMS_CMD_LEN);
        p += strlen(TPY);
        sprintf(tpy_sms_content,"%s",p);

        PUT("GET SMS \r\n");
        PUT(tpy_sms_content);
        clear_buf_uart3();
        current_status = READ_SMS_FINISH ;
        return ;
    }

    current_status = SMS_READ_CONTENT_S ;
    send_string_uart3(at_readsms);
}

static void PARSER_SMS_TO_CONFIG(char * sms)
{
    char sms_buf[SMS_CMD_LEN];
    char con[SMS_CMD_LEN];
    char tip[SMS_CMD_LEN];
    char *p = NULL;

    memset(sms_buf,0,SMS_CMD_LEN);
    memset(con,0,SMS_CMD_LEN);
    memset(tip,0,SMS_CMD_LEN);
    sprintf(sms_buf,"%s",sms);

    PUT("SMS_BUF-\r\n");
    PUT(sms_buf);
    PUT("-END\r\n");

    p=strstr((const char*)sms_buf,LED_CTRL);
    if(p)
    {
        p += strlen(LED_CTRL);
        sprintf(con,"%s",p);
        PUT(con);
        return ;
    }

    p=strstr((const char*)sms_buf,SEND_TEMPLATE_DATA);
    if(p)
    {
        return ;
    }

    memset(tpy_send_tel_num,0,SMS_CMD_LEN);
    p=strstr((const char*)sms_buf,SEND_THE_MSG_NUM);
    if(p)
    {
        p += strlen(SEND_THE_MSG_NUM);
        strncpy(tpy_send_tel_num,p,11);

        PUT("\r\nTEL-");
        PUT(tpy_send_tel_num);
        PUT("-END");

        sprintf(tip,"Set %s [ok]",tpy_send_tel_num);

        send_gsm_text((unsigned char *)tip,1);
        return ;
    }
}


void read_the_index_sms()
{
    int numsms;
    if(!is_gprs_mode_ok)
        return ;
    numsms = check_num_of_sms();

    if(numsms == VER2_DEFAULT_SMS_NUM)
    {
        current_status = SMS_READ_CONTENT_S;
        while(1)
        {
            err ++;
            if(err>MAX_ERROR_COUNT)
            {
                err = 0 ;
                break;
            }

            delay_GSM(1000);
            switch(current_status)
            {
            case SMS_READ_CONTENT_S:
                S_READ_SMS_CONTENT();
                break ;

            case READ_SMS_FINISH:
                PARSER_SMS_TO_CONFIG(tpy_sms_content);
                //                return ;
                break ;

            default:
                clear_buf_uart1();
                //                return ;
                break ;
            }
        }

    }

    if(numsms > 0)
    {
        current_status = SMS_READ_MESSAGE_START_S;
        while(1)
        {
            err ++;
            if(err>MAX_ERROR_COUNT)
            {
                err = 0 ;
                break;
            }

            delay_GSM(1000);
            switch(current_status)
            {
            case SMS_READ_MESSAGE_START_S:
                S_GSM_MSG_MODE_S();
                break ;

            case GSM_MSG_MODE_S:
                S_SMS_READ_CMD_S();
                break ;

            case SMS_READ_CONTENT_S:
                S_READ_SMS_CONTENT();
                break ;

            case READ_SMS_FINISH:
                PARSER_SMS_TO_CONFIG(tpy_sms_content);
                //                return ;
                break ;

            default:
                clear_buf_uart3();
                //                return ;
                break ;
            }
        }
    }
}

unsigned char is_gprs_mode_status_ok(void)
{
    return is_gprs_mode_ok;
}

static void S_GSM_AT_MODE_S()
{
    clear_buf_uart3();
    current_status = GSM_AT_MODE_CMD ;           //定义PDP移动场景
    send_data_uart3(0x1b);
}

static void S_GSM_AT_MODE_RET_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        clear_buf_uart3();
        current_status = GSM_AT_MODE_FINISH;
    }
    else
    {
        clear_buf_uart3();
        send_data_uart3(0x1b);
    }
}

//切换到AT模式
void creat_at_mode(void)
{

    if(!is_gprs_mode_ok)
        return ;

    current_status = GSM_AT_MODE_START;
    while(1)
    {
        err ++;
        if(err > MAX_ERROR_COUNT)
        {
            err = 0 ;
            delay_GSM(100000);
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case GSM_AT_MODE_START:
            S_GSM_AT_MODE_S();
            break ;

        case GSM_AT_MODE_CMD:
            S_GSM_AT_MODE_RET_S();
            return ;

        default:
            clear_buf_uart3();
            break ;
        }
    }
}




static void S_READ_SOCKET_DATA_S()
{
    clear_buf_uart3();
    current_status = READ_SOCKET_DATA_CMD ;           //定义PDP移动场景
    send_string_uart3(READ_SOCKET);
}

static void S_READ_SOCKET_DATA_RET_S()
{
  
}

//切换到AT模式
void read_socket_data(void)
{

    if(!is_gprs_mode_ok)
        return ;

    current_status = READ_SOCKET_DATA_START;
    while(1)
    {
        err ++;
        if(err > MAX_ERROR_COUNT)
        {
            err = 0 ;
            delay_GSM(100000);
            break;
        }

        delay_GSM(500);
        switch(current_status)
        {
        case READ_SOCKET_DATA_START:
            S_READ_SOCKET_DATA_S();
            break ;

        case READ_SOCKET_DATA_CMD:
            S_READ_SOCKET_DATA_RET_S();
            return ;

        default:
            clear_buf_uart3();
            break ;
        }
    }
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/

void read_data(void)
{
    if(current_status == TM_OK)
    {
        creat_at_mode();
    }
    if(current_status == GSM_AT_MODE_FINISH)
    {
        read_socket_data();
    }

    S_TM_TRANSPARENT_MODE_TPSMODE_S();
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void send_message(char *msg)
{
 
}


static void S_GSM_MSG_MODE_CN_S()
{

    PUT("S_GSM_MSG_MODE_S_CN OK \r\n");
    clear_buf_uart3();
    current_status = GSM_MSG_MODE_CN_S ;           //定义PDP移动场景
    send_string_uart3(GSM_MSG_MODE_CN);
    PUT(GSM_MSG_MODE_CN);
}



static void S_GSM_CHAR_MODE_CN_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_GSM_CHAR_MODE_S OK \r\n");
        clear_buf_uart3();
        current_status = GSM_CHAR_MODE_CN_S;
        send_string_uart3(GSM_CHAR_MODE_CN);
        PUT(GSM_CHAR_MODE_CN);
    }
    else
    {
        PUT("S_GSM_CHAR_MODE_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(GSM_MSG_MODE_CN);
    }
}

static void S_GSM_CHAR_MODE_RET_CN_S()
{
    if((buf_uart3.buf[2]=='O')&&(buf_uart3.buf[3]=='K'))
    {
        PUT("S_GSM_CHAR_MODE_RET_S OK \r\n");
        clear_buf_uart3();
        current_status = GSM_TEXT_MODE_FINISH;
    }
    else
    {
        PUT("S_GSM_CHAR_MODE_RET_S NO \r\n");
        clear_buf_uart3();
        send_string_uart3(GSM_CHAR_MODE_CN);
    }
}



void init_sms_to_send_chinese(void)
{
    current_status = GSM_SEND_TEXT_CN_S;

    while(1)
    {
        err ++;
        if(err>MAX_ERROR_COUNT)
        {
            err = 0 ;
            break;
        }

        delay_GSM(1000);
        switch(current_status)
        {
        case GSM_SEND_TEXT_CN_S:
            S_GSM_MSG_MODE_CN_S();
            break ;

        case GSM_MSG_MODE_CN_S:
            S_GSM_CHAR_MODE_CN_S();
            break ;

        case GSM_CHAR_MODE_CN_S:
            S_GSM_CHAR_MODE_RET_CN_S();
            break ;

        case GSM_TEXT_MODE_FINISH:
            return ;
        default:
            clear_buf_uart3();
            break ;
        }
    }
}

// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
// 如："8613851872468" --> "683158812764F8"
// 输入: pSrc - 源字符串指针
//        nSrcLength - 源字符串长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength, i;								// 目标字符串长度
    char ch;									// 用于保存一个字符
    // 复制串长度
    nDstLength = nSrcLength;
    // 两两颠倒
    for(i=0; i<nSrcLength;i+=2)
    {
        ch = *pSrc++;							// 保存先出现的字符
        *pDst++ = *pSrc++;						// 复制后出现的字符
        *pDst++ = ch;							// 复制先出现的字符
    }
    // 源串长度是奇数吗？
    if(nSrcLength & 1)
    {
        *(pDst-2) = 'F';						// 补'F'
        nDstLength++;							// 目标串长度加1
    }
    // 输出字符串加个结束符
    *pDst = '\0';
    // 返回目标字符串长度
    return nDstLength;
}


void send_gsm_text_chinese(unsigned char * buf ,int lenstr, unsigned int count)
{
    unsigned int i ;
    char send_sms_cmd_num[SMS_CMD_LEN];

    memset((void*)send_sms_cmd_num,0,SMS_CMD_LEN);

    init_sms_to_send_chinese();     //发短信前配置数据

    if(current_status != GSM_TEXT_MODE_FINISH)
        return ;

    PUT((char *)buf);
    //    PUT(TELNUM);
    //    PUT(g_config_data.ipaddr);
    //    PUT(g_config_data.portnum);
    PUT("\r\n");


    sprintf(send_sms_cmd_num,"AT+CMGS=%d\x00D\x00A",lenstr);

    PUT(send_sms_cmd_num);
    send_string_uart3(send_sms_cmd_num);

    delay_GSM(2000);
    for( i = 0 ;i < count ; i ++)
    {
        for ( i = 0 ; i < strlen((const char*)buf) ; i ++)
        {
            send_data_uart3(buf[i]);
            send_data_uart2(buf[i]);
        }
    }

    delay_GSM(2000);
    send_data_uart3(0X1A);
    delay_GSM(10000);

    PUT("\r\nSEND TEXT\r\n");
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-8-10
最后修改日期      ：2012-8-13
-----------------------------------------------------------------------*/
void send_key_status(void)
{
    int cc = 0;

    PUT(TELNUM);              //target tel number
    PUT("\r\n");

    PUT_PC(TELNUM);
    PUT_PC("\r\n");

    for(cc=0;cc<12;cc++)                    //old tel number
        send_data_uart2(msg1[cc+26]);
    PUT_PC("\r\n");

    for(cc=0;cc<12;cc++)
        send_data_uart1(msg1[cc+26]);
    PUT_PC("\r\n");

    cc=0;                                   //ser new tel number
    msg1[cc+26] = TELNUM[1];
    msg1[cc+27] = TELNUM[0];
    msg1[cc+28] = TELNUM[3];
    msg1[cc+29] = TELNUM[2];
    msg1[cc+30] = TELNUM[5];
    msg1[cc+31] = TELNUM[4];
    msg1[cc+32] = TELNUM[7];
    msg1[cc+33] = TELNUM[6];
    msg1[cc+34] = TELNUM[9];
    msg1[cc+35] = TELNUM[8];
    msg1[cc+36] = 'F';
    msg1[cc+37] = TELNUM[10];


    for(cc=0;cc<12;cc++)                    //check new tel number
        send_data_uart2(msg1[cc+26]);
    PUT_PC("\r\n");

    for(cc=0;cc<12;cc++)
        send_data_uart1(msg1[cc+26]);
    PUT_PC("\r\n");
}

