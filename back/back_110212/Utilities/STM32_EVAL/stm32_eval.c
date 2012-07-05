/**
  ******************************************************************************
  * @file    stm32_eval.c
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides firmware functions to manage Leds, push-buttons 
  *          and COM ports available on STM32 Evaluation Boards from STMicroelectronics.
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
#include "main.h"
/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup STM32_EVAL 
  * @brief This file provides firmware functions to manage Leds, push-buttons 
  *        and COM ports available on STM32 Evaluation Boards from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32_EVAL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Variables
  * @{
  */ 

GPIO_TypeDef* GPIO_PORT[outn] = {
								 out0_GPIO_PORT, 
                                 out1_GPIO_PORT, 
								 out2_GPIO_PORT,
								 out3_GPIO_PORT, 
                                 out4_GPIO_PORT, 
								 out5_GPIO_PORT,
								 out6_GPIO_PORT,                                  
                                 out7_GPIO_PORT};

const uint16_t GPIO_PIN[outn] = {out0_GPIO_PIN, 
                                 out1_GPIO_PIN, 
								 out2_GPIO_PIN,
								 out3_GPIO_PIN, 
                                 out4_GPIO_PIN, 
								 out5_GPIO_PIN,
								 out6_GPIO_PIN,
                                 out7_GPIO_PIN};

const uint32_t GPIO_CLK[outn] = {out0_GPIO_CLK, 
                                 out1_GPIO_CLK, 
								 out2_GPIO_CLK,
								 out3_GPIO_CLK, 
                                 out4_GPIO_CLK, 
								 out5_GPIO_CLK,
								 out6_GPIO_CLK,
                                 out7_GPIO_CLK};

#ifdef USE_STM3210C_EVAL
 GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_PORT, 
                                       TAMPER_BUTTON_PORT,
                                       KEY_BUTTON_PORT}; 

 const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, 
                                       TAMPER_BUTTON_PIN,
                                       KEY_BUTTON_PIN}; 

 const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_CLK, 
                                       TAMPER_BUTTON_CLK,
                                       KEY_BUTTON_CLK};

 const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
                                             TAMPER_BUTTON_EXTI_LINE, 
                                             KEY_BUTTON_EXTI_LINE};

 const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PORT_SOURCE,
                                               TAMPER_BUTTON_PORT_SOURCE, 
                                               KEY_BUTTON_PORT_SOURCE};
								 
 const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PIN_SOURCE,
                                              TAMPER_BUTTON_PIN_SOURCE, 
                                              KEY_BUTTON_PIN_SOURCE}; 
 const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_IRQn, 
                                        TAMPER_BUTTON_IRQn,
                                        KEY_BUTTON_IRQn};

 USART_TypeDef* COM_USART[COMn] = {EVAL_COM1}; 

 GPIO_TypeDef* COM_PORT[COMn] = {EVAL_COM1_GPIO};

 const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};

 const uint32_t COM_POR_CLK[COMn] = {EVAL_COM1_GPIO_CLK};

 const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TxPin};

 const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RxPin};

#else
 GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_PORT, 
                                       TAMPER_BUTTON_PORT, 
                                       KEY_BUTTON_PORT, RIGHT_BUTTON_PORT,
                                       LEFT_BUTTON_PORT, UP_BUTTON_PORT,
                                       DOWN_BUTTON_PORT, SEL_BUTTON_PORT}; 

 const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, TAMPER_BUTTON_PIN, 
                                       KEY_BUTTON_PIN, RIGHT_BUTTON_PIN,
                                       LEFT_BUTTON_PIN, UP_BUTTON_PIN,
                                       DOWN_BUTTON_PIN, SEL_BUTTON_PIN}; 

 const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_CLK, TAMPER_BUTTON_CLK,
                                       KEY_BUTTON_CLK, RIGHT_BUTTON_CLK,
                                       LEFT_BUTTON_CLK, UP_BUTTON_CLK,
                                       DOWN_BUTTON_CLK, SEL_BUTTON_CLK};

 const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
                                             TAMPER_BUTTON_EXTI_LINE, 
                                             KEY_BUTTON_EXTI_LINE,
                                             RIGHT_BUTTON_EXTI_LINE,
                                             LEFT_BUTTON_EXTI_LINE,
                                             UP_BUTTON_EXTI_LINE,
                                             DOWN_BUTTON_EXTI_LINE,
                                             SEL_BUTTON_EXTI_LINE};

 const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PORT_SOURCE,
                                               TAMPER_BUTTON_PORT_SOURCE, 
                                               KEY_BUTTON_PORT_SOURCE,
                                               RIGHT_BUTTON_PORT_SOURCE,
                                               LEFT_BUTTON_PORT_SOURCE,
                                               UP_BUTTON_PORT_SOURCE,
                                               DOWN_BUTTON_PORT_SOURCE,
                                               SEL_BUTTON_PORT_SOURCE};
								 
 const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_PIN_SOURCE,
                                              TAMPER_BUTTON_PIN_SOURCE, 
                                              KEY_BUTTON_PIN_SOURCE,
                                              RIGHT_BUTTON_PIN_SOURCE,
                                              LEFT_BUTTON_PIN_SOURCE,
                                              UP_BUTTON_PIN_SOURCE,
                                              DOWN_BUTTON_PIN_SOURCE,
                                              SEL_BUTTON_PIN_SOURCE}; 

 const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_IRQn, TAMPER_BUTTON_IRQn, 
                                        KEY_BUTTON_IRQn, RIGHT_BUTTON_IRQn,
                                        LEFT_BUTTON_IRQn, UP_BUTTON_IRQn,
                                        DOWN_BUTTON_IRQn, SEL_BUTTON_IRQn};

 USART_TypeDef* COM_USART[COMn] = {EVAL_COM1, EVAL_COM2}; 

 GPIO_TypeDef* COM_PORT[COMn] = {EVAL_COM1_GPIO, EVAL_COM2_GPIO};

 const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK, EVAL_COM2_CLK};

 const uint32_t COM_POR_CLK[COMn] = {EVAL_COM1_GPIO_CLK, EVAL_COM2_GPIO_CLK};

 const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TxPin, EVAL_COM2_TxPin};

 const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RxPin, EVAL_COM2_RxPin};

