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
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

//lwIP TCPIP Demo Modify For "ET-STM32F ARM KIT Board"

/* Includes ------------------------------------------------------------------*/
#include<time.h>
#include<stdio.h>
#include "stm32_eth.h"
#include "netconf.h"
#include "main.h"
#include "helloworld.h"
#include "httpd.h"
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


 
 unsigned char MCP23016_Device_Address; // MCP23016 Assigned Device Address. 
 int GPIO_port; // MCP23016 Data Port Register.
 
uint16_t capture1 = 0;
uint16_t CCR1_Val = 500;
uint16_t CCR2_Val = 400;
uint16_t CCR3_Val = 200;
uint16_t CCR4_Val = 100;

char uart_buf[512]; 
int ADCVal = 0;
unsigned int addi=0;
int tempcal[2];
unsigned int tempdel;
uint16_t buf_port;
uint8_t out_p;

int adc_p1;
int adc_p2;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t ocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
__IO uint32_t ocalTime1 = 0; 
__IO uint32_t octime = 0; 

uint32_t timingdelay;
uint32_t timingdelay1;
uint8_t eep_buffer[8];
//uint8_t eep_b[40];
uint8_t rtc[8];
uint8_t butt[8];
unsigned char locks=1;

int td1;
int td2;
int td3;
unsigned char modes;
int tr;
 
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
 
unsigned char num_digi7[]={
					0x3F, 0x06, 0x5B, 0x4F, 0x66,  //0,1,2,3,4
                    0x6D, 0x7D, 0x07, 0x7F, 0x6F,  //5,6,7,8,9
                    0x77, 0x7C, 0x39, 0x5E, 0x79,  //A,b,C,d,E
                    0x71, 0x80, 0x40, 0x54, 0x00,   //F,.,-,n,
                };
            
unsigned int temp_adcs[] = {
 121,128,135,143 ,151 ,159 ,168 ,177 ,186 ,196 ,
206 ,217 ,229 ,241 ,253 ,266 ,279 ,293 ,307 ,322 ,
338 ,354 ,371 ,388 ,406 ,425 ,444 ,464 ,484 ,505 ,
527 ,549 ,573 ,596 ,646 ,646 ,672 ,698 ,725 ,753 ,
784 ,810 ,840 ,870 ,901 ,933 ,965 ,998 ,1031 ,1064 ,
1098 ,1133 ,1169 ,1205 ,1241 ,1278 ,1315 ,1352 ,1393 ,
1427 ,1465 ,1503 ,1542 ,1581 ,1620 ,1659 ,1698 ,1737 ,
1776 ,1815 ,1854 ,1893 ,1932 ,1971 ,2009 ,2048 ,2086 ,
2124 ,2162 ,2199 ,2236 ,2274 ,2311 ,2347 ,2382 ,2417 ,
2454 ,2488 ,2522 ,2555 ,2588 ,2620 ,2654 ,2685 ,2716 ,
2746 ,2778 ,2807 ,2836 ,2864 ,2892 ,2921 ,2946 ,2974 ,
3000 ,3027 ,3051 ,3077 ,3100 ,3124 ,3145 ,3170 ,3192 ,
3212 ,3235 ,3255 ,3273 ,3294 ,3313 ,3332 ,3351 ,3368 ,
3384 ,3401 ,3418 ,3435 ,3450 ,3467 ,3482 ,3497 ,3512 ,
3524 ,3539 ,3552 ,3564 ,3576 ,3589 ,3602 ,3614 ,3624 ,
3637 ,3646 ,3656 ,3669 ,3679 ,3689 ,3696 ,3706 ,3716 ,
3726 ,3733 ,3743 ,3750 ,3757 ,3767 ,3774 ,3781 ,3788 ,
3795 ,3802 ,3809 ,
};

/* Private function prototypes -----------------------------------------------*/
void System_Periodic_Handle(void);
void test_i2c(void);
void printU(char *chh);

