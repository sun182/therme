/**
  ******************************************************************************
  * @file    et_stm32f_arm_kit_i2c.h 
  * @author  MCD Application Team
  * @version V3.1.2 
  * @date    09/28/2009
  * @brief   Header for i2c_ee.c module
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
  


/* Define to prevent recursive inclusion ------------------------------------ */

#define __ET_STM32F_ARM_KIT_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Use the defines below the choose the EEPROM type */
#define EE_M24C08  /* Support the device: M24C08. */

/* note: Could support: M24C01, M24C02, M24C04 and M24C16 if the blocks and 
   HW address are correctly defined*/
//#define EE_M24C64_32  /* Support the devices: M24C32 and M24C64 */

#ifdef EE_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Harware connection on the board (on STM3210C-EVAL board E0 = E1 = E2 = 0) */

 #define EEPROM_HW_ADDRESS     0xA0   /* E0 = E1 = E2 = 0 */ 

#elif defined (EE_M24C08)
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 
   EEPROM Addresses defines */
 #define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */ 

#endif /* EE_M24C64_32 */

 #define address_ds1307HW 0xA0   /* E2 = 0 */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_EE_Init(void);
void I2C_EE_ByteWrite(char* pBuffer, uint16_t WriteAddr);
void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_BufferWrite(char* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void I2C_EE_BufferRead(char* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);
void I2C_StandbyState(uint16_t add);
void write_to_MCP(unsigned char address, unsigned char cmd,unsigned char data1, unsigned char data2);
void MCP_read(int* pBuffer,unsigned char addressw, unsigned char cmd);
void DS1307_Write(unsigned char ctl,unsigned char dat);
void DS1307_Read(uint8_t *pBuffer);
//void write_to_MCP(unsigned char address, unsigned char cmd,unsigned char data1, unsigned char data2);
//void MCP_read(uint8_t* pBuffer,unsigned char addressw, unsigned char cmd, unsigned char addressr);


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


