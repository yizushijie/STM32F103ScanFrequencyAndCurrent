#include "dht11_cfg.h"

//---变量定义
DHT11_HandlerType g_DHT11Device0 = { 0 };
pDHT11_HandlerType pDHT11Device0 = &g_DHT11Device0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_Device0_Init(DHT11_HandlerType *DHT11x)
{
	DHT11x->msgDAT.msgGPIOBit = LL_GPIO_PIN_14;
	DHT11x->msgDAT.msgGPIOPort = GPIOB;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_Device1_Init(DHT11_HandlerType *DHT11x)
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
UINT8_T DHT11_Device2_Init(DHT11_HandlerType *DHT11x)
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
UINT8_T DHT11_GPIO_Init(DHT11_HandlerType *DHT11x)
{
	//---使能端口时钟
	GPIOTask_Clock(DHT11x->msgDAT.msgGPIOPort, 1);
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO的初始化
	GPIO_InitStruct.Pin = DHT11x->msgDAT.msgGPIOBit;       //---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; //---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH; //---GPIO的速度
	GPIO_InitStruct.OutputType =
		LL_GPIO_OUTPUT_OPENDRAIN;           //---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP; //---上拉使能
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0; //---端口复用模式
#endif

  //---初始化端口
	LL_GPIO_Init(DHT11x->msgDAT.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_1(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_Init(DHT11_HandlerType *DHT11x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay))
{
	//---使用的DHT11的端口
	if ((DHT11x != NULL) && (DHT11x == DHT11_TASK_ONE))
	{
		DHT11_Device0_Init(DHT11x);
	}
	else if ((DHT11x != NULL) && (DHT11x == DHT11_TASK_TWO))
	{
		DHT11_Device1_Init(DHT11x);
	}
	else if ((DHT11x != NULL) && (DHT11x == DHT11_TASK_THREE))
	{
		DHT11_Device2_Init(DHT11x);
	}
	else
	{
		return ERROR_1;
	}

	//---GPIO的初始化
	DHT11_GPIO_Init(DHT11x);

	//---us延时
	if (pFuncDelayus != NULL)
	{
		DHT11x->msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		DHT11x->msgFuncDelayus = DelayTask_us;
	}

	//---ms延时
	if (pFuncDelayms != NULL)
	{
		DHT11x->msgFuncDelayms = pFuncDelayms;
	}
	else
	{
		DHT11x->msgFuncDelayms = DelayTask_ms;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_DeInit(DHT11_HandlerType *DHT11x)
{
	LL_GPIO_DeInit(DHT11x->msgDAT.msgGPIOPort);
	GPIO_OUT_1(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_RESET(DHT11_HandlerType *DHT11x)
{
	//---释放总线
	GPIO_OUT_1(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);

	//---主机拉高2us
	DHT11x->msgFuncDelayus(2);

	GPIO_OUT_0(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);

	//---触发开始,总线拉低要大于18ms
	DHT11x->msgFuncDelayms(20);

	//---释放总线
	GPIO_OUT_1(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);

	//主机拉高20~40us;等待DHT11的低电平响应信号
	DHT11x->msgFuncDelayus(30);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：1:不存在;0:存在
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_Check(DHT11_HandlerType *DHT11x)
{
	UINT8_T count = 0;

	//---DHT11如果响应的话会拉低总线40~80us
	while (GPIO_GET_STATE(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit) != 0)
	{
		DHT11x->msgFuncDelayus(1);
		count++;
		if (count > 200)
		{
			break;
		}
	}
	if (count > 200)
	{
		return ERROR_1;
	}
	count = 0;

	//---DHT11高电平数据准备信号再次拉高40~80us
	while (GPIO_GET_STATE(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit) == 0)
	{
		DHT11x->msgFuncDelayus(1);
		count++;
		if (count > 200)
		{
			break;
		}
	}

	if (count > 200)
	{
		return ERROR_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：1:不存在;0:存在
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_START(DHT11_HandlerType *DHT11x)
{
	//---触发传输
	UINT8_T _return = DHT11_RESET(DHT11x);
	if (_return)
	{
		return _return;
	}

	//---检测设备
	return DHT11_Check(DHT11x);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：位数据“0”的格式为： 50 微秒的低电平和 26-28 微秒的高电平;
/////////////  位数据“1”的格式为： 50 微秒的低电平加 70微秒的高电平
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_ReadBit(DHT11_HandlerType *DHT11x)
{
	UINT8_T count = 0;

	//---等待变为低电平---高电平保持的时间约为50us
	while (GPIO_GET_STATE(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit) != 0)
	{
		DHT11x->msgFuncDelayus(1);
		count++;
		if (count > 150)
		{
			count = 0;
			break;
		}
	}

	//---读取高电平等待一下
	DHT11x->msgFuncDelayus(10);

	//---等待变高电平---低电平保持的时间约为20us
	while (GPIO_GET_STATE(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit) == 0)
	{
		DHT11x->msgFuncDelayus(1);

		count++;
		if (count > 150)
		{
			count = 0;
			break;
		}
	}

	//---等待40us;数据为0的信号时间为26-28us，1则为70us
	DHT11x->msgFuncDelayus(50);

	//---读取状态
	if (GPIO_GET_STATE(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit) != 0)
	{
		return 1;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T DHT11_ReadByte(DHT11_HandlerType *DHT11x)
{
	UINT8_T i = 0, _return = 0;
	for (i = 0; i < 8; i++)
	{
		_return <<= 1;
		_return |= DHT11_ReadBit(DHT11x);
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
UINT8_T DHT11_Read(DHT11_HandlerType *DHT11x)
{
	UINT8_T temp[5] = { 0 };
	UINT8_T i = 0;

	//---启动数据的读取并检查设备是否存在
	if (DHT11_START(DHT11x) != OK_0)
	{
		return ERROR_1;
	}

	//---读取数据
	for (i = 0; i < 5; i++)
	{
		temp[i] = DHT11_ReadByte(DHT11x);
	}

	//---释放总线
	GPIO_OUT_1(DHT11x->msgDAT.msgGPIOPort, DHT11x->msgDAT.msgGPIOBit);

	//---数据校验
	if ((temp[0] + temp[1] + temp[2] + temp[3]) != temp[4])
	{
		return ERROR_2;
	}

	//---湿度整数部分
	DHT11x->msgHumiX1000 = temp[0];

	//---湿度小数部分
	DHT11x->msgHumiX1000 = (DHT11x->msgHumiX1000 * 1000) + temp[1];

	//---温度整数部分
	DHT11x->msgTempX1000 = temp[2];

	//---温度小数部分
	DHT11x->msgTempX1000 = (DHT11x->msgTempX1000 * 1000) + temp[3];
	return OK_0;
}