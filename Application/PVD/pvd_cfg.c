#include "pvd_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PVD_Init(UINT32_T pwrLevel)
{
	//---PWR时钟
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	//---可编程电压检测中断等级
	NVIC_SetPriority(PVD_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));

	//---配置掉电检测等级
	LL_PWR_SetPVDLevel(pwrLevel);

	//--- PVD连接到中断线16上
	//--- Rising---表示电压从高下降到低于设定阀值时产生中断
	//--- Falling---表示电压从低上升到高于设定阀值时产生中断
	//--- Rising_Falling---表示电压上升或下降越过设定阀值时都产生中断

	LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_16;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);

	//---使能掉电检测
	LL_PWR_EnablePVD();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PVD_DeInit(void)
{
	LL_PWR_DeInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PVD_IRQTask(void)
{
}