void MENU_int1(void);
void MENU_int(void);
void set_date(unsigned char te1,unsigned char te2,unsigned char te3,unsigned char te4,unsigned char te5,unsigned char te6,unsigned char te7);
void Read_RTC(void);
void show_rtc(void);
void i2c_rr(void);

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
int main(void)
{
    //TCS
  uint16_t X;
  uint16_t Y;
    uint32_t timc;
	struct tm   *time_buf;
    time_t timer;
timer = 00;

  //TCS
  /* Setup STM32 system (clocks, Ethernet, GPIO, NVIC) and STM3210C-EVAL resources */
  System_Setup();
out_off(d7);
td1=0;
td2=0;
td3=0;

start = 1;
locks = 1;
eemodes =0;
delay_fin[0] =0;
delay_fin[1] =0;
delay_fin[2] =0;
mod =0;
modes = 0;


    ET_STM32_USART_Init();

    // ET_STM32_USART_Init();
  /* Initilaize the LwIP satck */
  LwIP_Init();
  /* Initilaize the HelloWorld module */
  HelloWorld_init();
  /* Initilaize the webserver module */
  httpd_init();
  /* Initialize the TFTP server */
	//tftpd_init();
	client_init();
    tr=0;
    //printU("mcp");
	I2C_EE_Init();
	    i2c_rr();
    out_on(d7);
    led_=0xff;
	
	
	
	write_to_MCP(0x40, 0x02, 0x00, 0xff);	  //initiallize it so the LSBs outputs are  & MSBs are inputs
    write_to_MCP(0x40, 0x06, 0x00, 0xF0);	  //initiallize the ouput latch

	write_to_MCP(0x4E, 0x02, 0x00, 0x00);	  //initiallize the ouput latch
    write_to_MCP(0x4E, 0x06, 0x00, 0x00);	  //initiallize it so the LSBs outputs are  & MSBs are inputs 7f all

	//I2C_EE_BufferRead(ebuff,0x01,30); 
	delay_fin[0]=1;
	start = 1;
	adc_p1 = 0;
	adc_p2 = 0;
	sdoor = 0;
    //test_i2c();
    MENU_int1();
   //set_temp(1);
	// Read_RTC();
    show_rtc();
	
	// test_i2c();
	//STM_EVAL_LEDToggle(1);
	
         
	//eemodes=1;
	//while(eemodes);
        
      //  set_led_on(led_2);
      //  set_led_on(led_3);
	//set_date(0,20,22,3,13,9,11);

  while (1)
  {
    System_Periodic_Handle();
	/* Periodic tasks */
  }
}
void i2c_rr(void)
{
	unsigned char i;
	unsigned char adi;
	
	i=0;
	ebuff[i++] = 0;
	ebuff[i++] = 1;
	ebuff[i++] = 0;
	ebuff[i++] = 5;
	ebuff[i++] = 0;
	ebuff[i++] = 5;
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
	
/*
	for(i=0;i<40;i++)
	{
		I2C_EE_ByteWrite(ebuff[i], 0x01+i);
	
	}		

	for(i=0;i<5;i++)
	{
		I2C_EE_BufferRead(ebuff[i],0x01,1);
		
	}		*/
	
}
void GetADC1Channel(unsigned char chanel)
{
   ADC_RegularChannelConfig(ADC1, chanel,1, ADC_SampleTime_55Cycles5); // Configure channel
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);// Start the conversion
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);// Wait until conversion completion    
//   return ADC_GetConversionValue(ADC1);// Get the conversion value
}
void check_temp() 
{
int i;
int fa;
int tem;
int cal;


uint32_t adcs1,adcs2;
adcs1=0;adcs2=0;

for(fa=0;fa<100;fa++){
        GetADC1Channel(ADC_Channel_4);
        ADCVal = ADC_GetConversionValue(ADC1);
        adcs1 += ADCVal;

        GetADC1Channel(ADC_Channel_6);
        ADCVal = ADC_GetConversionValue(ADC1);
        adcs2 += ADCVal;
}

adc_p1 = adcs1/100;
adc_p2 = adcs2/100;

    for(i=0;i<161;i++)
        {
           if(adc_p1 <= temp_adcs[i])
               {
                   if((temp_adcs[i] - adc_p1) > (adc_p1 - temp_adcs[i-1]))
                       {
                        temp_p1 = i-49.5;
                       }
                   else temp_p1 = i-50;
                   
                   break;
               }
               
        }
       
    for(i=0;i<161;i++)
        {
			if(adc_p2 <= temp_adcs[i])
			{
				if((temp_adcs[i] - adc_p2) > (adc_p2 - temp_adcs[i-1]))
				{
					temp_p2 = i-49.5;
				}
				else temp_p2 = i-50;
				break;
			}
        }
		/**/
		cal = 1 /2;
		temp_p1 += cal;
		cal = 1 /2;
		temp_p2 += cal;
		
		if((temp_tp1 - temp_p1)>0.5) temp_tp1 -= 0.5;
		else if((temp_tp1 - temp_p1)< -0.5) temp_tp1+= 0.5;
		else temp_tp1 = temp_p1;
		
		if((temp_tp2 - temp_p2)>0.5) temp_tp2-=0.5;
		else if((temp_tp2 - temp_p2)< -0.5) temp_tp2+=0.5;
		else temp_tp2 = temp_p2;

		temp_p1 = temp_tp1;
		temp_p2 = temp_tp2;
		
		tem = temp_p1*10;
		
        if(temp_p1<0)
        {
            tem *=-1;
            sprintf(tempc1,"-%d%d.%d",tem/100,(tem/10)%10,(tem%10));
			if(locks==1) temp_[0]=17;
        }
        else
        {
            sprintf(tempc1," %d%d.%d",tem/100,(tem/10)%10,(tem%10));
			if(locks==1) temp_[0]=19;
        }
    
		if(locks==1) temp_[1]=(tem/10)%10;
		if(locks==1) temp_[2]=tem/100;
    
		tem = temp_p2*10;
        if(temp_p2<0)
        {
            tem*=-1;
            sprintf(tempc2,"-%d%d.%d",tem/100,(tem/10)%10,(tem%10));
			if(locks==2) temp_[0]=17;
        }
        else
        {
            sprintf(tempc2," %d%d.%d",tem/100,(tem/10)%10,(tem%10));
			if(locks==2) temp_[0]=19;
        }
		if(locks==2) temp_[1]=(tem/10)%10;
		if(locks==2) temp_[2]=tem/100;
    
}

