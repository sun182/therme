/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    11/20/2009
 * @brief   Main program body
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITHCODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
 */

/* Includes ------------------------------------------------------------------*/
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include "stm32_eth.h"
#include "netconf.h"
#include "main.h"
#include "helloworld.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "httpclient.h"

//#include "tftpserver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define SYSTEMTICK_PERIOD_MS  10

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define buz_on	GPIOE->BSRR = GPIO_Pin_15
#define buz_off	GPIOE->BRR = GPIO_Pin_15


/* Private macro -------------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private variables ---------------------------------------------------------*/


unsigned char MCP23016_Device_Address; // MCP23016 Assigned Device Address.
int GPIO_port; // MCP23016 Data Port Register.

uint16_t capture1 = 0;
uint16_t CCR1_Val = 500;
uint16_t CCR2_Val = 400;
uint16_t CCR3_Val = 200;
uint16_t CCR4_Val = 100;

uint16_t del_ds;

char uart_buf[512];

unsigned int addi = 0;
int tempcal[2];
unsigned int tempdel;
uint16_t buf_port;
uint8_t out_p;
unsigned char modes;

char mcp_in[3];
double temp_p1;
double temp_p2;
double temp_tp1;
double temp_tp2;
unsigned char eemodes;
u8 buz_on_;
u8 come;

char leds[4];
u8 def;
unsigned int adc_p1;
unsigned int adc_p2;

u8 locks_d;

unsigned char temp_[3];
unsigned char temp_1[3];
unsigned char led_;

unsigned char delay_button;
unsigned char mode_dis;
unsigned char dis_delay;

unsigned int delay_com_start;

static struct tcp_pcb *TcpPCBs;

struct http_state
{
  char *file;
  u32_t left;
};


__IO uint32_t ocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
__IO uint32_t ocalTime1 = 0;
__IO uint32_t octime = 0;

uint8_t TxBuffer1_[200];
uint8_t TxBuffer2_[200];


#define TxBufferSize1   (199)
#define TxBufferSize2   (199)
#define RxBufferSize1   TxBufferSize2
#define RxBufferSize2   TxBufferSize1

__IO uint8_t TxCounter1 = 0x00;
__IO uint8_t TxCounter2 = 0x00;
__IO uint8_t RxCounter1 = 0x00;
__IO uint8_t RxCounter2 = 0x00;

uint8_t NbrOfDataToTransfer1 = TxBufferSize1;
uint8_t NbrOfDataToTransfer2 = TxBufferSize2;
uint8_t NbrOfDataToRead1 = RxBufferSize1;
uint8_t NbrOfDataToRead2 = RxBufferSize2;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;

uint32_t timingdelay;
uint32_t timingdelay1;
uint8_t eep_buffer[8];
//uint8_t eep_b[40];
uint8_t rtc[8];
int butt[8];
unsigned char locks = 1;

unsigned int dbuz1, dbuz2, dbu1, dbu2;

int td1;
int td2;
int td3;
int tr;
struct ip_addr server;
			
/* Create a new TCP control block  */

/*
 #define mcp1 0x40
 #define mcp2 0x4e

 #define mcp3 0x48
 #define mcp4 0x4c

 */
 #define mcp1 0x40
 #define mcp2 0x4e
 /*
#define p1h 0
#define p1m 1
#define p2h 2
#define p2m 3
#define p3h 4
#define p3m 5
#define p4m 6
#define p4h 7

#define p2_less_c 8
#define p2_more_c 9

#define p3_more_c 9

#define d1 9
#define d2 9
*/


#define GP0 0x00
#define GP1 0x01
#define OLAT0 0x02
#define OLAT1 0x03
#define IPOL0 0x04 // INPUT POLARITY PORT REGISTER 0
#define IPOL1 0x05 // INPUT POLARITY PORT REGISTER 1
#define IODIR0 0x06 // I/O DIRECTION REGISTER 0
#define IODIR1 0x07 // I/O DIRECTION REGISTER 1
#define INTCAP0 0x08 // INTERRUPT CAPTURE REGISTER 0
#define INTCAP1 0x09 // INTERRUPT CAPTURE REGISTER 1
#define IOCON0 0x0A // I/O EXPANDER CONTROL REGISTER 0
#define IOCON1 0x0B // I/O EXPANDER CONTROL REGISTER 1
#define led_1 0x01
#define led_2 0x02
#define led_3 0x04
#define led_4 0x08

#define door_o 0x20
#define door_c 0x1f

/*DSCK -PA4 DAT PA5 RST DAT*/
#define ds1302Clk GPIO_Pin_13   //????????????
#define ds1302Dat GPIO_Pin_14   //????????????
#define ds1302Rst GPIO_Pin_15   //????????????
#define ds1302Port GPIOD
/* PA 4,6,???*/

#define WrEnDisCmd  0x8e  //???/??????
#define WrEnDat     0x00 //?????
#define WrDisDat    0x80 //?????
#define OscEnDisCmd 0x80 //?????/??????
#define OscEnDat    0x00 //???????
#define OscDisDat   0x80 //???????
#define WrMulti     0xbe //???????????
#define WrSingle    0x84 //???????????
#define RdMulti  0xbf //???????????
struct tm /*is a structure used to hold the time and date. Its members are as follows: */
{
  unsigned char sec; /* seconds after the minute (0 to 61) */
  unsigned char min; /* minutes after the hour (0 to 59) */
  unsigned char hour; /* hours since midnight (0 to 23) */
  unsigned char day; /* months since January (0 to 11) */
  unsigned char year; /* years since 1900 */
  unsigned char month; /* days since Sunday (0 to 6 Sunday=0) */
  unsigned char date; /* days since January 1 (0 to 365) */
};

unsigned char num_digi7[] =
  { 0x3F, 0x06, 0x5B, 0x4F, 0x66, //0,1,2,3,4
      0x6D, 0x7D, 0x07, 0x7F, 0x6F, //5,6,7,8,9
      0x77, 0x7C, 0x39, 0x5E, 0x79, //A,b,C,d,E
      0x71, 0x80, 0x40, 0x54, 0x00, //F,.,-,n,
    };

unsigned int temp_adcs[] =
  { 121, 128, 135, 143, 151, 159, 168, 177, 186, 196, 206, 217, 229, 241, 253,
      266, 279, 293, 307, 322, 338, 354, 371, 388, 406, 425, 444, 464, 484, 505,
      527, 549, 573, 596, 646, 646, 672, 698, 725, 753, 784, 810, 840, 870, 901,
      933, 965, 998, 1031, 1064, 1098, 1133, 1169, 1205, 1241, 1278, 1315, 1352,
      1393, 1427, 1465, 1503, 1542, 1581, 1620, 1659, 1698, 1737, 1776, 1815,
      1854, 1893, 1932, 1971, 2009, 2048, 2086, 2124, 2162, 2199, 2236, 2274,
      2311, 2347, 2382, 2417, 2454, 2488, 2522, 2555, 2588, 2620, 2654, 2685,
      2716, 2746, 2778, 2807, 2836, 2864, 2892, 2921, 2946, 2974, 3000, 3027,
      3051, 3077, 3100, 3124, 3145, 3170, 3192, 3212, 3235, 3255, 3273, 3294,
      3313, 3332, 3351, 3368, 3384, 3401, 3418, 3435, 3450, 3467, 3482, 3497,
      3512, 3524, 3539, 3552, 3564, 3576, 3589, 3602, 3614, 3624, 3637, 3646,
      3656, 3669, 3679, 3689, 3696, 3706, 3716, 3726, 3733, 3743, 3750, 3757,
      3767, 3774, 3781, 3788, 3795, 3802, 3809, };

