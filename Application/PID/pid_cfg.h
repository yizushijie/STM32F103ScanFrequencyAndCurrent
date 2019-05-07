#ifndef	PID_CFG_H_
#define	PID_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===定义结构体
	typedef struct _PID_HandlerType			PID_HandlerType;

	//===定义指针结构体
	typedef struct _PID_HandlerType			*pPID_HandlerType;

	//===定义结构体
	struct _PID_HandlerType
	{
		float kP;			//---比例系数
		float kI;			//---积分系数
		float kD;			//---微分系数
		float uMax;			//---最大值
		float uMin;			//---最小值
		float err;			//---定义偏差
		float err1;			//---第一次偏差
		float err2;			//---第二次偏差
		float errMax;		//---偏差最大值
		float errMin;		//---偏差的最小值
		float errDelta;		//---偏差增量系数
		float iDelta;		//---积分值
		float setVal;		//---设定值
		float actVal;		//---实际值
		float calcVal;		//---计算值
		float calcDelta;	//---计算增量，偏差值
	};

	//===定义的任务函数
	#define	PID_TASK_ONE			pPID0
	#define	PID_TASK_TWO			0
	#define	PID_TASK_THREE			0
	
	//===外部调用接口
	extern PID_HandlerType			g_PID0;
	extern pPID_HandlerType			pPID0;

	//===函数定义
	UINT8_T  PID_Init(PID_HandlerType *PIDx, PID_HandlerType pidx);
	float  PID_PosCalc(PID_HandlerType *PIDx, float setVal);
	float  PID_IncCalc(PID_HandlerType *PIDx, float setVal);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*PID_CFG_H_ */
