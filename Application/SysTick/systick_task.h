#ifndef SYSTICK_TASK_H_
#define SYSTICK_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
//---头文件定义
#include "systick_lib.h"

//---函数定义
	UINT8_T	 SysTickTask_Init(void);
	UINT8_T	 SysTickTask_DeInit(void);
	UINT8_T	 SysTickTask_FuncTick(void(*pFuncTick)(void));
	UINT8_T  SysTickTask_IRQDecTick(UINT32_T waitDecTick);
	UINT8_T  SysTickTask_DecTick(UINT32_T waitDecTick);
	UINT32_T SysTickTask_GetTick(void);
	UINT8_T	 SysTickTask_IRQTick(void);
	UINT8_T	 SysTickTask_WaitTick(UINT32_T usTime);
	UINT8_T  SysTickTask_WaitusTick(UINT32_T usTime);
	UINT8_T  SysTickTask_1msIncTick(void);
	UINT32_T SysTickTask_GetTickOVF(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*SYSTICK_TASK_H_ */