/* Private function prototypes -----------------------------------------------*/
void System_Periodic_Handle(void);
void test_i2c(void);

void
MENU_int1(void);
void
MENU_int(void);
void
set_date(unsigned char* te);
void
Read_RTC(void);
void
show_rtc(void);
void
i2c_rr(void);

void
melloc_ar(void);

static err_t
close_cons(void *arg, struct tcp_pcb *pcb, u16_t len);

void client_write_s(void);

err_t ee(void *arg, struct tcp_pcb *tpcb, err_t err);

void
clear_delayfn(void);
void
WriteByte_1302(u8 CmdDat, u8 Num, u8 *pSend);
void
RecByte_1302(u8 CmdDat, u8 Num, u8 *pRec);
void
Ds1302_int(void);
void
uDelay(unsigned char i);

void page_recv(u8_t num, hc_errormsg errormsg, char * page, u16_t len);

struct tm *tm;
/* Private functions ---------------------------------------------------------*/
#define i1 "158614"
#define i2 "158615"
#define i3 "158616"
#define i4 "158617"

#define h1 "fail"
#define h2 "pass"

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */

int
main(void)
{

  //TCS
  
  uint16_t X;
  uint16_t Y;
  uint32_t timc;
  struct tm *time_buf;
  time_t timer;
  timer = 00;
    /* Check if the system has resumed from WWDG reset */

  //TCS
  /* Setup STM32 system (clocks, Ethernet, GPIO, NVIC) and STM3210C-EVAL resources */
  System_Setup();
  

  
  out_off(d7);
  td1 = 0;
  td2 = 0;
  td3 = 0;
  def = 0;
  start = 1;
  locks = 1;
  eemodes = 0;
  modes = 0;
  locks_d = 0;
  err_c[0] = 0;
  err_c[1] = 0;
  err_c[2] = 0;
  err_c[3] = 0;


  //IP4_ADDR(&ipaddr, 200,200,2,95); // 10.1.1.8 //  200,200,2, 95 //
  //IP4_ADDR(&netmask, 255, 255, 255, 0);
  //IP4_ADDR(&gw, 10, 1, 1, 1); //10, 1,1, 1
  //  
  
	IP4_ADDR(&ip_ipaddr, 200,200,2,95); // 10.1.1.8 //  200,200,2, 95 //
  IP4_ADDR(&ip_netmask, 255, 255, 255, 0);
  IP4_ADDR(&ip_gw, 200, 200, 2, 1); //10, 1,1, 1
 /*
  IP4_ADDR(&ip_ipaddr, 192,168,1,8); // 10.1.1.8 //  200,200,2, 95 //
  IP4_ADDR(&ip_netmask, 255, 255, 255, 0);
  IP4_ADDR(&ip_gw, 192, 168, 1, 1); //10, 1,1, 1
*/
  //ET_STM32_USART_Init();

  /* Initilaize the LwIP satck */
  modes = 0;
  ET_STM32_USART_Init(); // add at
  
  LwIP_Init();
  
  HelloWorld_init();

    
  httpd_init();
  //tftpd_init();
  //client_init();
  tr = 0;
  
  //printU(0);
  melloc_ar();
  

  I2C_EE_Init();
  // buz_on;
  out_on(d7);
  led_ = 0xff;

  //i2c_rr();
	read_ebuff(ebuff);
	  //printU("4");
	
	
  //'Call I2c_mcp23016_write(&H04 , &Hff , &Hff) 'Invert All Input Polarities
  //'Call I2c_mcp23016_write(&H0a , &H01 , &H01) 'Initialize Iares , For Fast Input Scan Mode

  //write_to_MCP(mcp1, IPOL0, 0x00, 0xf0); // NonInvert all input polarities if active low.
  write_to_MCP(mcp1, 0x06, 0x00, 0xf0); //initiallize the ouput latch
  write_to_MCP(mcp1, 0x02, 0x00, 0x0f); //initiallize it so the LSBs outputs are  & MSBs are inputs

  write_to_MCP(mcp2, 0x06, 0x00, 0x00); //initiallize it so the LSBs outputs are  & MSBs are inputs 7f all
  write_to_MCP(mcp2, 0x02, 0x00, 0x00); //initiallize the ouput latch

  //write_to_MCP(mcp3, 0x06, 0x00, 0xf0);	  //initiallize the ouput latch
  //write_to_MCP(mcp3, 0x02, 0x00, 0xff);	  //initiallize it so the LSBs outputs are  & MSBs are inputs

  //write_to_MCP(mcp4, 0x06, 0x00, 0x00);	  //initiallize it so the LSBs outputs are  & MSBs are inputs 7f all
  //write_to_MCP(mcp4, 0x02, 0x00, 0x00);	  //initiallize the ouput latch

  Ds1302_int();
	Read_RTC();
  //show_rtc();

  //I2C_EE_BufferRead(ebuff,0x01,30);

  clear_delayfn();
  delay_fin[0] = 1;
  start = 1;
  adc_p1 = 0;
  adc_p2 = 0;
  sdoor = 0;
  //test_i2c();
  // MENU_int1();
  //printU(3);
  buz_off;
  //IP4_ADDR(&server, 192,168,1,2);
	tsa = 0;
	modes = 0;
	
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {

    RCC_ClearFlag();
	start = 0;
	err_c[3] = 1;
  }
  else
  {

  }
  
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  IWDG_SetPrescaler(IWDG_Prescaler_32);

  IWDG_SetReload(349);

  IWDG_ReloadCounter();

  IWDG_Enable();
  
  MENU_int1();
  //set_temp(1);
  //Read_RTC();
  //show_rtc();

  // test_i2c();
  //STM_EVAL_LEDToggle(1);

  //eemodes=1;
  //while(eemodes);

  //  set_led_on(led_2);
  //  set_led_on(led_3);
  //set_date(0,20,22,3,13,9,11);
  // 
  ssa = 0;
  
  //ET_STM32_USART_Init();
  
  while (1)
    {
		//sprintf(uart_buf, "mcp2\r\n");
		//printUART1((unsigned int *) uart_buf);
		//printUART2((unsigned int *) uart_buf);
		System_Periodic_Handle();
      /* Periodic tasks */
    }
  return (0);

}

static void
close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{

  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}
