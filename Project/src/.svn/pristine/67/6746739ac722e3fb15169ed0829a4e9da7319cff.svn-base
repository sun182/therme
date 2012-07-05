/**
  ******************************************************************************
  * @file    stm32f10x_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "stm32_eth.h"
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


extern __IO uint32_t TimeDisplay;

__IO uint32_t TimeDisplay = 0;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

ErrorStatus HSEStartUpStatus;

uint16_t capture = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;

/* Private function prototypes -----------------------------------------------*/
extern void LwIP_Pkt_Handle(void);
extern void tcp_led_control(out_TypeDef out);


/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Update the LocalTime by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
  Time_Update();

}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
/*
void TIM2_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

   
   STM_EVAL_LEDToggle(0);
    //STM_EVAL_LEDToggle(0);
    capture = TIM_GetCapture1(TIM2);
    TIM_SetCompare1(TIM2, capture + CCR1_Val);
  //TIM_SetCompare1(TIM2, capture + 6000);
  }
  else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

    
    STM_EVAL_LEDToggle(1);
  //STM_EVAL_LEDToggle(1);  
  capture = TIM_GetCapture2(TIM2);
    TIM_SetCompare2(TIM2, capture + CCR2_Val);
  //TIM_SetCompare2(TIM2, capture + 3000);
  }
  else if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);

   
   STM_EVAL_LEDToggle(2);
  capture = TIM_GetCapture3(TIM2);
    TIM_SetCompare3(TIM2, capture + CCR3_Val);
  //TIM_SetCompare3(TIM2, capture + 1000);
  }
  else
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);

   
    STM_EVAL_LEDToggle(3);
  capture = TIM_GetCapture4(TIM2);
    TIM_SetCompare4(TIM2, capture + CCR4_Val);
    //TIM_SetCompare4(TIM2, capture + 500);
  }
}
*/

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
 {

  // if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  // {
    // /* Clear the RTC Second interrupt */
    // RTC_ClearITPendingBit(RTC_IT_SEC);

  
   

    // /* Enable time update */
    // TimeDisplay = 1;

    // /* Wait until last write operation on RTC registers has finished */
    // RTC_WaitForLastTask();
    // /* Reset RTC Counter when Time is 23:59:59 */
    // if (RTC_GetCounter() == 0x00015180)
    // {
      // RTC_SetCounter(0x0);
      // /* Wait until last write operation on RTC registers has finished */
      // RTC_WaitForLastTask();
    // }
  // }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles ETH interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  /* Handles all the received frames */
  while(ETH_GetRxPktSize() != 0) 
  {		
    LwIP_Pkt_Handle();
  }

  /* Clear the Eth DMA Rx IT pending bits */
  ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
  ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}


/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line14) != RESET)
  {   

   EXTI_ClearITPendingBit(EXTI_Line14);
  }   
 
}

//void EXTI15_10_IRQHandler(void)
//{
//
//
  //if(EXTI_GetITStatus(EXTI_LINE_IOE_ITLINE) != RESET)
  //{   
     //static TS_STATE* TS_State;
    //
     ///* Check if the interrupt source is the Touch Screen */
     //if (IOE_GetGITStatus(IOE_1_ADDR, IOE_TS_IT) & IOE_TS_IT)
     //{
       ///* Update the structure with the current position */
       //TS_State = IOE_TS_GetState();  
      //
       //if ((TS_State->TouchDetected) && (TS_State->Y < 220) && (TS_State->Y > 180))
       //{
         //if ((TS_State->X > 10) && (TS_State->X < 70))
         //{         
		   //STM_EVAL_LEDOn(LED3);
		  //
		   ///* Send out a TCP packet to toggle the server's led4 */
		   //tcp_led_control(LED3);
         //}
         //else if ((TS_State->X > 90) && (TS_State->X < 150))
         //{
           //STM_EVAL_LEDOn(LED2);
		  //
		   ///* Send out a TCP packet to toggle the server's led3 */
		   //tcp_led_control(LED2);
         //}
         //else if ((TS_State->X > 170) && (TS_State->X < 230))
         //{
         //
           //STM_EVAL_LEDOn(LED1);
		  //
		   ///* Send out a TCP packet to toggle the server's led2 */
		   //tcp_led_control(LED1);
         //}     
         //else if ((TS_State->X > 250) && (TS_State->X < 310))
         //{
           //STM_EVAL_LEDOn(LED0);
		  //
		   ///* Send out a TCP packet to toggle the server's led1 */
		   //tcp_led_control(LED0);
         //}
       //}
       //else
       //{
         //STM_EVAL_LEDOff(LED0);
         //STM_EVAL_LEDOff(LED1);
         //STM_EVAL_LEDOff(LED2);
         //STM_EVAL_LEDOff(LED3);
       //}    
      //
       ///* Clear the interrupt pending bits */    
       //IOE_ClearGITPending(IOE_1_ADDR, IOE_TS_IT);      
     //}
     //else
     //{
       //IOE_ClearGITPending(IOE_1_ADDR, ALL_IT);
       //IOE_ClearGITPending(IOE_2_ADDR, ALL_IT);
     //}
    //
   //EXTI_ClearITPendingBit(EXTI_LINE_IOE_ITLINE);
  //}  
//}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
