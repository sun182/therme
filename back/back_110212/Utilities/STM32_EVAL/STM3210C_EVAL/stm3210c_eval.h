/**
  ******************************************************************************
  * @file    stm3210c_eval.h
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file contains definitions for STM3210C_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
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
#ifndef __STM3210C_EVAL_H
#define __STM3210C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM3210C_EVAL
  * @{
  */ 


/** @defgroup STM3210C_EVAL_Exported_Types
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM3210C_EVAL_Exported_Constants
  * @{
  */ 

/** @addtogroup STM3210C_EVAL_LED
  * @{
  */
#define outn                        8

#define out0_GPIO_PORT              GPIOE
#define out0_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out0_GPIO_PIN               GPIO_Pin_9
  
#define out1_GPIO_PORT              GPIOE
#define out1_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out1_GPIO_PIN               GPIO_Pin_10
  
#define out2_GPIO_PORT              GPIOE
#define out2_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out2_GPIO_PIN               GPIO_Pin_11
  
#define out3_GPIO_PORT              GPIOE
#define out3_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out3_GPIO_PIN               GPIO_Pin_12

#define out4_GPIO_PORT              GPIOE
#define out4_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out4_GPIO_PIN               GPIO_Pin_13
  
#define out5_GPIO_PORT              GPIOE
#define out5_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out5_GPIO_PIN               GPIO_Pin_14
  
#define out6_GPIO_PORT              GPIOE
#define out6_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out6_GPIO_PIN               GPIO_Pin_15
  
#define out7_GPIO_PORT              GPIOB
#define out7_GPIO_CLK               RCC_APB2Periph_GPIOE  
#define out7_GPIO_PIN               GPIO_Pin_5

#define in1_GPIO_PORT              GPIOD
#define in1_GPIO_CLK               RCC_APB2Periph_GPIOD
#define in1_GPIO_PIN               GPIO_Pin_4
/**
  * @}
  */ 
  
/** @addtogroup STM3210C_EVAL_BUTTON
  * @{
  */  
  
#define BUTTONn                     3 /*!< Joystick pins are connected to an IO Expander (accessible through I2C1 interface) */

/**
 * @brief Wakeup push-button = PA0
 */
#define WAKEUP_BUTTON_PORT          GPIOA
#define WAKEUP_BUTTON_CLK           RCC_APB2Periph_GPIOA
#define WAKEUP_BUTTON_PIN           GPIO_Pin_0
#define WAKEUP_BUTTON_EXTI_LINE     EXTI_Line0
#define WAKEUP_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOA
#define WAKEUP_BUTTON_PIN_SOURCE    GPIO_PinSource0
#define WAKEUP_BUTTON_IRQn          EXTI0_IRQn 

/**
 * @brief Tamper push-button = PC13
 */
#define TAMPER_BUTTON_PORT          GPIOC
#define TAMPER_BUTTON_CLK           RCC_APB2Periph_GPIOC
#define TAMPER_BUTTON_PIN           GPIO_Pin_13
#define TAMPER_BUTTON_EXTI_LINE     EXTI_Line13
#define TAMPER_BUTTON_PORT_SOURCE   GPIO_PortSourceGPIOC
#define TAMPER_BUTTON_PIN_SOURCE    GPIO_PinSource13
#define TAMPER_BUTTON_IRQn          EXTI15_10_IRQn 

/**
 * @brief Key push-button = PD15
 */
#define KEY_BUTTON_PORT             GPIOD
#define KEY_BUTTON_CLK              RCC_APB2Periph_GPIOD
#define KEY_BUTTON_PIN              GPIO_Pin_4
#define KEY_BUTTON_EXTI_LINE        EXTI_Line4
#define KEY_BUTTON_PORT_SOURCE      GPIO_PortSourceGPIOD
#define KEY_BUTTON_PIN_SOURCE       GPIO_PinSource4
#define KEY_BUTTON_IRQn             EXTI15_10_IRQn
/**
  * @}
  */ 