/*-----------------------------------------------------------------------------------*/
static void
send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;

  /* We cannot send more data than space avaliable in the send
   buffer. */
  if (tcp_sndbuf(pcb) < hs->left)
    {
      len = tcp_sndbuf(pcb);
    }
  else
    {
      len = hs->left;
    }

  err = tcp_write(pcb, hs->file, len, 0);

  if (err == ERR_OK)
    {
      hs->file += len;
      hs->left -= len;
    }
}
static err_t
http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  hs = arg;

  if (hs->left > 0)
    {
      send_data(pcb, hs);
    }
  else
    {
      close_conn(pcb, hs);
    }

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static void
conn_err(void *arg, err_t err)
{
  struct http_state *hs;
  hs = arg;
  	
  mem_free(hs);
}

err_t ee_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
		tcp_abort (pcb);

	tcp_recved(pcb, p->tot_len);
	
			  tcp_close(pcb);
			  
			mem_free(pcb);
	

	
}	
err_t ee(void *arg, struct tcp_pcb *pcb, err_t err)
{
	//char  *headers;
	char *Page="test.php?v=123456789012345678901234567890123456789012345678901234567890";
	struct http_state *hs;

  /* Allocate memory for the structure that holds the state of the
   connection. */
  hs = mem_malloc(sizeof(struct http_state));

  if (hs == NULL)
    {
      return ERR_MEM;
    }

  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* Tell TCP that this is the structure we wish to be passed for our
   callbacks. */

	
	 hs->file = malloc(19 + strlen(Page));
     sprintf(hs->file,"GET /%s HTTP/1.0\r\n\r\n", Page);
	hs->left = strlen(hs->file );
	// tcp_recv(pcb, ee_recv);
	  
    tcp_write(pcb, hs->file, hs->left, 1);
    tcp_output(pcb);
	tcp_close(pcb);
mem_free(hs);
			  return ERR_OK;

}

void client_write_s(void)
{
	struct tcp_pcb *pcb;
	struct ip_addr server;
		char *Page="test.php?v=123456789012345678901234567890123456789012345678901234567890";
	struct http_state *hs;
hs = mem_malloc(sizeof(struct http_state));


  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* Tell TCP that this is the structure we wish to be passed for our
   callbacks. */

	
	 hs->file = malloc(19 + strlen(Page));
     sprintf(hs->file,"GET /%s HTTP/1.0\r\n\r\n", Page);
	hs->left = strlen(hs->file );
	// tcp_recv(pcb, ee_recv);


  IP4_ADDR(&server, 192,168,1,2);

  pcb = tcp_new(); // create tcp pcb
  
  	if(tcp_bind(pcb, IP_ADDR_ANY, 0) == ERR_OK)
	{
		tcp_connect(pcb, &server, 80, NULL);
		tcp_write(pcb, hs->file, hs->left, 1);
		
		
	}
	tcp_close(pcb);
	
mem_free(hs);

}

void
int_mcp(unsigned char add)
{
  write_to_MCP(add, IPOL0, 0x00, 0x00); // NonInvert all input polarities if active low.
  write_to_MCP(add, IPOL1, 0x00, 0x00); // NonInvert all input polarities if active low.
  write_to_MCP(add, IOCON0, 0x01, 0x01); // IARES(1) to max. activity detection time.
  write_to_MCP(add, IOCON1, 0x01, 0x01); // IARES(1) to max. activity detection time.
  write_to_MCP(add, OLAT0, 0xFF, 0xFF); // Update o/p latch that controls the output.
  write_to_MCP(add, OLAT1, 0xFF, 0xFF); // Update o/p latch that controls the output.

}

void
set_ebuff(char* eb)
{

  char ebb[8];
  unsigned char iss, issa, ai, i;
  issa = 0;
  ai = 0;

  for (i = 0; i < 8; i++)
    {
      for (iss = 0; iss < 8; iss++)
        ebb[iss] = 0;

      for (iss = 0; iss < 8; iss++)
        ebb[iss] = eb[iss + (ai)];

      I2C_EE_BufferWrite(ebb, issa, 8);
      issa += 0x10;
      ai += 8;

    }

}
void
read_ebuff(char* eb)
{
  char ebb[8];
  char ebba[48];
  unsigned char iss, issa, ai, i;

  issa = 0;
  ai = 0;
  for (i = 0; i < 8; i++)
    {
      for (iss = 0; iss < 8; iss++)
        ebb[iss] = 0;

      I2C_EE_BufferRead(ebb, issa, 8);

      for (iss = 0; iss < 8; iss++)
        *(eb + (iss + ai)) = ebb[iss];

      ai += 8;
      issa += 0x10;
    }

}

void
tempc_read(char* t1, char* t2)
{
  *t1 = *tempc1;
  *t2 = *tempc2;
}
void
leds_set(unsigned char add, unsigned char val)
{
  leds[add] = val;

}
void
delay_read(char* d_f, char* d_h, char* d_m, char* d_s)
{
  //while (*d_f != '\0')
//	{
  *d_f = *delay_fin;
  *d_h = *delh;
  *d_m = *delm;
  *d_s = *dels;
  //d_f++;d_h++;d_m++;d_s++;delay_fin++;delh++;delm++;dels++;
  //}
}
void
set_start(unsigned char s)
{
  start = s;
}
unsigned char
chk_start(void)
{
  return start;
}

void
set_sdoor(unsigned char sd)
{
  sdoor = sd;
}
unsigned char
chk_sdoor(void)
{
  return sdoor;
}

void
setmodes(unsigned char m)
{
  modes = m;
}
unsigned char
chk_modes(void)
{
  return modes;
}
void
i2c_rr(void)
{
  unsigned char i;
  unsigned char adi;

  i = 0;
  /*
   ebuff[i++] = 0;
   ebuff[i++] = 1;
   ebuff[i++] = 0;
   ebuff[i++] = 1;
   ebuff[i++] = 0;
   ebuff[i++] = 1;
   ebuff[i++] = 0;
   ebuff[i++] = 1;

   ebuff[i++] = -24;
   ebuff[i++] = -22;

   ebuff[i++] = 30;

   ebuff[i++] = 1;
   ebuff[i++] = 1;

   ebuff[i++] = 19;
   ebuff[i++] = 00;
   ebuff[i++] = 0;
   ebuff[i++] = 5;
   ebuff[i++] = 35;

   ebuff[i++] = 35;
   ebuff[i++] = 30;	// 19
   //34:35:60:50:34:35:61:60:0:0
   ebuff[i++] = 34;	// 20
   ebuff[i++] = 35;
   ebuff[i++] = 60;
   ebuff[i++] = 50;
   ebuff[i++] = 34;
   ebuff[i++] = 35;
   ebuff[i++] = 61;
   ebuff[i++] = 60;
   ebuff[i++] = 0;
   ebuff[i++] = 0;

   ebuff[i++] =  0;	// 30
   ebuff[i++] =  0;	// 31

   ebuff[i++] =  0;	// 32
   ebuff[i++] =  0;	// 33
   ebuff[i++] =  0;	// 33
   ebuff[i++] =  0;	// 35
   ebuff[i++] =  0;	// 36
   ebuff[i++] =  0;	// 37
   ebuff[i++] =  0;	// 38
   ebuff[i++] =  0;	// 39
   */

//set_ebuff(ebuff);
  read_ebuff(ebuff);

  /*
   I2C_EE_ByteWrite(ebuff, 0x01);
   for(i=0;i<40;i++)
   {
   I2C_EE_ByteWrite(ebuff[i], 0x01+i);

   }

   for(i=0;i<40;i++)
   {
   I2C_EE_BufferRead(ebuff[i],0x01+i,1);

   }
   */

}

