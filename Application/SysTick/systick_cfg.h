#ifndef SYSTICK_CFG_H_
#define SYSTICK_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===定义结构体
	typedef struct _SysTick_HandlerType			SysTick_HandlerType;

	//===定义指针结构体
	typedef struct _SysTick_HandlerType			*pSysTick_HandlerType;

	//===结构体定义
	struct _SysTick_HandlerType
	{
		VLTUINT32_T				msgIncTick;																				//---递加计数器
		VLTUINT32_T				msgDecTick;																				//---递减计数器
		VLTUINT32_T				msgIncTickOVF;																			//---递加计数器溢出
		void(*msgHalIncTick)(void);																					//---Hal库用到的计数器
		void(*msgFuncTick)(void);																						//---任务函数
	};

	//===外部调用接口
	extern SysTick_HandlerType		g_SysTick;
	extern pSysTick_HandlerType		pSysTick;

	//===函数定义
	UINT8_T  SysTick_Init(void);
	UINT8_T  SysTick_DeInit(void);
	UINT8_T  SysTick_FuncTick(void(*pFuncTick)(void));
	UINT8_T  SysTick_IRQDecTick(UINT32_T waitDecTick);
	UINT8_T  SysTick_DecTick(UINT32_T waitDecTick);
	UINT32_T SysTick_GetTick(void);
	UINT8_T  SysTick_IRQTick(void);
	UINT8_T  SysTick_WaitTick(UINT32_T usTime);
	UINT8_T  SysTick_WaitusTick(UINT32_T usTime);
	UINT8_T  SysTick_WaitSysTick(UINT32_T sysTickTime);
	UINT8_T  SysTick_1msIncTick(void);
	UINT32_T SysTick_GetTickOVF(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* SYSTICK_CFG_H_ */