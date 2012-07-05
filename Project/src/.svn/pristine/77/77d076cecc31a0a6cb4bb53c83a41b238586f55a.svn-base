/**
  ******************************************************************************
  * @file    stm32f107.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   STM32F107 hardware configuration
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
#include "stm32_eth.h"
#include "stm32f107.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DP83848_PHY        /* Ethernet pins mapped on STM3210C-EVAL Board */
#define PHY_ADDRESS   0x01 /* Relative to STM3210C-EVAL Board */

//ET-STM32F ARM KIT Board Used Ethernet Interface = RMII Only
#define RMII_MODE    

/*--------------- LCD Messages ---------------*/
#define MESSAGE1   "     STM32F107      "
#define MESSAGE2   " Connectivity Line  "
#define MESSAGE3   "   * LwIP demos *   "
#define MESSAGE4   "                    "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void ADC_Configuration(void);
void Ethernet_Configuration(void);
void ET_STM32_RCC_Configuration(void);

/**
  * @brief  Setup STM32 system (clocks, Ethernet, GPIO, NVIC) and STM3210C-EVAL resources.
  * @param  None
  * @retval None
  */
void System_Setup(void)
{
 SystemInit();

  RCC_ClocksTypeDef RCC_Clocks;

  /* Setup STM32 clock, PLL and Flash configuration) */
  SystemInit();
    ET_STM32_RCC_Configuration();

  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


  /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

  /* Enable GPIOs and ADC1 clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO |
						 RCC_APB2Periph_ADC1, ENABLE);


  /* TIM2 clock enable */
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  //TIM_Configuration();
  /* NVIC configuration */
  NVIC_Configuration();  

  /* ADC configuration */
  ADC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Initialize the STM3210C-EVAL's LCD */
  //STM3210C_LCD_Init();

  /* Initialize STM3210C-EVAL's ks */
  
  out_init(k0);
  out_init(k1);
  out_init(k2);
  out_init(k3);
  out_init(k4);
  out_init(k5);
  out_init(k6);
  out_init(d7);

  /* Turn on leds available on STM3210X-EVAL */
  out_off(k0);
  out_off(k1);
  out_off(k2);
  out_off(k3);
  out_off(k4);
  out_off(k5);
  out_off(k6);
  out_off(d7);

  /* Clear the LCD */
  //LCD_Clear(Blue);

  /* Set the LCD Back Color */
  //LCD_SetBackColor(Blue);

  /* Set the LCD Text Color */
  //LCD_SetTextColor(White);

  /* Display message on the LCD*/
  //LCD_DisplayStringLine(Line0, MESSAGE1);
  //LCD_DisplayStringLine(Line1, MESSAGE2);
  //LCD_DisplayStringLine(Line2, MESSAGE3);
  //LCD_DisplayStringLine(Line3, MESSAGE4);

  /* Configure the Ethernet peripheral */
  Ethernet_Configuration();

  /* SystTick configuration: an interrupt every 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);

  /* Update the SysTick IRQ priority should be higher than the Ethernet IRQ */
  /* The Localtime should be updated during the Ethernet packets processing */
  NVIC_SetPriority (SysTick_IRQn, 1);  
  
  /* Configure the Key button */ 
  //STM_EVAL_PBInit(Button_KEY, Mode_GPIO);
 STM_EVAL_PBInit(Button_KEY, Mode_GPIO);
}

/**
  * @brief  Configures the clock
  * @param  None
  * @retval None
  */