void Read_RTC(void) 
{
	DS1307_Read(rtc);
}

void set_temp(int tsi)
{
	if(tsi<0)
	{
		temp_[0]=17;
    tsi = tsi*-1;
	}
	else
	{
		temp_[0]=19;
	}
    
	if(tsi>9)
	{
		temp_[1]=tsi%10;
		temp_[2]=tsi/10;
    }
    else
        {
        temp_[1]=tsi%10;
		temp_[2]=19;
        }
	show_seg1(temp_);
}
void set_led_off(char tsi)
{
	tsi = 1<<tsi;
	led_ = led_ | tsi;
write_to_MCP(0x40, OLAT0, num_digi7[temp_[0]], led_);
}

void set_led_on(char tsi)
{
	tsi = 1<<tsi;
	led_ &= ~tsi;
write_to_MCP(0x40, OLAT0, num_digi7[temp_[0]], led_);
}

void show_seg1(unsigned char *pbuf)
{
	char i;
	write_to_MCP(0x40, OLAT0, (num_digi7[pbuf[0]]), led_);	  //initiallize it so the LSBs outputs are  & MSBs are inputs
	write_to_MCP(0x4e, OLAT0,(num_digi7[pbuf[1]]),(num_digi7[pbuf[2]]));	  //initiallize it so the LSBs outputs are  & MSBs are inputs
	
	for(i=0;i<4;i++)
	{
		if(leds[i]==1)
		{
			set_led_on(i);
			leds[i]=3;
		}
		else if(leds[i]==2)
		{
			set_led_off(i);
			leds[i]=0;
		}
		
	}
}
void show_rtc()
{
	sprintf(uart_buf,"%d-%d-%d %d:%d:%d day %d\r\n",rtc[6],rtc[5],rtc[4],rtc[2],rtc[1],rtc[0],rtc[3]);
	printUART1((unsigned int *)uart_buf);
	printUART2((unsigned int *)uart_buf);

}
void set_date(unsigned char te1,unsigned char te2,unsigned char te3,unsigned char te4,unsigned char te5,unsigned char te6,unsigned char te7)
{
			uint8_t temp;
			
			temp=((te1/10)<<4)|(te1%10);
			DS1307_Write(0x00,temp); printU("s");
			temp=((te2/10)<<4)|(te2%10);
			DS1307_Write(0x01,temp);printU("s");
			temp=((te3/10)<<4)|(te3%10);
			DS1307_Write(0x02,temp);printU("s");
			temp=((te4/10)<<4)|(te4%10);
			DS1307_Write(0x03,temp);printU("s");
			temp=((te5/10)<<4)|(te5%10);
			DS1307_Write(0x04,temp);printU("s");
			temp=((te6/10)<<4)|(te6%10);
			DS1307_Write(0x05,temp);printU("s");
			temp=((te7/10)<<4)|(te7%10);
			DS1307_Write(0x06,temp);printU("s");
			
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void printU(char *chh)
{
	sprintf(uart_buf,"%s\r\n",chh);
	printUART1((unsigned int *)uart_buf);
	printUART2((unsigned int *)uart_buf);
}

void Delay1(uint32_t nTime)
{
  /* Capture the current local time */
ocalTime1 =0;  

  /* wait until the desired delay finish */  
  while(ocalTime1 < nTime)
  {  
  
  }
 
}


void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = ocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > ocalTime)
  {  
  
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  ocalTime += SYSTEMTICK_PERIOD_MS;
  ocalTime1 += SYSTEMTICK_PERIOD_MS;
    octime++;

}
void time_cal(void)
{
int i;
	if(octime>100)
	{
	octime=0;
	for(i=0;i<3;i++)
	{
		if(delay_fin[i]==2)
		{
			if(dels[i]>0)
			{
				dels[i]--;
				if(delm[i] > 0 && dels[i] <= 0)
				{
					delm[i]--;
					dels[i] = 59;
					if(delh[i]> 0 && delm[i] <= 0)
					{
						delh[i]--;
						delm[i] = 59;
					}		
				}
				if(delh[i]<=0&&delm[i]<=0&&dels[i]<=0)
				{
					if(i==1) delay_fin[i] = 0;
					else if(i==2)
					{ 
						delay_fin[i] = 3;
						if(sdoor == 8) sdoor = 9;
					}						
					else
					delay_fin[i] = 1;
					
				}
			
			}
		}			
	}
	}
	
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */

void System_Periodic_Handle(void)
{
  /* Update the LCD display and the LEDs status */
  /* Manage the IP address setting */
  Display_Periodic_Handle(ocalTime);
  
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(ocalTime);

}
/* Check modes and status */
void func_chk(unsigned char mod)
{
	unsigned char mi,i;
	mi=0;
	unsigned char em,ep;
	em = mod*2;
	
	if(start==1) 
	{
		if(delay_fin[0]==1) // main recipe
		{
			delh[0] = ebuff[em];
			delm[0] = ebuff[em+1];
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
			
			delay_fin[0] = 2;
			outpp(ebuff[(modes+20)]);
			modes++;
			if(modes>4) modes=1;
		}
		else
		{
			
			//chk_ru(mod);
		}
		
		if(modes==2 && sdoor != 1)  // recipe 1 check temp
		{
			if(temp_p1 < ebuff[8])
			{
				outpp(ebuff[24]);
			}
			else if(temp_p1 < ebuff[9])
			{
				outpp(ebuff[25]);
			}
		}
		if(modes==3) // defrost check temp
		{
			if(temp_p2 > ebuff[10])
			{
				delay_fin[0]=1;
			}
		}		
			

	
		if(delay_fin[2]==1 || delay_fin[2]==3 ) // Door open delay
		{
			delh[2] = 0;
		
			if(delay_fin[2]==1)	delm[2] = ebuff[11];
			else 
			{
				delm[2] = ebuff[12];
				buf_port = GPIO_ReadInputData(GPIOE);
				out_p  = ebuff[27]&(buf_port>>9);
				outpp(out_p);
				sdoor = 8;
			}			
		
			if(delm[2] == 0 && delh[2]>0)
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
			delay_fin[2]=2;
		//	outpp(eep_b[mod+29]);
		}
	
		if(sdoor == 1)	// door open
		{
			if(delay_fin[2]==0)
			{
				buf_port = GPIO_ReadInputData(GPIOE);
				out_p  = ebuff[26]&(buf_port>>9);
				outpp(out_p);
				locks = 0;
		
				delay_fin[2]=1;
			
			}		
		}
		else if(sdoor == 9)
		{
			locks = 0;
			buf_port = GPIO_ReadInputData(GPIOE);
			out_p  = ebuff[28]&(buf_port>>9);
			outpp(out_p);
		}		
		else if(sdoor == 0)
		{
			if(locks==0)
			{
				outpp(ebuff[(modes+19)]);
				locks = 1;
				delay_fin[2]=0;
				delh[2] = 0;
				delm[2] = 0;
			}	
		}
	}
	else if(start == 3) // stop
	{
		delay_fin[0]=0;
		delay_fin[2]=0;
		start=0;
		modes=0;
		sdoor = 0;
		i2c_rr();
		modes=0;
		/*for(i=0;i<10;i++)
		{
			delh[i] = 0;
			delm[i] = 0;
			dels[i] = 0;
			
		}*/
		outpp(0);
	}
	else if(start == 5)
	{
			modes=0;
			delay_fin[0]=1;
			start =1;
	}
	
		if(delay_fin[1]==1) // delay com
		{
			delh[1] = 0;
			delm[1] = 2;
			if(delm[1] == 0 && delh[1]>0)
			{
				delh[1]--;
				delm[1] = 59;
				dels[1] = 59;
			}
			else
			{
				delm[1]--;
				dels[1] = 59;
			}
			delay_fin[1]=2;
		//	outpp(eep_b[mod+19]);
		}
	
}
void eemode(unsigned char mods)
{
	//I2C_EE_BufferRead(eep_b,mods,40);
}
void outpp(unsigned char val)
{
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9))
	{
		if((val & 0x01)==0x01)
		{
			leds[0] = 1;
		}
		else
		{
			delay_fin[1] = 1;
			leds[0] = 2;
		}
	}
	else
	{
		if((val & 0x01)==0x01)
		{
			if(delay_fin[1] == 2) val &= 0xfe;
			else leds[0] = 1;
		}
		else leds[0] = 2;		
	}
	
	if((val & 0x02)==0x02) leds[1] = 1;
	else leds[1] = 2;

	if((val & 0x04)==0x04) leds[3] = 1;
	else leds[3] = 2;
	
	 out_write(val);

}


