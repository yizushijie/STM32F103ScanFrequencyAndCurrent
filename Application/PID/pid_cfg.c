#include "pid_cfg.h"

//===定义全局变量
PID_HandlerType			g_PID0 = { 0 };
pPID_HandlerType		pPID0 = &g_PID0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 第一个PID变量的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PID_Task0_Init(PID_HandlerType *PIDx, PID_HandlerType pidx)
{
	g_PID0 = pidx;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 第二个PID变量的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PID_Task1_Init(PID_HandlerType *PIDx, PID_HandlerType pidx)
{
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 第三个PID变量的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void PID_Task2_Init(PID_HandlerType *PIDx, PID_HandlerType pidx)
{
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： PID初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  PID_Init(PID_HandlerType *PIDx, PID_HandlerType pidx)
{
	//---指定设备的初始化
	if ((PIDx != NULL) && (PIDx == PID_TASK_ONE))
	{
		PID_Task0_Init(PIDx, pidx);
	}
	else if ((PIDx != NULL) && (PIDx == PID_TASK_TWO))
	{
		PID_Task1_Init(PIDx, pidx);
	}
	else if ((PIDx != NULL) && (PIDx == PID_TASK_THREE))
	{
		PID_Task2_Init(PIDx, pidx);
	}
	else
	{
		return ERROR_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： PID位置式计算
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
float  PID_PosCalc(PID_HandlerType *PIDx, float setVal)
{
	float index = 0;

	//---设定值
	PIDx->setVal = setVal;

	//---计算当前偏差
	PIDx->err = PIDx->setVal - PIDx->actVal;

	//---饱和积分
	if (PIDx->actVal > PIDx->uMax)
	{
		//===积分分离过程
		//---当前偏差的绝对值
		if (ABS(PIDx->err) > PIDx->errMax)
		{
			//---积分分离
			index = 0;
		}
		else if (ABS(PIDx->err) < PIDx->errMin)
		{
			index = 1;
			if (PIDx->err < 0)
			{
				//---积分增量系数
				PIDx->iDelta += PIDx->err;
			}
		}
		else
		{
			index = (PIDx->errMax - ABS(PIDx->err)) / (PIDx->errDelta);
			if (PIDx->err < 0)
			{
				//---积分增量系数
				PIDx->iDelta += PIDx->err;
			}
		}
	}
	else if (PIDx->actVal < PIDx->uMin)
	{
		//===积分分离过程
		//---当前偏差的绝对值
		if (ABS(PIDx->err) > PIDx->errMax)
		{
			//---积分分离
			index = 0;
		}
		else if (ABS(PIDx->err) < PIDx->errMin)
		{
			index = 1;
			if (PIDx->err > 0)
			{
				//---积分增量系数
				PIDx->iDelta += PIDx->err;
			}
		}
		else
		{
			index = (PIDx->errMax - ABS(PIDx->err)) / (PIDx->errDelta);
			if (PIDx->err > 0)
			{
				//---积分增量系数
				PIDx->iDelta += PIDx->err;
			}
		}
	}
	else
	{
		//---当前偏差的绝对值
		if (ABS(PIDx->err) > PIDx->errMax)
		{
			//---积分分离
			index = 0;
		}
		else if (ABS(PIDx->err) < PIDx->errMin)
		{
			index = 1;

			//---积分增量系数
			PIDx->iDelta += PIDx->err;
		}
		else
		{
			index = (PIDx->errMax - ABS(PIDx->err)) / (PIDx->errDelta);

			//---积分增量系数
			PIDx->iDelta += PIDx->err;
		}
	}

	//---计算值
	PIDx->calcVal = (PIDx->kP)*(PIDx->err) + index * (PIDx->kI)*(PIDx->iDelta) + (PIDx->kD)*(PIDx->err - PIDx->err1);

	//---保留当前积分
	PIDx->err1 = PIDx->err;

	//---当前实际值
	PIDx->actVal = PIDx->calcVal*PIDx->calcDelta;
	return  PIDx->actVal;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： PID增量式计算
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
float PID_IncCalc(PID_HandlerType *PIDx, float setVal)
{
	float ep, ei, ed;

	//---设定值
	PIDx->setVal = setVal;

	//---计算当前偏差
	PIDx->err = PIDx->setVal - PIDx->actVal;

	//---比例增量误差
	ep = PIDx->err - PIDx->err1;

	//---积分误差
	ei = PIDx->err;

	//---微分增量误差
	ed = PIDx->err - 2 * (PIDx->err1) + (PIDx->err2);

	//---计算值
	PIDx->calcVal = (PIDx->kP)*(ep)+(PIDx->kI)*(ei)+(PIDx->kD)*(ed);

	//---范围内的最小值
	PIDx->calcVal = RANGE(PIDx->calcVal, -PIDx->uMax, PIDx->uMax);

	//---保留上一次误差
	PIDx->err2 = PIDx->err1;

	//---保留当前误差
	PIDx->err1 = PIDx->err;

	//---当前实际值
	PIDx->actVal = PIDx->calcVal*PIDx->calcDelta;
	return  PIDx->actVal;
}