/** @addtogroup STM3210C_EVAL_COM
  * @{
  */
  
#define COMn                        2

/**
 * @brief Definition for COM port1, connected to USART2 (USART2 pins remapped on GPIOD)
 */ 

#define EVAL_COM1                   USART1		 				//COM1 = USART1
#define EVAL_COM1_GPIO              GPIOB						//USART1 Port = PB
#define EVAL_COM1_CLK               RCC_APB2Periph_USART1		//Enable USART1 Clock
#define EVAL_COM1_GPIO_CLK          RCC_APB2Periph_GPIOB		//Enable PB Clock
#define EVAL_COM1_RxPin             GPIO_Pin_7					//RX1=PB7
#define EVAL_COM1_TxPin             GPIO_Pin_6					//TX1=PB6

//     UART2(Remap) = PD6:RX2
//                  = PD5:TX2
#define EVAL_COM2                   USART2						//COM2 = USART2
#define EVAL_COM2_GPIO              GPIOD                       //USART2 Port = PD 
#define EVAL_COM2_CLK               RCC_APB1Periph_USART2		//Enable UART2 Clock
#define EVAL_COM2_GPIO_CLK          RCC_APB2Periph_GPIOD		//Enable PD Clock
#define EVAL_COM2_RxPin             GPIO_Pin_6					//RX2=PD6
#define EVAL_COM2_TxPin             GPIO_Pin_5					//TX2=PD5


// ET-STM32F ARM KIT(STM32F107VCT6) Hardware Kit 
// 4). I2C1(Remap)  = PB8:SCL1
//                  = PB9:SDA1
#define I2C_EE             			I2C1
#define I2C_EE_CLK         			RCC_APB1Periph_I2C1
#define I2C_EE_GPIO        			GPIOB
#define I2C_EE_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define I2C_EE_SCL         			GPIO_Pin_8
#define I2C_EE_SDA         			GPIO_Pin_9
#define I2C_EE_Speed				50000
#define I2C_EE_SLAVE_ADDRESS7     	0xA0

#define I2C_ds             			I2C1
#define I2C_ds_CLK         			RCC_APB1Periph_I2C1
#define I2C_ds_GPIO        			GPIOB
#define I2C_ds_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define I2C_ds_SCL         			GPIO_Pin_8
#define I2C_ds_SDA         			GPIO_Pin_9
#define I2C_ds_Speed              	10000
#define I2C_ds_SLAVE_ADDRESS7     	0xD0

#define I2C_mcp             			I2C1
#define I2C_mcp_CLK         			RCC_APB1Periph_I2C1
#define I2C_mcp_GPIO        			GPIOB
#define I2C_mcp_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define I2C_mcp_SCL         			GPIO_Pin_8
#define I2C_mcp_SDA         			GPIO_Pin_9
#define I2C_mcp_Speed              	10000
#define I2C_mcp_SLAVE_ADDRESS7     	0xD0

#define ET_STM32F_ARM_KIT_I2C_H

/*
// ET-STM32F ARM KIT(STM32F107VCT6) Hardware Kit 
// 4). I2C1(Remap)  = PB8:SCL1
//                  = PB9:SDA1
#define I2C_MC             			I2C1
#define I2C_MC_CLK         			RCC_APB1Periph_I2C1
#define I2C_MC_GPIO        			GPIOB
#define I2C_MC_GPIO_CLK    			RCC_APB2Periph_GPIOB
#define I2C_MC_SCL         			GPIO_Pin_8
#define I2C_MC_SDA         			GPIO_Pin_9
#define I2C_MC_SpMCd              	400000
#define I2C_MC_SLAVE_ADDRESS7     	0x40
*/

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/** @defgroup STM3210C_EVAL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM3210C_EVAL_Exported_Functions
  * @{
  */ 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM3210C_EVAL_H */
/**
  * @}
  */ 


/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
