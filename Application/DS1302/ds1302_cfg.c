#include "ds1302_cfg.h"

//===全局变量定义
DS1302_HandlerType		g_DS1302Device0 = { 0 };
pDS1302_HandlerType		pDS1302Device0 = &g_DS1302Device0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DS1302_Device0_Init(DS1302_HandlerType *DS1302x)
{
	DS1302x->msgCS.msgGPIOPort = GPIOB;
	DS1302x->msgCS.msgGPIOBit = LL_GPIO_PIN_6;

	DS1302x->msgCLK.msgGPIOPort = GPIOB;
	DS1302x->msgCLK.msgGPIOBit = LL_GPIO_PIN_7;

	DS1302x->msgDAT.msgGPIOPort = GPIOB;
	DS1302x->msgDAT.msgGPIOBit = LL_GPIO_PIN_8;

	DS1302x->msgPluseWidth = 2;
	DS1302x->msgFuncDelayus = NULL;

	//---使能端口时钟
	GPIOTask_Clock(DS1302x->msgCS.msgGPIOPort, 1);
	GPIOTask_Clock(DS1302x->msgCLK.msgGPIOPort, 1);
	GPIOTask_Clock(DS1302x->msgDAT.msgGPIOPort, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---CS端口的初始化
	GPIO_InitStruct.Pin = DS1302x->msgCS.msgGPIOBit;
	LL_GPIO_Init(DS1302x->msgCS.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_0(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);

	//---CLK端口的初始化
	GPIO_InitStruct.Pin = DS1302x->msgCLK.msgGPIOBit;
	LL_GPIO_Init(DS1302x->msgCLK.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_1(DS1302x->msgCLK.msgGPIOPort, DS1302x->msgCLK.msgGPIOBit);

	//---DAT端口的初始化
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pin = DS1302x->msgDAT.msgGPIOBit;
	LL_GPIO_Init(DS1302x->msgDAT.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_1(DS1302x->msgDAT.msgGPIOPort, DS1302x->msgDAT.msgGPIOBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DS1302_Device1_Init(DS1302_HandlerType *DS1302x)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DS1302_Device2_Init(DS1302_HandlerType *DS1302x)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DS1302_Init(DS1302_HandlerType *DS1302x, void(*pFuncDelayus)(UINT32_T delay))
{
	//---指定设备的初始化
	if ((DS1302x != NULL) && (DS1302x == DS1302_TASK_ONE))
	{
		DS1302_Device0_Init(DS1302x);
	}
	else if ((DS1302x != NULL) && (DS1302x == DS1302_TASK_TWO))
	{
		DS1302_Device1_Init(DS1302x);
	}
	else if ((DS1302x != NULL) && (DS1302x == DS1302_TASK_THREE))
	{
		DS1302_Device2_Init(DS1302x);
	}
	else
	{
		return ERROR_1;
	}

   //---us延时
	if (pFuncDelayus!=NULL)
	{
		DS1302x->msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		DS1302x->msgFuncDelayus = DelayTask_us;
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：低位在前
//////////////////////////////////////////////////////////////////////////////
void DS1302_WriteBitLSB(DS1302_HandlerType *DS1302x, UINT8_T wVal, UINT8_T *pRVal)
{
	//---发送1bit的数据
	if ((wVal & 0x01) == 0x00)
	{
		GPIO_OUT_0(DS1302x->msgDAT.msgGPIOPort, DS1302x->msgDAT.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(DS1302x->msgDAT.msgGPIOPort, DS1302x->msgDAT.msgGPIOBit);
	}

	//---读取1bit的数据
	if (GPIO_GET_STATE(DS1302x->msgDAT.msgGPIOPort, DS1302x->msgDAT.msgGPIOBit) != 0)
	{
		if (pRVal != NULL)
		{
			*pRVal |= 0x80;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void DS1302_WriteByteLSB(DS1302_HandlerType *DS1302x, UINT8_T wVal, UINT8_T *pRVal)
{
	UINT8_T i = 0;
	if (pRVal != NULL)
	{
		*pRVal = 0;
	}
	for (i = 0; i < 8; i++)
	{
		if (pRVal != NULL)
		{
			*pRVal >>= 1;
		}

		//---读写1BIT的数据
		DS1302_WriteBitLSB(DS1302x, wVal, pRVal);
		wVal >>= 1;
		GPIO_OUT_1(DS1302x->msgCLK.msgGPIOPort, DS1302x->msgCLK.msgGPIOBit);
		if (DS1302x->msgPluseWidth>0)
		{
			DS1302x->msgFuncDelayus(DS1302x->msgPluseWidth);
		}
		GPIO_OUT_0(DS1302x->msgCLK.msgGPIOPort, DS1302x->msgCLK.msgGPIOBit);
		if (DS1302x->msgPluseWidth>0)
		{
			DS1302x->msgFuncDelayus(DS1302x->msgPluseWidth);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void DS1302_WriteReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat)
{
	GPIO_OUT_0(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
	GPIO_OUT_0(DS1302x->msgCLK.msgGPIOPort, DS1302x->msgCLK.msgGPIOBit);
	GPIO_OUT_1(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
	if (DS1302x->msgPluseWidth>0)
	{
		DS1302x->msgFuncDelayus(DS1302x->msgPluseWidth);
	}
	DS1302_WriteByteLSB(DS1302x, addr, NULL);
	DS1302_WriteByteLSB(DS1302x, dat, NULL);
	GPIO_OUT_0(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void DS1302_ReadReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal)
{
	GPIO_OUT_0(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
	GPIO_OUT_0(DS1302x->msgCLK.msgGPIOPort, DS1302x->msgCLK.msgGPIOBit);
	GPIO_OUT_1(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
	if (DS1302x->msgPluseWidth>0)
	{
		DS1302x->msgFuncDelayus(DS1302x->msgPluseWidth);
	}
	DS1302_WriteByteLSB(DS1302x, (addr | 0x01), pVal);
	GPIO_OUT_0(DS1302x->msgCS.msgGPIOPort, DS1302x->msgCS.msgGPIOBit);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：编程时间
//////////////////////////////////////////////////////////////////////////////
void DS1302_WriteTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat)
{
	//---取消写保护
	DS1302_WriteReg(DS1302x, DS1302_REG_CONTROL, 0x00);

	//---在指定的位置写入指定的数据
	DS1302_WriteReg(DS1302x, addr, dat);

	//---打开写保护
	DS1302_WriteReg(DS1302x, DS1302_REG_CONTROL, 0x80);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：读取时间
//////////////////////////////////////////////////////////////////////////////
void DS1302_ReadTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal)
{
	DS1302_ReadReg(DS1302x, addr, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：编程实时时钟
//////////////////////////////////////////////////////////////////////////////
void DS1302_WriteRTC(DS1302_HandlerType *DS1302x, RTC_HandlerType rtcTime)
{
	DS1302x->msgRTC = rtcTime;

	//---取消写保护
	DS1302_WriteReg(DS1302x, DS1302_REG_CONTROL, 0x00);

	//---写秒数据
	DS1302_WriteReg(DS1302x, DS1302_REG_SECOND, rtcTime.second);

	//---写分数据
	DS1302_WriteReg(DS1302x, DS1302_REG_MINUTE, rtcTime.minute);

	//---写时数据
	DS1302_WriteReg(DS1302x, DS1302_REG_HOUR, rtcTime.hour);

	//---写日数据
	DS1302_WriteReg(DS1302x, DS1302_REG_DAY, rtcTime.day);

	//---写星期数据
	DS1302_WriteReg(DS1302x, DS1302_REG_WEEK, rtcTime.week);

	//---写月数据
	DS1302_WriteReg(DS1302x, DS1302_REG_MONTH, rtcTime.month);

	//---写年数据
	DS1302_WriteReg(DS1302x, DS1302_REG_YEAR, rtcTime.year);

	//---打开写保护
	DS1302_WriteReg(DS1302x, DS1302_REG_CONTROL, 0x80);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：读取实时时钟
//////////////////////////////////////////////////////////////////////////////
void DS1302_ReadRTC(DS1302_HandlerType *DS1302x)
{
	//---读秒数据
	DS1302_ReadReg(DS1302x, DS1302_REG_SECOND, &(DS1302x->msgRTC.second));

	//---写分数据
	DS1302_ReadReg(DS1302x, DS1302_REG_MINUTE, &(DS1302x->msgRTC.minute));

	//---写时数据
	DS1302_ReadReg(DS1302x, DS1302_REG_HOUR, &(DS1302x->msgRTC.hour));

	//---写日数据
	DS1302_ReadReg(DS1302x, DS1302_REG_DAY, &(DS1302x->msgRTC.day));

	//---写星期数据
	DS1302_ReadReg(DS1302x, DS1302_REG_WEEK, &(DS1302x->msgRTC.week));

	//---写月数据
	DS1302_ReadReg(DS1302x, DS1302_REG_MONTH, &(DS1302x->msgRTC.month));

	//---写年数据
	DS1302_ReadReg(DS1302x, DS1302_REG_YEAR, &(DS1302x->msgRTC.year));
}