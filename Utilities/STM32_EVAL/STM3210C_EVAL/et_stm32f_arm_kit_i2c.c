/**
  ******************************************************************************
  * @file    et_atm32f_arm_kit_i2c.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides a set of functions needed to manage the
  *          communication between I2C peripheral and I2C M24CXX EEPROM.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "et_stm32f_arm_kit_i2c.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

 #define I2C_FLASH_PAGESIZE    16


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t EEPROM_ADDRESS;
uint16_t TWI_SLA_ds1307;

char uart_buf[12]; 

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configurationi2c(void);
void I2C_Configuration(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the used I/O ports pin
  * @param  None
  * @retval None
  */
void GPIO_Configurationi2c(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  /* Enable the I2C1 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

  /* Configure I2C_EE pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  I2C_EE_SCL | I2C_EE_SDA; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(I2C_EE_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  I2C Configuration
  * @param  None
  * @retval None
  */
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  
  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_EE_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C_EE, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C_EE, &I2C_InitStructure);
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void I2C_EE_Init()
{
  /* I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(I2C_EE_CLK, ENABLE);   
  
  /* GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(I2C_EE_GPIO_CLK, ENABLE);    
  
  /* GPIO configuration */
  GPIO_Configurationi2c();

  /* I2C configuration */
  I2C_Configuration();

    EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
  
  TWI_SLA_ds1307 = address_ds1307HW;
  TWI_SLA_ds1307 = 0xd0;
}

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @retval None
  */
void I2C_EE_ByteWrite(char* pBuffer, uint16_t WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

#ifdef EE_M24C08
  
  /* Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C_EE, WriteAddr);
  
#elif defined(EE_M24C64_32)
  
  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /* EE_M24C08 */  
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C_EE, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
   I2C_EE_WaitEepromStandbyState();
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read 
  *   from the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
void I2C_EE_BufferRead(char* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
	
    /* While the bus is busy */
 while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));
 
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  
  /* Send the EEPROM's internal address to read from: Only one byte address */
  I2C_SendData(I2C_EE, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C_EE, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C_EE, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C_EE);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--; 
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C_EE, ENABLE);
 
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void I2C_EE_BufferWrite(char* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;
	
  Addr = WriteAddr % I2C_FLASH_PAGESIZE;
  count = I2C_FLASH_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
  NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
 
  /* If WriteAddr is I2C_FLASH_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE); 
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_FLASH_PAGESIZE aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Write the data conained in same page */
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();      
        
        /* Write the remaining data in the following page */
        I2C_EE_PageWrite((uint8_t*)(pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));
        I2C_EE_WaitEepromStandbyState();        
      }      
      else      
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();        
      }     
    }
    /* If NumByteToWrite > I2C_FLASH_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_FLASH_PAGESIZE;
      NumOfSingle = NumByteToWrite % I2C_FLASH_PAGESIZE;
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_FLASH_PAGESIZE);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_FLASH_PAGESIZE;
        pBuffer += I2C_FLASH_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be 
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C_EE, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

#ifdef EE_M24C08
  
  /* Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(I2C_EE, WriteAddr);
  
#elif defined(EE_M24C64_32)
  
  /* Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(I2C_EE, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(I2C_EE, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /* EE_M24C08 */  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C_EE, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C_EE, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Read I2C_EE SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(I2C_EE, I2C_Register_SR1);

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2C_EE, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C_EE, I2C_FLAG_AF);
  
  /* STOP condition */    
  I2C_GenerateSTOP(I2C_EE, ENABLE);  
}
void I2C_StandbyState(uint16_t add)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C_EE, ENABLE);

    /* Read I2C_EE SR1 register to clear pending flags */
    SR1_Tmp = I2C_ReadRegister(I2C_EE, I2C_Register_SR1);

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_EE, add, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2C_EE, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C_EE, I2C_FLAG_AF);
  
  /* STOP condition */    
  I2C_GenerateSTOP(I2C_EE, ENABLE);  
}