void ET_STM32_RCC_Configuration(void)
{
  RCC_ClocksTypeDef RCC_ClockFreq;
  ErrorStatus HSEStartUpStatus;

   /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /***************************************************/
    /* HSE=25MHz, HCLK=72MHz, PCLK2=72MHz, PCLK1=36MHz */
    /***************************************************/

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /*  ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* Configure PLLs */
    /* PPL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PPL1 configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }

  //Initial System Clock Complete Start Initial Peripheral Clock
  RCC_GetClocksFreq(&RCC_ClockFreq);

  //Start of Initial USB Clock input (48MHz)
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
  //End of Initial USB Clock input (48MHz)

  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	


  /* Enable ETHERNET clock  */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC    | 
                        RCC_AHBPeriph_ETH_MAC_Tx |
                        RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);

  /* Enable GPIOs clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |	
                         RCC_APB2Periph_GPIOB | 
						 RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | 
						 RCC_APB2Periph_GPIOE | 
						 RCC_APB2Periph_AFIO, ENABLE);

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}


/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
void Ethernet_Configuration(void)
{
  ETH_InitTypeDef ETH_InitStructure;

  //Start of Config RMII Mode
  GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

  /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
  RCC_PLL3Config(RCC_PLL3Mul_10);
  /* Enable PLL3 */
  RCC_PLL3Cmd(ENABLE);
  /* Wait till PLL3 is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
  {}

  /* Get PLL3 clock on PA8 pin (MCO) */
  RCC_MCOConfig(RCC_MCO_PLL3CLK);
  //End of Config RMII Mode

  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();

  /* Wait for software reset */
  while (ETH_GetSoftwareResetStatus() == SET);

  /* ETHERNET Configuration ------------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/  
  
  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     
 
  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;                                                          
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;                                                                 
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  /* Configure Ethernet */
  ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

  /* Enable the Ethernet Rx Interrupt */
  ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


						 
  // Start Initial GPIO For Interface DP83848V
  // Initial ET-STM32F ARM KIT Ethernet Interface = RMII Mode
  // PA1  <- ETH_RMII_REF_CLK(Default)
  // PA2  -> ETH_RMII_MDIO(Default)
  // PA8  -> MCO(Default)
  // PB11 -> ETH_RMII_TXEN(Default)
  // PB12 -> ETH_RMII_TXD0(Default)
  // PB13 -> ETH_RMII_TXD1(Default)
  // PC1  -> ETH_RMII_MDC(Default)
  // PD8  <- ETH_RMII_RXDV(Remap)
  // PD9  <- ETH_RMII_RXD0(Remap)
  // PD10 <- ETH_RMII_RXD1(Remap)

  // Configure PA2 as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//PA2 = ETH_RMII_MDIO
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure PC1 as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PC1 = ETH_RMII_MDC
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Configure PB11, PB12 and PB13 as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 |			//PB11 = ETH_RMII_TXEN 
                                GPIO_Pin_12 | 		  	//PB12 = ETH_RMII_TXD0
								GPIO_Pin_13;			//PB13 = ETH_RMII_TXD1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Configure PA1 as input 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//PA1 = ETH_RMII_REF_CLK
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // ETHERNET pins remapp in ET-STM32F ARM KIT board: RX_DV and RxD[1:0] 
  // PD8=CRS_DV(RMII Remap)
  // PD9=RXD0(RMII Remap)
  // PD10=RXD1(RMII Remap)
  GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

  // Configure PD8, PD9, PD10 as input 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | 			//PD8  = ETH_RMII_RX_DV
  							    GPIO_Pin_9 | 			//PD9  = ETH_RMII_RXD0
								GPIO_Pin_10;			//PD10 = ETH_RMII_RXD1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  // Configure MCO (PA8) as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 			//PA8 = MCO
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  

  /* ADC Channel14 config ----------------------------------------------------*/
  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
void ADC_Configuration(void)
{

   GPIO_InitTypeDef GPIO_InitStructure;
   ADC_InitTypeDef   ADC_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   ADC_DeInit(ADC1);

  // ADC Structure Initialization
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Cmd(ADC1, ENABLE); // Enable the ADC
  ADC_ResetCalibration(ADC1);// ADC calibration ,Enable ADC1 reset calibaration register
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);// Check the end of ADC1 reset calibration register
  ADC_StartCalibration(ADC1);// Start ADC1 calibaration
  while(ADC_GetCalibrationStatus(ADC1) == SET);// Check the end of ADC1 calibration
  
}



/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  
  /* Enable the Ethernet global Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);   


  /* Enable the RTC Interrupt */
/*
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
*/
  /* Enable the TIM2 gloabal Interrupt */

  
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
