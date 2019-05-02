#include "pcf8563_cfg.h"

//===变量的定义
PCF8563_HandlerType g_PCF8563Device0 = { 0 };
pPCF8563_HandlerType pPCF8563Device0 = &g_PCF8563Device0;

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:isHWI2C---1 硬件，0 软件
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_Init(PCF8563_HandlerType *PCF8563x, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C)
{
	UINT8_T _return = OK_0;

	//---指定设备的初始化
	if ((PCF8563x != NULL) && (PCF8563x == PCF8563_TASK_ONE))
	{
		PCF8563_I2C_Device0_Init(PCF8563x);
	}
	else if ((PCF8563x != NULL) && (PCF8563x == PCF8563_TASK_TWO))
	{
		PCF8563_I2C_Device1_Init(PCF8563x);
	}
	else if ((PCF8563x != NULL) && (PCF8563x == PCF8563_TASK_THREE))
	{
		PCF8563_I2C_Device2_Init(PCF8563x);
	}
	else
	{
		return ERROR_1;
	}

	//---判断是硬件I2C还是软件I2C
	if (isHWI2C)
	{
		//---初始化硬件I2C
		_return = I2CTask_MHW_Init(&(PCF8563x->msgI2C));
		PCF8563x->msgI2C.msgModelIsHW = 1;
	}
	else
	{
		//---初始化软件模拟I2C
		_return = I2CTask_MSW_Init(&(PCF8563x->msgI2C), pFuncDelayus);
		PCF8563x->msgI2C.msgModelIsHW = 0;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_Device0_Init(PCF8563_HandlerType *PCF8563x)
{
	PCF8563x->msgI2C.msgI2Cx = NULL;
	PCF8563x->msgI2C.msgSCL.msgGPIOPort = GPIOB;
	PCF8563x->msgI2C.msgSCL.msgGPIOBit = LL_GPIO_PIN_6;
	PCF8563x->msgI2C.msgSDA.msgGPIOPort = GPIOB;
	PCF8563x->msgI2C.msgSDA.msgGPIOBit = LL_GPIO_PIN_7;
	PCF8563x->msgI2C.msgModelIsHW = 0;
	PCF8563x->msgI2C.msgPluseWidth = 0;
	PCF8563x->msgI2C.msgFuncDelayus = NULL;
	PCF8563x->msgI2C.msgAddr = 0xA2;//PCF8563_WRITE_ADDR;
	PCF8563x->msgI2C.msgClockSpeed = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_Device1_Init(PCF8563_HandlerType *PCF8563x)
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
UINT8_T PCF8563_I2C_Device2_Init(PCF8563_HandlerType *PCF8563x)
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
UINT8_T PCF8563_I2C_DeInit(PCF8563_HandlerType *PCF8563x)
{
	//---注销I2C设备
	if (PCF8563x->msgI2C.msgModelIsHW == 1)
	{
		return ERROR_1;
	}
	else
	{
		return I2CTask_MSW_DeInit(&(PCF8563x->msgI2C));
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_SWI2C_WriteReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T val)
{
	UINT8_T _return = OK_0;

	//---启动并发送地址
	_return = I2CTask_MSW_START(&(PCF8563x->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}

	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(PCF8563x->msgI2C), reg);

	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(PCF8563x->msgI2C));
	if (_return != OK_0)
	{
		//---发送数据失败
		_return = ERROR_3;
		goto GoToExit;
	}

	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(PCF8563x->msgI2C), val);

	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(PCF8563x->msgI2C));
GoToExit:

	//---发送停止信号
	I2CTask_MSW_STOP(&(PCF8563x->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_HWI2C_WriteReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T val)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：写寄存器
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_WriteReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T val)
{
	if (PCF8563x->msgI2C.msgModelIsHW == 0)
	{
		return PCF8563_SWI2C_WriteReg(PCF8563x, reg, val);
	}
	else
	{
		return PCF8563_HWI2C_WriteReg(PCF8563x, reg, val);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：读取寄存器
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_SWI2C_ReadReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T *pVal, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;

	//---启动写数据
	_return = I2CTask_MSW_START(&(PCF8563x->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}

	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(PCF8563x->msgI2C), reg);

	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(PCF8563x->msgI2C));
	if (_return != OK_0)
	{
		//---发送数据失败
		_return = ERROR_3;
		goto GoToExit;
	}

	//---发送停止信号
	I2CTask_MSW_STOP(&(PCF8563x->msgI2C));

	//---启动读取数据
	_return = I2CTask_MSW_START(&(PCF8563x->msgI2C), 0);
	if (_return != OK_0)
	{
		//---启动读数据失败
		_return = ERROR_4;
		goto GoToExit;
	}
	for (i = 0; i < length; i++)
	{
		//---读取数据
		pVal[i] = I2CTask_MSW_ReadByte(&(PCF8563x->msgI2C));
		if (i == (length - 1))
		{
			_return = 1;
		}

		//---发送应答信号
		I2CTask_MSW_SendACK(&(PCF8563x->msgI2C), _return);
	}
	_return = OK_0;
GoToExit:

	//---发送停止信号
	I2CTask_MSW_STOP(&(PCF8563x->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_HWI2C_ReadReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T *pVal, UINT16_T length)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：读取寄存器
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_ReadReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T *pVal, UINT16_T length)
{
	if (PCF8563x->msgI2C.msgModelIsHW == 0)
	{
		return PCF8563_SWI2C_ReadReg(PCF8563x, reg, pVal, length);
	}
	else
	{
		return PCF8563_HWI2C_ReadReg(PCF8563x, reg, pVal, length);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数： UINT8_T PCF8563_ClockOut(UINT8_T preVal)
//////功	   能： PCF8563输出时钟，输出分频
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_ClockOut(PCF8563_HandlerType *PCF8563x, UINT8_T preVal)
{
	UINT8_T _return = OK_0;
	UINT8_T temp = 0;

	//---写寄存器
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_CLKOUT, (preVal | 0x80));
	if (_return != OK_0)
	{
		_return = ERROR_2;
		goto GoToExit;
	}

	//---读取寄存器
	_return = PCF8563_I2C_ReadReg(PCF8563x, PCF8563_REG_CLKOUT, &temp, 1);
	if (_return != OK_0)
	{
		_return = ERROR_3;
		goto GoToExit;
	}

	//---数据的校验
	if ((temp & 0x7F) != preVal)
	{
		_return = ERROR_1;
	}
GoToExit:
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_ReadRTC(PCF8563_HandlerType *PCF8563x)
{
	UINT8_T _return = OK_0;
	UINT8_T temp[7];

	//---读取时钟
	_return = PCF8563_I2C_ReadReg(PCF8563x, PCF8563_REG_SECOND, temp, 7);

	//---解析数据
	if (_return == OK_0)
	{
		//---将获取的数据填充到缓存区
		PCF8563x->msgRTC.second = (temp[0] & 0x7F);
		PCF8563x->msgRTC.minute = (temp[1] & 0x7F);
		PCF8563x->msgRTC.hour = (temp[2] & 0x3F);
		PCF8563x->msgRTC.day = (temp[3] & 0x3F);
		PCF8563x->msgRTC.week = (temp[4] & 0x07);
		PCF8563x->msgRTC.month = (temp[5] & 0x1F);

		//---年份
		PCF8563x->msgRTC.year = temp[6];

		//---世纪
		if (temp[5] & 0x80)
		{
			PCF8563x->msgRTC.century = 0x19;
		}
		else
		{
			PCF8563x->msgRTC.century = 0x20;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T PCF8563_I2C_WriteRTC(PCF8563_HandlerType *PCF8563x, RTC_HandlerType rtc)
{
	UINT8_T _return = OK_0;

	//---判断数据类型是不是BCD码
	if ((rtc.century != 0x19) || (rtc.century != 0x20))
	{
		//===处理数据位BCD格式
		rtc.second = DecToBcd(rtc.second);
		rtc.minute = DecToBcd(rtc.minute);
		rtc.hour = DecToBcd(rtc.hour);
		rtc.day = DecToBcd(rtc.day);
		rtc.week = DecToBcd(rtc.week);
		rtc.month = DecToBcd(rtc.month);
		rtc.year = DecToBcd(rtc.year);
		rtc.century = DecToBcd(rtc.century);
	}

	//---世纪参数的处理
	if (rtc.century == 0x20)
	{
		rtc.month &= 0x3F;
	}
	else
	{
		rtc.month |= 0x80;
	}

	//---写秒
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_SECOND, rtc.second);
	if (_return != OK_0)
	{
		_return = ERROR_2;
		goto GoToExit;
	}

	//---写分
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_MINUTE, rtc.minute);
	if (_return != OK_0)
	{
		_return = ERROR_3;
		goto GoToExit;
	}

	//---写时
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_HOURS, rtc.hour);
	if (_return != OK_0)
	{
		_return = ERROR_4;
		goto GoToExit;
	}

	//---写天
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_DAY, rtc.day);
	if (_return != OK_0)
	{
		_return = ERROR_5;
		goto GoToExit;
	}

	//---写星期
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_WEEK, rtc.week);
	if (_return != OK_0)
	{
		_return = ERROR_6;
		goto GoToExit;
	}

	//---写月
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_MONTH, rtc.month);
	if (_return != OK_0)
	{
		_return = ERROR_7;
		goto GoToExit;
	}

	//---写年
	_return = PCF8563_I2C_WriteReg(PCF8563x, PCF8563_REG_YEAR, rtc.year);
GoToExit:
	return _return;
}