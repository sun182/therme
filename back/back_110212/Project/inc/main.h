																			   /**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
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
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
#define TCP_TMR_INTERVAL       250
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f107.h"
#include "et_stm32f_arm_kit_i2c.h"
#include "et_stm32f_arm_kit_uart.h"
#include "stdint.h"


/* Exported function prototypes ----------------------------------------------*/
void Time_Update(void);
void Delay(uint32_t nCount);
void Delay1(uint32_t nTime);
void set_temp(int tsi);
void set_led_on(char tsi);
void show_seg1(unsigned char* pbuf);
void outpp(uint8_t value);
void printU(unsigned char chh);

void GetADC1Channel(unsigned char chanel);

void tempc_read(char* tempc1,char* tempc2);


void out_toggle_s(out_TypeDef out);
void leds_set(unsigned char add,unsigned char val);


void delay_read(char* delay_fin,char* delh,char* delm,char* dels);

void set_ebuff(char* eb);
void read_ebuff(char* eb);

void set_modes(unsigned char m);
unsigned char chk_modes(void);

void set_sdoor(unsigned char sd);
unsigned char chk_sdoor(void);


void set_start(unsigned char s);
unsigned char chk_start(void);



    unsigned char sdoor;
__IO unsigned char start;
char ebuff[45];
char ubuff[45];
char ubuff1 [45];
unsigned char eemodes;
__IO char tempc1[6];
__IO char tempc2[6];

/*
 malloc( sizeof(*ptr) );
__IO unsigned char delay_fin[10];

__IO unsigned char delh[10];
__IO unsigned char delm[10];
__IO unsigned char dels[10];
*/
unsigned char *delay_fin;

unsigned char *delh;
unsigned char *delm;
unsigned char *dels;

	
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