/************************************************************** DS1307_Write */
void DS1307_Write(unsigned char ctl,unsigned char dat)
{

 /* Send STRAT condition */
  I2C_GenerateSTART(I2C_ds, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C_ds, TWI_SLA_ds1307, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the EEPROM's internal address to write to : only one byte Address */
	I2C_SendData(I2C_ds, ctl);
   while (!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	 I2C_SendData(I2C_ds, dat);
	while (!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	 
      /* STOP condition */    
  I2C_GenerateSTOP(I2C_ds, ENABLE); 
    
   I2C_StandbyState(TWI_SLA_ds1307);
    /* Send STOP condition */

}

unsigned char DS1307_R(unsigned char ctl)
{
	
    unsigned char dat;
	               // Read NACK Received
   // while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));
 
   I2C_GenerateSTART(I2C_ds, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_MODE_SELECT));  
  /* Send EEPROM address for write */
   I2C_Send7bitAddress(I2C_ds, 0xd0, I2C_Direction_Transmitter);
  
   /* Test on EV6 and clear it */
   while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  // /* Send the EEPROM's internal address to write to : only one byte Address */
   I2C_SendData(I2C_ds, ctl);
  while (!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   // while (!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   I2C_GenerateSTART(I2C_ds, ENABLE);
  
  // /* Test on EV5 and clear it */
   while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_MODE_SELECT));
  /* Send address for read */
  I2C_Send7bitAddress(I2C_ds,TWI_SLA_ds1307 , I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_ds, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
   /* While there is data to be read */


    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C_EE, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      dat = I2C_ReceiveData(I2C_EE);

	}
	
        I2C_AcknowledgeConfig(I2C_EE, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C_EE, ENABLE);
  /* Enable Acknowledgement to be ready for another reception */
        I2C_AcknowledgeConfig(I2C_EE, ENABLE);

  return dat;


}

void DS1307_Read(unsigned char *pBuffer)
{
    unsigned char dat;
   /* While there is data to be read */

  for(dat=0;dat<=6;dat++)
  {
    //Delay1(10);
  	
      /* Read a byte from the EEPROM */
      *pBuffer = DS1307_R(dat);
  sprintf(uart_buf,"%u : %u\r\n",*pBuffer,dat);
  printUART2((unsigned int *)uart_buf);
    pBuffer ++;

	}
  
}

void write_to_MCP(unsigned char address, unsigned char cmd,unsigned char data1, unsigned char data2)
{
	I2C_StandbyState(address);

while(I2C_GetFlagStatus(I2C_EE, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C_mcp, ENABLE);
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_MODE_SELECT));  
 	//Transmit the "ADDRESS and WRITE" byte

  I2C_Send7bitAddress(I2C_mcp,address, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C_mcp, cmd);	//Transmit the COMMAND byte
   while (!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	 I2C_SendData(I2C_mcp, data1);	//Transmit first DATA byte
	while (!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	 I2C_SendData(I2C_mcp, data2);	//Transmit second DATA byte
	while (!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_TRANSMITTED));


      /* STOP condition */    
  I2C_GenerateSTOP(I2C_mcp, ENABLE); 
    I2C_StandbyState(address);

/*
   i2c_start(); 		//Generate a START condition
   i2c_write(address);		//Transmit the "ADDRESS and WRITE" byte
   i2c_write(cmd);		//Transmit the COMMAND byte
   i2c_write(data1);		//Transmit first DATA byte
   i2c_write(data2);		//Transmit second DATA byte
   i2c_stop();			//Generate a STOP condition
*/
}
void MCP_read(int* pBuffer,unsigned char addressw, unsigned char cmd)
{
	unsigned char NumByteToRead;
	unsigned char i;
	NumByteToRead = 4;

	while(I2C_GetFlagStatus(I2C_mcp, I2C_FLAG_BUSY));
	/////printU(i++);
	//I2C_GenerateSTART(I2C_mcp, ENABLE);
	///* Test on EV5 and clear it */
	//while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_MODE_SELECT));  
	////printU(i++);
	///* Send address for read */
	//I2C_Send7bitAddress(I2C_mcp,addressw, I2C_Direction_Transmitter);
	///* Test on EV6 and clear it */
	//while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  //
	////printU(i++);
	//I2C_SendData(I2C_mcp, cmd);	//Transmit the COMMAND byte
	//while (!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   ////
	//printU(i++);
	I2C_GenerateSTART(I2C_mcp, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_MODE_SELECT));  
	//printU(i++);
	 I2C_Send7bitAddress(I2C_mcp,addressw, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C_mcp, 0x00);	//Transmit the COMMAND byte 00
   while (!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   
   
   	I2C_GenerateSTART(I2C_mcp, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_MODE_SELECT));  
	
		/* Send address for read */
	I2C_Send7bitAddress(I2C_mcp,addressw +0x01, I2C_Direction_Receiver);
  
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	//printU(i++);



	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			I2C_AcknowledgeConfig(I2C_mcp, DISABLE);
			I2C_GenerateSTOP(I2C_mcp, ENABLE);
		}
		
		if(I2C_CheckEvent(I2C_mcp, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{
		  
		  *pBuffer  = I2C_ReceiveData(I2C_mcp);
		  pBuffer++;
		  NumByteToRead--; 
		}       
	}
	  /* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C_mcp, ENABLE);
    I2C_AcknowledgeConfig(I2C_mcp, ENABLE);
	//printU(i++);
/*
   i2c_start(); 	     // Generate START condition
   i2c_write(AddressW);      // Transmit ADDRESS with WRITE command
   i2c_write(Command);	     // Transmit COMMAND byte
   i2c_repStart();	     // Generate a REPEATED-START condition
   i2c_write(AddressR);      // Transmit ADDRESS with READ command
   MCP_LSB=i2c_read(1);      // Receive first DATA byte  (LSB) and acknowledge
   MCP_MSB=i2c_read(0);      // Receive second DATA byte (MSB) and don't acknowledge
   i2c_stop();		     // Generate a STOP condition
*/
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
