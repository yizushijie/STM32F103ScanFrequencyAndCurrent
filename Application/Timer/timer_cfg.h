#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "systick_task.h"

	//===结构体定义
	typedef struct _CalcFreq_HandlerType				CalcFreq_HandlerType;

	//===指针结构体定义
	typedef struct _CalcFreq_HandlerType				*pCalcFreq_HandlerType;

	//===计算频率的数据结构体
	struct  _CalcFreq_HandlerType
	{
		VLTUINT8_T  msgStep;			//---操作步序
		float		msgFreqMHz;			//---MHz频率
		VLTUINT32_T msgFreqKHz;			//---KHz频率
		void(*msgFuncTask)(void);		//---操作任务
	};

	//===外部调用接口
	extern CalcFreq_HandlerType g_CalcFreq;
	extern pCalcFreq_HandlerType pCalcFreq;

	//===函数定义
	void Timer_CalcFreq_Task(void);
	UINT32_T Timer_GetFreqKHz(void);
	float Timer_GetFreqMHz(void);
	void Timer_Init(void);
	void Timer_Clock(TIM_TypeDef *TIMx, UINT8_T isEnable);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*TIMER_CFG_H_ */