void
GetADC1Channel(unsigned char chanel)
{
  ADC_RegularChannelConfig(ADC1, chanel, 1, ADC_SampleTime_55Cycles5); // Configure channel
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); // Start the conversion
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
    ; // Wait until conversion completion
//   return ADC_GetConversionValue(ADC1);// Get the conversion value
}
void
check_temp()
{
  int i;
  int fa;
  int tem;
  int cal;
  unsigned int ADCVal = 0;

  uint32_t adcs1, adcs2;
  adcs1 = 0;
  adcs2 = 0;

  for (fa = 0; fa < 100; fa++)
    {
      GetADC1Channel(ADC_Channel_4);
      ADCVal = ADC_GetConversionValue(ADC1);
      adcs1 += ADCVal;

      GetADC1Channel(ADC_Channel_6);
      ADCVal = ADC_GetConversionValue(ADC1);
      adcs2 += ADCVal;
    }

  adc_p1 = adcs1 / 100;
  adc_p2 = adcs2 / 100;

  for (i = 0; i < 161; i++)
    {
      if (adc_p1 <= temp_adcs[i])
        {
          if ((temp_adcs[i] - adc_p1) > (adc_p1 - temp_adcs[i - 1]))
            {
              temp_p1 = i - 49.5;
              break;
            }
          temp_p1 = i - 50;
          break;

        }

    }

  for (i = 0; i < 161; i++)
    {
      if (adc_p2 <= temp_adcs[i])
        {
          if ((temp_adcs[i] - adc_p2) > (adc_p2 - temp_adcs[i - 1]))
            {
              temp_p2 = i - 49.5;
              break;
            }
          temp_p2 = i - 50;
          break;
        }
    }

  cal = ebuff[60] / 2;
  temp_p1 += cal;
  
  cal = ebuff[61] / 2;
  temp_p2 += cal;
  
  if(err_com == 2)
  {
	  if(ebuff[22] < temp_p1)
	  {
		  start = 3;
		  err_c[2] = 1;
	  }
	  else err_com = 0;
	  
	  
  }	  
    
  if(temp_p1<-40)
  { 
	err_c[0] = 1;
	start = 3;
	
  }	  
  else err_c[0] = 0;
      
  if(temp_p2 < -40) err_c[1] = 1;
  else err_c[1] = 0;

  if ((temp_tp1 - temp_p1) > 0.5)
    temp_tp1 -= 0.5;
  else if ((temp_tp1 - temp_p1) < -0.5)
    temp_tp1 += 0.5;
  else
    temp_tp1 = temp_p1;

  if ((temp_tp2 - temp_p2) > 0.5)
    temp_tp2 -= 0.5;
  else if ((temp_tp2 - temp_p2) < -0.5)
    temp_tp2 += 0.5;
  else
    temp_tp2 = temp_p2;

  temp_p1 = temp_tp1;
  temp_p2 = temp_tp2;

  tem = temp_p1 * 10;

  if (temp_p1 < 0)
    {
      tem *= -1;
      sprintf(tempc1, "-%d%d.%d", tem / 100, (tem / 10) % 10, (tem % 10));
      if (locks == 1)
        temp_[0] = 17;
    }
  else
    {
      sprintf(tempc1, " %d%d.%d", tem / 100, (tem / 10) % 10, (tem % 10));
      if (locks == 1)
        temp_[0] = 19;
    }

  if (locks == 1)
    temp_[1] = (tem / 10) % 10;
  if (locks == 1)
    temp_[2] = tem / 100;

  tem = temp_p2 * 10;
  if (temp_p2 < 0)
    {
      tem *= -1;
      sprintf(tempc2, "-%d%d.%d", tem / 100, (tem / 10) % 10, (tem % 10));
      if (locks == 1)
        temp_1[0] = 17;
    }
  else
    {
      sprintf(tempc2, " %d%d.%d", tem / 100, (tem / 10) % 10, (tem % 10));
      if (locks == 1)
        temp_1[0] = 19;
    }
  if (locks == 1)
    temp_1[1] = (tem / 10) % 10;
  if (locks == 1)
    temp_1[2] = tem / 100;

if(err_c[0]==1)
  { 
	sprintf(tempc1, " E 01");
  }	  
  if(err_c[1]==1)
  { 
	sprintf(tempc2, " E 02");
  }	  

}

void
Read_RTC(void)
{
  u8 i;
  //DS1307_Read(rtc);
  RecByte_1302(RdMulti, 8, rtc);

  for (i = 0; i < 7; i++)
    rtc[i] = (rtc[i] / 16) * 10 + rtc[i] % 16;

  sprintf(ubuff, "%02d-%02d-%02d %02d:%02d:%02d - %d:%d:%d:%d", rtc[4], rtc[5],
      rtc[6], rtc[2], rtc[1], rtc[0], butt[0], butt[1], butt[2], butt[3]);

}

void
set_temp(int tsi)
{

  if (tsi < 0)
    {
      temp_[0] = 17;
      tsi = tsi * -1;
    }
  else
    {
      temp_[0] = 19;
    }

  if (tsi > 9)
    {
      temp_[1] = tsi % 10;
      temp_[2] = tsi / 10;
    }
  else
    {
      temp_[1] = tsi % 10;
      temp_[2] = 19;
    }
  show_seg1(temp_);

}
void set_led_off(unsigned char pbuf,char tsi)
{
  tsi = 1 << tsi;
  led_ |= tsi;

  write_to_MCP(mcp1, OLAT0, num_digi7[pbuf], led_);
//write_to_MCP(mcp3, OLAT0, num_digi7[pbuf], led_);

}

void set_led_on(unsigned char pbuf,char tsi)
{
  tsi = 1 << tsi;
  led_ &= ~tsi;
  write_to_MCP(mcp1, OLAT0, num_digi7[pbuf], led_);
//write_to_MCP(mcp3, OLAT0, num_digi7[pbuf], led_);
}

void
show_seg1(unsigned char *pbuf)
{
  unsigned char i;
  write_to_MCP(mcp1, OLAT0, (num_digi7[pbuf[0]]), (led_));
  write_to_MCP(mcp2, OLAT0, (num_digi7[pbuf[1]]), (num_digi7[pbuf[2]]));
  //write_to_MCP(mcp3, OLAT0, (num_digi7[pbuf[0]]), ( led_));
  //write_to_MCP(mcp4, OLAT0,(num_digi7[pbuf[1]]),(num_digi7[pbuf[2]]));

  for (i = 0; i < 4; i++)
    {
      if (leds[i] == 1)
        {
          set_led_on(pbuf[0],i);
          leds[i] = 3;
        }
      else if (leds[i] == 2)
        {
          set_led_off(pbuf[0],i);
          leds[i] = 0;
        }
    }
}
void
show_rtc()
{
  sprintf(uart_buf, "%d-%d-%d %d:%d:%d day %d\r\n", rtc[3], rtc[4], rtc[6],
      rtc[2], rtc[1], rtc[0], rtc[5]);
  printUART1((unsigned int *) uart_buf);
  printUART2((unsigned int *) uart_buf);

}
void
set_date(unsigned char *te)
{
  u8 i;

  u8 temp;
  //te[i]=((te[i]/10)<<4)|(te[i]%10);
	Ds1302SendBuf[7] = 33;
  for (i = 0; i < 7; i++)
    {
      temp = te[i] / 10;
      te[i] = te[i] % 10 + temp * 16;
    }

  WriteByte_1302(WrMulti, 8, te);

}

