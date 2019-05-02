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

	//===�ṹ�嶨��
	typedef struct _CalcFreq_HandlerType				CalcFreq_HandlerType;

	//===ָ��ṹ�嶨��
	typedef struct _CalcFreq_HandlerType				*pCalcFreq_HandlerType;

	//===����Ƶ�ʵ����ݽṹ��
	struct  _CalcFreq_HandlerType
	{
		VLTUINT8_T  msgStep;			//---��������
		float		msgFreqMHz;			//---MHzƵ��
		VLTUINT32_T msgFreqKHz;			//---KHzƵ��
		void(*msgFuncTask)(void);		//---��������
	};

	//===�ⲿ���ýӿ�
	extern CalcFreq_HandlerType g_CalcFreq;
	extern pCalcFreq_HandlerType pCalcFreq;

	//===��������
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