void test_i2c()
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
  I2C_EE_BufferWrite(eep_buffer, 0x10,8); 				// Write 8 Byte From Address 0x10...
 */
   I2C_EE_WaitEepromStandbyState();
  // I2C_EE_BufferRead(eep_buffer,0x00,8); 
	for ( i = 0; i < 8; i++ )							// Read 8 Byte to Buffer 
    {
		out_write(eep_buffer[i]); 
		Delay1(250);
		set_temp(eep_buffer[i]);
    }

	/* Read from I2C EEPROM from EEPROM_ReadAddress1 */

//    I2C_EE_BufferRead(eep_buffer,0x10,8); 

	for ( i = 0; i < 8; i++ )							// Read 8 Byte to Buffer 
    {
	  out_write(eep_buffer[i]); 
	  Delay1(250);
    }

}


void TIM3_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    // set_temp();
 // if( td1>=99) td1=-99;

	  if(eemodes==1)
	  {
		//eemode(mod);
		eemodes=0;
	  }
	  else if(eemodes==10)
	  {
		//I2C_EE_BufferWrite(ebuff,0x01,30);
		//I2C_EE_BufferRead(ebuff,0x01,30); 
		//i2c_rr();
		eemodes=0;
	  }
	  else if(eemodes==11)
	  {
		eemodes=0;
	  }
	  else
	  {
		  show_seg1(temp_);
	  }
	//MCP_read(butt,0x4e, 0x01);
	if((butt[0] & 0x20)==0x20) ebuff[39] = 1;
	else ebuff[39] = 0;
 // sprintf(uart_buf,"%d : %d  \r\n",butt[0],butt[1],butt[2]);
 // printUART2((unsigned int *)uart_buf);
    //STM_EVAL_LEDToggle(0);
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
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)) sdoor = 0;
	else sdoor=1;

	capture1 = TIM_GetCapture4(TIM3);
	TIM_SetCompare4(TIM3, capture1 + CCR4_Val);
	//TIM_SetCompare4(TIM3, capture + 500);
  }
}

void MENU_int(void)
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

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
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

void MENU_int1(void)
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

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
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