/**
 * @brief  Inserts a delay time.
 * @param  nCount: number of 10ms periods to wait for.
 * @retval None
 */

void
printU(unsigned char chh)
{
  sprintf(uart_buf, "%d\r\n", chh);
  printUART1((unsigned int *) uart_buf);
  printUART2((unsigned int *) uart_buf);
}
void
printsU(int* chh, u8 ii)
{
  int i;
  for (i = 0; i < ii; i++)
    {
      sprintf(uart_buf, "%d:", chh[i]);
      printUART2((unsigned int *) uart_buf);

    }
  sprintf(uart_buf, "\r\n");
  //printUART2((unsigned int *)uart_buf);

  sprintf(ubuff1, "%i:%i:%i:%i:%i:%i:%i:%i:\r\n", chh[0], chh[1], chh[2],
      chh[3], chh[4], chh[5], chh[6], chh[7]);

}
void
Delay1(uint32_t nTime)
{
  /* Capture the current local time */
  ocalTime1 = 0;

  /* wait until the desired delay finish */
  while (ocalTime1 < nTime)
    {

    }

}

void
Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = ocalTime + nCount;

  /* wait until the desired delay finish */
  while (timingdelay > ocalTime)
    {

    }
}

/**
 * @brief  Updates the system local time
 * @param  None
 * @retval None
 */
void
Time_Update(void)
{
  ocalTime += SYSTEMTICK_PERIOD_MS;
  ocalTime1 += SYSTEMTICK_PERIOD_MS;
  octime++;
  del_ds++;
  
    /* Reload IWDG counter */
  IWDG_ReloadCounter();
  
}
unsigned char tii;
void
time_cal(void)
{
  int i;
  if (octime > 100) //120
    {
	
			if(tii++ >= 60)
			{
				if(err_com == 1)
				{
					delay_com_start--;
					if(delay_com_start <= 0)
					err_com = 2;
				}
				
				//hc_open(server,"test.php?v=123456789012345678901234567890123456789012345678901234567890","p=32",page_recv);
				
				tii=0;
			}
		

		
		
      octime = 0;
      for (i = 0; i < 5; i++)
        {
          if (delay_fin[i] == 2)
            {
              if (dels[i] > 0)
                {
                  dels[i]--;
                  if (delm[i] > 0 && dels[i] <= 0)
                    {
                      delm[i]--;
                      dels[i] = 59;
                    }
                  else if (delh[i] > 0 && delm[i] <= 0)
                    {
                      delh[i]--;
                      delm[i] = 59;
                      dels[i] = 59;
                    }
                  else if (delh[i] <= 0 && delm[i] <= 0 && dels[i] <= 0)
                    {
                      if (i == 1)
                        delay_fin[i] = 0;
                      else if (i == 2)
                        {
                          delay_fin[i] = 3;
                          if (sdoor == 8 || sdoor == 11)
                            {
                              sdoor = 9;
                              buz_off;
                            }
                        }
                      else if (i == 3)
                        {
                          delay_fin[i] = 3;
                        }
                      else
                        {
                          delay_fin[i] = 1;
						  def = 0;
                        }

                      

                    }

                }

              if (sdoor >= 9 && i == 2)
                {
                  if (dels[i] == 55)
                    buz_on;
                  if (dels[i] == 54)
                    buz_off;

                  if (dels[i] == 45)
                    buz_on;
                  if (dels[i] == 44)
                    buz_off;

                  if (dels[i] == 35)
                    buz_on;
                  if (dels[i] == 34)
                    buz_off;

                  if (dels[i] == 25)
                    buz_on;
                  if (dels[i] == 24)
                    buz_off;

                  if (dels[i] == 15)
                    buz_on;
                  if (dels[i] == 14)
                    buz_off;

                  if (dels[i] == 5)
                    buz_on;
                  if (dels[i] == 4)
                    buz_off;
                }

            }
        }
		
    }
		if(delay_button > 8)
		{
			if(start == 1)
			{
				start = 3;
				mode_dis = 5;
			}				
			else if(start == 0)
			{
				start = 5;
				mode_dis = 4;
			}				
			delay_button=0;
			
		}
		else if(delay_button>2)
		{
			mode_dis = 1;
			dis_delay=0;
		}

}

/**
 * @brief  Handles the periodic tasks of the system
 * @param  None
 * @retval None
 */

