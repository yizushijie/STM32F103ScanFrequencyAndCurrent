#include "css_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void CSS_Init(void)
{
	if (LL_RCC_HSE_IsReady() == 1)
	{
		//---使能HSE时钟安全系统
		LL_RCC_HSE_EnableCSS();
	}
	/* NonMaskableInt_IRQn interrupt configuration */
	NVIC_SetPriority(NonMaskableInt_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* RCC_IRQn interrupt configuration */
	NVIC_SetPriority(RCC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

	//---使能中断
	NVIC_EnableIRQ(NonMaskableInt_IRQn);

	//---使能中断
	NVIC_EnableIRQ(RCC_IRQn);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void CSS_DeInit(void)
{
	//---不使能中断
	NVIC_DisableIRQ(NonMaskableInt_IRQn);

	//---不使能中断
	NVIC_DisableIRQ(RCC_IRQn);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：当外部晶振被短路不能起振后进入NMI中断函数
//////////////////////////////////////////////////////////////////////////////
void CSS_NMI_IRQ(void)
{
	if (LL_RCC_IsActiveFlag_HSECSS() != 0)
	{
		//---使能外部晶振
		LL_RCC_HSE_Enable();

		//---使能HSE就绪中断
		LL_RCC_EnableIT_HSERDY();

		//---清除时钟安全系统的中断挂起位
		LL_RCC_ClearFlag_HSECSS();
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：当HSE恢复正常时进入该函数
//////////////////////////////////////////////////////////////////////////////
void CSS_RCC_IRQ(void)
{
	if (LL_RCC_IsActiveFlag_HSERDY() != 0)
	{
		//---关闭HSE就绪中断
		LL_RCC_DisableIT_HSERDY();

		//---清除HSE就绪中断标志位
		LL_RCC_ClearFlag_HSERDY();

		//---系统软复位
		SOFT_RESET();
	}
}