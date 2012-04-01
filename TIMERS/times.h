#ifndef _TIMERS_H_
#define _TIMERS_H_

#define MAX_COMMA 256
typedef struct TIME2_T
{
    unsigned int    time_overflow_10ms;
    unsigned int    time_overflow_ms;
    unsigned char   read_gps_flag;
    unsigned int    send_gps_position;
}TIME2_T;

extern void Delay(unsigned long nCount);
extern void delay_GSM(unsigned int i);
extern void delay_1ms(unsigned int i);

extern unsigned char is_enable_read_gps(void);
extern void clean_time2_flags(void);
extern unsigned char is_enable_send_gps_position(void);
extern unsigned char GetComma(unsigned char num,char *str);
#endif