void
System_Periodic_Handle(void)
{
  /* Update the LCD display and the LEDs status */
  /* Manage the IP address setting */
  Display_Periodic_Handle(ocalTime);

  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(ocalTime);

}
void
melloc_ar(void)
{

  delay_fin = (unsigned char*) malloc(10 * sizeof(unsigned char));
  delh = (unsigned char*) malloc(10 * sizeof(unsigned char));
  delm = (unsigned char*) malloc(10 * sizeof(unsigned char));
  dels = (unsigned char*) malloc(10 * sizeof(unsigned char));

}
void
clear_delayfn(void)
{
  u8 i;
  for (i = 0; i < 10; i++)
    {
      delay_fin[i] = 0;
      delh[i] = 0;
      delm[i] = 0;
      dels[i] = 0;
    }

  modes = 0;

}
/* Check modes and status */
void
func_chk(unsigned char mod)
{
  unsigned char mi, i;
  unsigned char chk_day;
  mi = 0;
  unsigned char em, ep;
  unsigned char dat[7];

  /*  start  ******************************************************/
  if (start == 1)
    {
      if (delay_fin[0] == 1) // main recipe
        {
          if (modes >= 4)
            modes = 1;
          em = modes * 2;
          //printU(em);
          if (em >= 8)
            em = 0;
			
          delh[0] = ebuff[em];
          delm[0] = ebuff[em + 1];
		  
			if(modes==2 && err_c[1]==1)
			{
				delh[0] = 0;
				delm[0] = ebuff[24];
			}	
			
          if (delm[0] == 0 && delh[0] > 0)
            {
              delh[0]--;
              delm[0] = 59;
              dels[0] = 59;
              delay_fin[0] = 2;
            }
          else if (delh[0] <= 0 && delm[0] <= 0)
            {
              dels[0] = 1;
              delay_fin[0] = 1;
            }
          else
            {
              delm[0]--;
              dels[0] = 59;
              delay_fin[0] = 2;
            }

          outpp(ebuff[(modes + 37)]); // 22
          modes++;

        }
      else
        {

          //chk_ru(mod);
        }

      if (modes == 2) // recipe 1 check temp
        {
          //buf_port = GPIO_ReadInputData(GPIOE);
          if (temp_p1 < ebuff[8])
            {
              outpp(ebuff[41]);
            }
          else if (temp_p1 > ebuff[9])
            {
              outpp(ebuff[42]);
            }
        }
      if (modes == 3) // defrost check temp
        {
          if (def == 0)	// normal defrost
            {
              if (temp_p2 > ebuff[10])
                {
                  delay_fin[0] = 1;
				  def = 0;
                }
            }
          else			// big defrost
            {
              if (temp_p2 > ebuff[17])
                {
                  delay_fin[0] = 1;
                  def = 0;
                }
            }

          if (temp_p1 > ebuff[18]) // lock display
            {
              locks = 5;
            }
          else if (temp_p1 < ebuff[19])
            {
              locks = 1;
            }

        }

      if (delay_fin[2] == 1 || delay_fin[2] == 3) // Door open delay
        {
          delh[2] = 0;

          if (delay_fin[2] == 1)
            delm[2] = ebuff[11];
          else if (delay_fin[2] == 3)
            {
              if (sdoor == 9)
                {
                  sdoor == 11;
                  //locks = 0;
                  //buf_port = GPIO_ReadInputData(GPIOE);
                  //out_p = ebuff[45] & (buf_port >> 9);

                  //outpp(out_p);
                  delm[2] = 1;

                }
              else
                {
                  delm[2] = ebuff[12];
                  //buf_port = GPIO_ReadInputData(GPIOE);
                  //out_p = ebuff[44] & (buf_port >> 9);
                  //outpp(out_p);

                  sdoor = 8;
                }
            }

          if (delm[2] == 0 && delh[2] > 0)
            {
              delh[2]--;
              delm[2] = 59;
              dels[2] = 59;
            }
          else
            {
              delm[2]--;
              dels[2] = 59;
            }
          delay_fin[2] = 2;
          //	outpp(eep_b[mod+29]);
        }
	
      if (sdoor == 1) // door open
        {
          if (delay_fin[2] == 0)
            {
              buf_port = GPIO_ReadInputData(GPIOE);
              out_p = ebuff[43] & (buf_port >> 9);
              outpp(out_p);
              locks_d = 1;
              //locks = 0;
              delay_fin[2] = 1;

            }
        }
      else if (sdoor == 0)
        {
			
        }
		/*
		chk_day = 0;
		for(chk_day=1;chk_day<8;chk_day++)
		{
			if(ebuff[46] && 1<<i)
			{
				chk_day = i;
				break;
			}
		}	
		*/
      
      // if((ebuff[29] & 1<<(rtc[5]-1)) == 1<<(rtc[5]-1) )		
       if(ebuff[46] && 1<<(rtc[3]) )	//big defrost chk
	   {

		   if(ebuff[13]==rtc[2] && ebuff[14]==rtc[1] && def == 0)
		   {
			   delh[0] = ebuff[15];
			   delm[0] = ebuff[16];

			   if(delm[0] == 0 && delh[0]>0)
			   {
				   delh[0]--;
				   delm[0] = 59;
				   dels[0] = 59;
			   }
			   else
			   {
				   delm[0]--;
				   dels[0] = 59;
			   }
			   
			   if(err_c[1]==1)
			   {
					delh[0] = 0;
					delm[0] = ebuff[24] - 1;
					dels[0] = 59;
			   }

			   delay_fin[0] = 2;
			   def = 1;
			   outpp(ebuff[(39)]);
			   modes=3;
		   }
       }
       

      if (delay_fin[3] == 1)
        {
          delh[3] = 0;
          delm[3] = 1;
          if (delm[3] == 0 && delh[3] > 0)
            {
              delh[3]--;
              delm[3] = 59;
              dels[3] = 59;
            }
          else
            {
              delm[3]--;
              dels[3] = 59;
            }
          delay_fin[3] = 2;
          //	outpp(eep_b[mod+19]);
        }
      if (delay_fin[3] == 2)
        {
          if (temp_p1 < 22)
            {
              delay_fin[3] = 0;
              delh[3] = 0;
              delm[3] = 0;
              dels[3] = 0;
            }

        }
      if (delay_fin[3] == 3) // delay com on
        {
          start = 3;
        }
	if (temp_p1 < ebuff[19])
            {
              locks = 1;
            }
    }

  /*e  start  ******************************************************/

  else if (start == 3) // stop
    {
      clear_delayfn();
      start = 0;
      modes = 0;
      sdoor = 0;
      //i2c_rr();
      /*for(i=0;i<10;i++)
       {
       delh[i] = 0;
       delm[i] = 0;
       dels[i] = 0;

       }*/
      outpp(0);
    }
  else if (start == 5)  // start
    {
      clear_delayfn();
      modes = 0;
      delay_fin[0] = 1;
      start = 1;
	  err_c[2] = 0;
	  err_c[3] = 0;
	  locks = 1;
    }

  if (delay_fin[1] == 1) // delay com off
    {
      delh[1] = 0;
      delm[1] = ebuff[23]-1;
      dels[1] = 59;
      delay_fin[1] = 2;
      //	outpp(eep_b[mod+19]);
    }

  if (start == 1)
    {
  if (sdoor == 1) // door open
    {

      if (delay_fin[2] == 0)
        {
          buf_port = GPIO_ReadInputData(GPIOE);
          out_p = ebuff[43] & (buf_port >> 9);//26
          outpp(out_p);
          locks_d = 1;
          //locks = 0;
          delay_fin[2] = 1;

        }

      //out_on(k5);
    }
  else if (sdoor == 0)
    {
      //outpp();
      delh[2] = 0;
      delm[2] = 0;
      dels[2] = 0;
      //out_off(k5);
    }
	
	}	

}
void
eemode(unsigned char mods)
{
  //I2C_EE_BufferRead(eep_b,mods,40);
}
void
set_dat_o(unsigned char i)
{
  ds1302Port->BSRR = ds1302Dat;

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = ds1302Dat;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  if (i == 1)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  GPIO_Init(ds1302Port, &GPIO_InitStructure);
}

void
SendDat_1302(u8 Dat)
{
  u8 i;
  u8 cTmp;
//printU(Dat);
  for (i = 0; i < 8; i++)
    {
      ds1302Port->BRR = ds1302Clk;
      uDelay(1);

      cTmp = Dat & 0x01;

      if (cTmp == 0x01) //1
        ds1302Port->BSRR = ds1302Dat;
      else
        ds1302Port->BRR = ds1302Dat;

      Dat >>= 1;
      uDelay(1);

      ds1302Port->BSRR = ds1302Clk;
      uDelay(1);
    }
}

void
WriteByte_1302(u8 CmdDat, u8 Num, u8 *pSend)
{
  u8 i = 0;
  ds1302Port->BRR = ds1302Rst;
  ds1302Port->BRR = ds1302Clk;

  set_dat_o(0);
  uDelay(1);
  //printU(100);
  ds1302Port->BSRR = ds1302Rst;
  uDelay(1);
  SendDat_1302(CmdDat);
  for (i = 0; i < Num; i++)
    {
      SendDat_1302(*(pSend + i));
    }
  uDelay(1);
  ds1302Port->BRR = ds1302Rst;
  uDelay(1);
  ds1302Port->BRR = ds1302Clk;
}

