

/**
  ******************************************************************************
  * @file    USART/Printf/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; C6OPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>
#include <string.h>
#include "USART/usart.h"
#include "CHIPSET/chipset.h"
#include "TIMERS/times.h"
#include "USART/usart.h"
#include "GPRS/gprs.h"


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
    }

    return ch;
}


int err = 0;
unsigned char is_gps_fix_position = 0;
char *p = NULL;

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-3-10
最后修改日期      ：2012-3-13
-----------------------------------------------------------------------*/
void stm32_Init(void)
{
    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    USART_Configuration();
}

/*-----------------------------------------------------------------------
创思通信 http://tpytongxin.taobao.com/

编写日期          ：2012-3-10
最后修改日期      ：2012-3-13
-----------------------------------------------------------------------*/
int main(void)
{
    clean_gps_flag();
    clear_buf_uart3();
    clean_time2_flags();
    clean_tp_flag();
    stm32_Init();
    printf("\r\n<<<<START>>>>\r\n");
    PUT("init usart finished \r\n");
    Init_TIM2();
    PUT("timer2 init finished \r\n");
    init_gprs_mode();
    PUT("init gprs mode reg finished \r\n");

    if(is_gprs_mode_status_ok())
    {
        PUT("GSM OK\r\n");
        PUT_PC("\r\nGSM OK \r\n");
    }
    else
    {
        PUT("NO GSM \r\n");
        PUT_PC("\r\nGSM NO \r\n");
    }


#if 1
    send_gsm_text("http://tpytongxin.taobao.com/\r\n",1);
    while(1);
#endif


}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}

#endif