#endif 
/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_FunctionPrototypes
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup STM32_EVAL_Private_Functions
  * @{
  */ 

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void out_init(out_TypeDef out)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[out], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[out];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[out], &GPIO_InitStructure);
}




/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void out_on(out_TypeDef out)
{
  GPIO_PORT[out]->BSRR = GPIO_PIN[out]; 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval None
  */
void out_off(out_TypeDef out)
{
  GPIO_PORT[out]->BRR = GPIO_PIN[out];
}

void out_write(uint8_t value)
{
  GPIO_WriteBit(GPIO_PORT[k0], GPIO_PIN[k0], (value & 0x01) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k1], GPIO_PIN[k1], (value & 0x02) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k2], GPIO_PIN[k2], (value & 0x04) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k3], GPIO_PIN[k3], (value & 0x08) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k4], GPIO_PIN[k4], (value & 0x10) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k5], GPIO_PIN[k5], (value & 0x20) ? Bit_SET : Bit_RESET);
  GPIO_WriteBit(GPIO_PORT[k6], GPIO_PIN[k6], (value & 0x40) ? Bit_SET : Bit_RESET);
//  GPIO_WriteBit(GPIO_PORT[out7], GPIO_PIN[out7], (value & 0x80) ? Bit_SET : Bit_RESET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void out_toggle(out_TypeDef out)
{
    GPIO_PORT[out]->ODR ^= GPIO_PIN[out];
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters:   
  *     @arg Button_WAKEUP: Wakeup Push Button
  *     @arg Button_TAMPER: Tamper Push Button  
  *     @arg Button_KEY: Key Push Button 
  *     @arg Button_RIGHT: Joystick Right Push Button 
  *     @arg Button_LEFT: Joystick Left Push Button 
  *     @arg Button_UP: Joystick Up Push Button 
  *     @arg Button_DOWN: Joystick Down Push Button
  *     @arg Button_SEL: Joystick Sel Push Button
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg Mode_GPIO: Button will be used as simple IO 
  *     @arg Mode_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, Button_Mode_TypeDef Button_Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

 //  Enable Button GPIO clock 
  RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == Mode_EXTI)
  {
    // Connect Button EXTI Line to Button GPIO Pin 
    GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);  

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  

      //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
	}
	
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:    
  *     @arg Button_WAKEUP: Wakeup Push Button
  *     @arg Button_TAMPER: Tamper Push Button  
  *     @arg Button_KEY: Key Push Button 
  *     @arg Button_RIGHT: Joystick Right Push Button 
  *     @arg Button_LEFT: Joystick Left Push Button 
  *     @arg Button_UP: Joystick Up Push Button 
  *     @arg Button_DOWN: Joystick Down Push Button
  *     @arg Button_SEL: Joystick Sel Push Button    
  * @retval The Button GPIO pin value.
  */
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  //return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}
uint32_t read_inp(out_TypeDef out)
{
	return	GPIO_ReadInputDataBit(GPIO_PORT[out],GPIO_PIN[out]);
  
}

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(COM_POR_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
#if defined (USE_STM3210E_EVAL)
  if (COM == COM1)
  {
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
  }
  else
  {
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }
#elif defined (USE_STM3210B_EVAL)
  if (COM == COM1)
  {
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
  }
  else
  {
    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }
#elif defined (USE_STM3210C_EVAL)
  if (COM == COM1)
  {
    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }
#endif

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_PORT[COM], &GPIO_InitStructure);
    
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(COM_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

void ET_STM32_COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  if (COM == COM1)
  {
	/* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(EVAL_COM1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* Enable the USART1 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(EVAL_COM1_CLK, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EVAL_COM1_GPIO, &GPIO_InitStructure);
    
    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EVAL_COM1_GPIO, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(EVAL_COM1, USART_InitStruct);
    
    /* Enable USART */
    USART_Cmd(EVAL_COM1, ENABLE);
  }
  else
  {
    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(EVAL_COM2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(EVAL_COM2_CLK, ENABLE);

	/* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM2_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EVAL_COM2_GPIO, &GPIO_InitStructure);
    
    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = EVAL_COM2_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(EVAL_COM2_GPIO, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(EVAL_COM2, USART_InitStruct);
    
    /* Enable USART */
    USART_Cmd(EVAL_COM2, ENABLE);
  }
}


/**
  * @}
  */ 


/**
  * @}
  */ 

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