void
RecByte_1302(u8 CmdDat, u8 Num, u8 *pRec)
{
  u8 i, j, tmp, cTmp;
  ds1302Port->BRR = ds1302Rst;
  ds1302Port->BRR = ds1302Clk;
  uDelay(1);
  //printU(200);
  ds1302Port->BSRR = ds1302Rst;
  uDelay(1);
  SendDat_1302(CmdDat); //????
  set_dat_o(1);
  for (i = 0; i < Num; i++)
    {
      tmp = 0x00;
      for (j = 0; j < 8; j++)
        {
          ds1302Port->BRR = ds1302Clk;
          uDelay(1);

          cTmp = GPIO_ReadInputDataBit(ds1302Port, ds1302Dat);

          tmp = (cTmp << 7) | (tmp >> 1);

          ds1302Port->BSRR = ds1302Clk;
          uDelay(1);
        }
      *(pRec + i) = tmp;
      //	printU(tmp);
    }
  uDelay(1);
  ds1302Port->BRR = ds1302Rst;
  uDelay(1);
  ds1302Port->BRR = ds1302Clk;
  set_dat_o(0);
}

void
WrCmd(u8 CmdDat, u8 CmdWord)
{
  u8 CmdBuf[2];
  CmdBuf[0] = CmdWord;
  WriteByte_1302(CmdDat, 1, CmdBuf);
}

void
Ds1302_int()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = ds1302Clk | ds1302Rst;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ds1302Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = ds1302Dat;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ds1302Port, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);


  u8 Ds1302RecBuf[8]; //???????
  //printU(2);
  //WrCmd(WrEnDisCmd,WrEnDat);  //???
  //printU(3);
  //WrCmd(OscEnDisCmd,OscEnDat); //?????
  //printU(4);
  // 
  // Ds1302SendBuf
  /*
  Ds1302SendBuf[0] = 0;
  Ds1302SendBuf[1] = 8;
  Ds1302SendBuf[2] = 15;
  Ds1302SendBuf[3] = 14;
  Ds1302SendBuf[4] = 10;
  Ds1302SendBuf[5] = 7;
  Ds1302SendBuf[6] = 10;
  Ds1302SendBuf[7] = 33;
  WriteByte_1302(WrMulti, 8, Ds1302SendBuf);
  */
  //set_date(Ds1302SendBuf);
  //WriteByte_1302(WrMulti,8,Ds1302SendBuf);//??????DS1302?
  // ?????,
	
}

void
uDelay(unsigned char i)
{
  uint32_t dd;
  dd = 0;
  while (10000 > dd)
    {
      dd++;
    }
}
void
out_toggle_s(out_TypeDef out)
{
  unsigned char uii;
  uii = 1;
  unsigned char ii;
  ii = read_inp(out);
  if (start == 0)
    {
      if (out == 0)
        {
          if (ii)
            {
              leds[2] = 2;
              delay_fin[1] = 1;
            }
          else
            {
              if (delay_fin[1] == 2)
                uii = 2;
              else
                {
                  leds[2] = 1;
                }
            }
        }

      if (out == 1)
        {
          if (ii)
            leds[1] = 2;
          else
            leds[1] = 1;
        }

      if (out == 2)
        {
          if (ii)
            leds[3] = 2;
          else
            leds[3] = 1;
        }
      /*
       if(out == 5)
       {
       uii = 0;
       }
       }*/
      if (uii == 1)
        {
          out_toggle(out);
        }
    }
}
void
outpp(unsigned char val)
{

  if (sdoor == 1)
    {
      val &= ebuff[43]; // 26 29
    }
  else if (sdoor == 8)
    {
      val &= ebuff[44]; // 27 30
    }
  else if (sdoor >= 9)
    {
      val &= ebuff[45]; // 28 31
    }

  if (read_inp(k0))
    {
      if ((val & 0x01) == 0x01)
        {
          leds[2] = 1;
          delay_fin[3] = 1;
		  if(err_com <= 0)
		  {
			  err_com = 1;
			  delay_com_start = (ebuff[20]*60) + ebuff[21];
		  }
       }
      else
        {
          delay_fin[1] = 1;
          leds[2] = 2;
          delay_fin[3] = 0;
          delh[3] = 0;
          delm[3] = 0;
          dels[3] = 0;
		  err_com = 0;
		  delay_com_start = 0;
        }
    }
  else
    {
      if ((val & 0x01) == 0x01)
        {
          if (delay_fin[1] == 2)
            val &= 0xfe;
          else
            {
              leds[2] = 1;
			  if(err_com <= 0)
			  {
				  err_com = 1;
				  delay_com_start = (ebuff[20]*60) + ebuff[21];
			  }			  
			  
            }
        }
      else
        {
          leds[2] = 2;
          delay_fin[3] = 0;
          delh[3] = 0;
          delm[3] = 0;
          dels[3] = 0;
		  err_com = 0;
		  delay_com_start = 0;
        }
    }

  if ((val & 0x02) == 0x02)
    leds[1] = 1;
  else
    leds[1] = 2;

  if ((val & 0x04) == 0x04)
    leds[3] = 1;
  else
    leds[3] = 2;
  /*
   if(modes==2)		 // recipe 1 check temp
   {
   if(temp_p1 < ebuff[8])
   {
   val &=ebuff[27]; // 24
   }
   else if(temp_p1 > ebuff[9])
   {
   val &=ebuff[28]; // 28
   }
   }
   }	*/
  /*
   if(sdoor == 1)
   {
   val |= door_o;

   }
   else
   {
   val &= door_c;

   }
   */

  out_write(val);

}

