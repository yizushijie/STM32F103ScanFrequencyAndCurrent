/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  /* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：外部晶振被短路不能起振后进入NMI中断函数
//////////////////////////////////////////////////////////////////////////////
void NMI_Handler(void)
{
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */
	CSSTask_NMI_IRQ();
	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */

	/* USER CODE END NonMaskableInt_IRQn 1 */
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： RCC中断处理函数
//////////////////////////////////////////////////////////////////////////////
void RCC_IRQHandler(void)
{
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */
	CSSTask_RCC_IRQ();
	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */

	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
	/* USER CODE BEGIN HardFault_IRQn 0 */
	HardFault_ErrIRQ();
	/* USER CODE END HardFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
	/* USER CODE BEGIN HardFault_IRQn 1 */

	/* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */

	/* USER CODE END MemoryManagement_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
		/* USER CODE END W1_MemoryManagement_IRQn 0 */
	}
	/* USER CODE BEGIN MemoryManagement_IRQn 1 */

	/* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
	/* USER CODE BEGIN BusFault_IRQn 0 */

	/* USER CODE END BusFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_BusFault_IRQn 0 */
		/* USER CODE END W1_BusFault_IRQn 0 */
	}
	/* USER CODE BEGIN BusFault_IRQn 1 */

	/* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
	/* USER CODE BEGIN UsageFault_IRQn 0 */

	/* USER CODE END UsageFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
		/* USER CODE END W1_UsageFault_IRQn 0 */
	}
	/* USER CODE BEGIN UsageFault_IRQn 1 */

	/* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
	/* USER CODE BEGIN SVCall_IRQn 0 */

	/* USER CODE END SVCall_IRQn 0 */
	/* USER CODE BEGIN SVCall_IRQn 1 */

	/* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
	/* USER CODE BEGIN PendSV_IRQn 0 */

	/* USER CODE END PendSV_IRQn 0 */
	/* USER CODE BEGIN PendSV_IRQn 1 */

	/* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */
	SysTickTask_IRQTick();
	/* USER CODE END SysTick_IRQn 0 */

	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：WWDG 中断复服务程序，如果发生了此中断，表示程序已经出现了故障，
////////////// 这是一个死前中断。在此中断服务程序中应该干最重要的事，
////////////// 比如保存重要的数据等，这个时间具体有多长，要
////////////// 由WDGTB 的值决
//////////////////////////////////////////////////////////////////////////////
void WWDG_IRQHandler()
{
	////---刷新递减计数器的值，当禁掉此句后,窗口看门狗将产生复位
	//LL_WWDG_SetCounter(WWDG, 0x7F);
	//---清除提前唤醒中断标志
	LL_WWDG_ClearFlag_EWKUP(WWDG);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 串口1的中断处理函数
//////////////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void)
{
	//---数据接收中断处理函数---接收寄存器不为空
	if (LL_USART_IsActiveFlag_RXNE(pUSART1->msgUSART) && LL_USART_IsEnabledIT_RXNE(pUSART1->msgUSART))
	{
		USARTTask_ITRead_Task(pUSART1, LL_USART_ReceiveData8(USART1));

		//---清楚中断标志位
		LL_USART_ClearFlag_RXNE(USART1);
	}

	//---数据发送寄存器空发送中断处理函数
	if (LL_USART_IsActiveFlag_TXE(pUSART1->msgUSART) && LL_USART_IsEnabledIT_TXE(pUSART1->msgUSART))
	{
		USARTTask_ITWrite_TXETask(pUSART1);
	}

	//---数据发送完成中断处理函数
	if (LL_USART_IsActiveFlag_TC(pUSART1->msgUSART) && LL_USART_IsEnabledIT_TC(pUSART1->msgUSART))
	{
		USARTTask_ITWrite_TCTask(pUSART1);

		//---清楚中断标志位
		LL_USART_ClearFlag_TC(USART1);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void DMA1_Channel1_IRQHandler(void)
{
	if (LL_DMA_IsActiveFlag_TC1(DMA1) && (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_1)))
	{
		ADCTask_ADCTask_STOP(ADC1);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void DMA2_Channel4_5_IRQHandler(void)
{
	if (LL_DMA_IsActiveFlag_TC5(DMA2) && (LL_DMA_IsEnabledIT_TC(DMA2, LL_DMA_CHANNEL_5)))
	{
		ADCTask_ADCTask_STOP(ADC3);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：定时器2的中断
//////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
	DecodeTask_IRQTask();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：可编程掉电检测
//////////////////////////////////////////////////////////////////////////////
void PVD_IRQHandler(void)
{
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_16);
	if (LL_PWR_IsActiveFlag_PVDO())
	{
		//---掉电检测处理函数
		PVDTask_IRQTask();
	}
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/