void
test_i2c()
{

  int i;
  /* Test Write I2C EEPROM
   eep_buffer[0] = 0x80;
   eep_buffer[1] = 0x40;
   eep_buffer[2] = 0x20;
   eep_buffer[3] = 0x10;
   eep_buffer[4] = 0x01;
   eep_buffer[5] = 0x02;
   eep_buffer[6] = 0x04;
   eep_buffer[7] = 0x08;
   I2C_EE_BufferWrite(eep_buffer, 0x00,8);  				// Write 8 Byte From Address 0x00...

   eep_buffer[0] = 0x01;
   eep_buffer[1] = 0x03;
   eep_buffer[2] = 0x07;
   eep_buffer[3] = 0x0F;
   eep_buffer[4] = 0x1F;
   eep_buffer[5] = 0x3F;
   eep_buffer[6] = 0x7F;
   eep_buffer[7] = 0xFF;
   I2C_EE_BufferWrite(eep_buffer, 0x00,8);
   I2C_EE_BufferWrite(eep_buffer, 0x10,8); 				// Write 8 Byte From Address 0x10...
   */
  I2C_EE_WaitEepromStandbyState();
  // I2C_EE_BufferRead(eep_buffer,0x00,8);
  for (i = 0; i < 8; i++) // Read 8 Byte to Buffer
    {
      out_write(eep_buffer[i]);
      Delay1(250);
      set_temp(eep_buffer[i]);
    }

  /* Read from I2C EEPROM from EEPROM_ReadAddress1 */

//    I2C_EE_BufferRead(eep_buffer,0x10,8);
  for (i = 0; i < 8; i++) // Read 8 Byte to Buffer
    {
      out_write(eep_buffer[i]);
      Delay1(250);
    }

}
// This function ll be called by the webclient page when the page is received
void page_recv(u8_t num, hc_errormsg errormsg, char * page, u16_t len)
{

}
void
TIM3_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
      /* Pin PC.06 toggling with frequency = 73.24 Hz */
      // set_temp();
      // if( td1>=99) td1=-99;
      if (eemodes == 1)
        {
          //eemode(mod);
          eemodes = 0;
        }
      else if (eemodes == 10)
        {
          //I2C_EE_BufferWrite(ebuff,0x01,30);
          //I2C_EE_BufferRead(ebuff,0x01,30);
         
          set_ebuff(ebuff);
          read_ebuff(ebuff);
		  set_date(Ds1302SendBuf);
          eemodes = 0;
        }
	 else if (eemodes == 14)
        {
          //I2C_EE_BufferWrite(ebuff,0x01,30);
          //I2C_EE_BufferRead(ebuff,0x01,30);
         
          set_ebuff(ebuff);
          read_ebuff(ebuff);
          eemodes = 0;
        }
      else if (eemodes == 11)
        {
          read_ebuff(ebuff);
          eemodes = 0;
        }
		
		if(mode_dis == 1 )
		{
			
			dis_delay++;
			if(dis_delay >= 5)
			{
				mode_dis = 0;
				dis_delay =0;
			}
			 if(err_c[1] == 1)	
			{
					temp_1[0] = 14;
					temp_1[1] = 2;
					temp_1[2] = 19;
			}	
						
			show_seg1(temp_1);
		}
		else if(mode_dis == 5)
		{
			dis_delay++;
			if(dis_delay >= 2)
			{
				mode_dis  = 0;
				dis_delay = 0;
			}
			temp_1[0] = 0;
			temp_1[1] = 15;
			temp_1[2] = 15;
			 if(err_c[1] == 1)	
			{
					temp_1[0] = 14;
					temp_1[1] = 2;
					temp_1[2] = 19;
			}	
						
			show_seg1(temp_1);
		}	
		else if(mode_dis == 4)
		{
			dis_delay++;
			if(dis_delay >= 2)
			{
				mode_dis  = 0;
				dis_delay = 0;
			}
			
			temp_1[0] = 0;
			temp_1[1] = 18;
			temp_1[2] = 19;
			
			
			 if(err_c[1] == 1)	
			{
					temp_1[0] = 14;
					temp_1[1] = 2;
					temp_1[2] = 19;
			}	
						
			show_seg1(temp_1);
		}	
		else
		{		
			if(err_c[0] == 1)	
			{
				temp_[0] = 14;
				temp_[1] = 1;
				temp_[2] = 19;
				
			}
			else if(err_c[1] == 1)	
			{
				dis_delay++;
				if(dis_delay >= 100)
				{
					temp_[0] = 14;
					temp_[1] = 2;
					temp_[2] = 19;
					if(dis_delay >=108) dis_delay = 0;
				}
				
			}	
			else if(err_c[2] == 1)	
			{
				temp_[0] = 14;
				temp_[1] = 3;
				temp_[2] = 19;
				
			}	
			
			show_seg1(temp_);
		}		  
        

      if (!(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4))) // door open event
        {
          sdoor = 0;
          leds[0] = 2;
          if (locks_d == 1)
            {
              sdoor = 0;

				outpp(ebuff[(modes + 36)]); // 19  buf_port
			 		
			  
              locks_d = 0;
              delay_fin[2] = 0;
              delh[2] = 0;
              delm[2] = 0;
            }
        }
      else
        {
          if (sdoor == 0)
            
              sdoor = 1;
              leds[0] = 1;

            }
        

      // sprintf(uart_buf,"%d : %d  \r\n",butt[0],butt[1],butt[2]);
      // printUART2((unsigned int *)uart_buf);
      //STM_EVAL_LEDToggle(0);

      MCP_read(butt, mcp1, 0x00);
      //printsU(butt,8);

      if ((butt[1] & 0x20) == 0x20) // push button
        {
			
		delay_button = 0;
         ebuff[63] = 1;
         			if(ssa == 1)  
					 {				
						 /*
						char *Page="test.php?v=123456789012345678901234567890123456789012345678901234567890";
						struct http_state *hs;

					
						hs = mem_malloc(sizeof(struct http_state));


						hs->file = NULL;
						hs->left = 0;

						hs->file = malloc(12 + strlen(Page));
						sprintf(hs->file,"%s:%10u:", Page,tsa++);
						hs->left = strlen(hs->file );
						
						tcp_write(pub_pcb,hs->file,hs->left,1);
						mem_free(hs);
						
						*/
						 
					 }
				//client_write_s();
			//hc_open(server,"test.php?v=123456789012345678901234567890123456789012345678901234567890","p=32",page_recv);
		
		
        }
      else
        {
			delay_button++;
			
			ebuff[63] = 0;
			/*
			  struct ip_addr server;
    	  IP4_ADDR(&server, 192,168,1,2);
		  
		  hc_open(server,"test.php",NULL,NULL);
		  
		  
          ebuff[39] = 0;
          outpp(0XFF);*/
			
			
			
			
        }

      Read_RTC();

      capture1 = TIM_GetCapture1(TIM3);
      TIM_SetCompare1(TIM3, capture1 + CCR1_Val);

      //TIM_SetCompare1(TIM3, capture + 6000);
    }
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {

      TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
      check_temp();

      /* Pin PC.07 toggling with frequency = 109.8 Hz */

      //STM_EVAL_LEDToggle(1);
      capture1 = TIM_GetCapture2(TIM3);
      TIM_SetCompare2(TIM3, capture1 + CCR2_Val);
      //TIM_SetCompare2(TIM3, capture + 3000);
    }
  if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
    {
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

      /* Pin PC.08 toggling with frequency = 219.7 Hz */

      capture1 = TIM_GetCapture3(TIM3);
      TIM_SetCompare3(TIM3, capture1 + CCR3_Val);
      //TIM_SetCompare3(TIM3, capture + 1000);
    }
  if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
      //I2C_EE_BufferRead(eep_buffer,0x00,8);
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

      /* Pin PC.09 toggling with frequency = 439.4 Hz */
      time_cal();
      func_chk(modes);

      capture1 = TIM_GetCapture4(TIM3);
      TIM_SetCompare4(TIM3, capture1 + CCR4_Val);
      //TIM_SetCompare4(TIM3, capture + 500);
    }
}

void
MENU_int(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 100;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM2, 4, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);

  /* TIM IT enable */
  TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);

}

void
MENU_int1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 59999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  /* Prescaler configuration */

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  
  /* 2 bits for Preemption Priority and 2 bits for Sub Priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  


}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
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
      {}
  }
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
