#include "one_wire_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T OneWire_Init(OneWire_HandlerType *OneWirex, void(*pFuncDelayus)(UINT32_T delay))
{
	//---使能端口时钟
	GPIOTask_Clock(OneWirex->msgDAT.msgGPIOPort, 1);
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO的初始化
	GPIO_InitStruct.Pin = OneWirex->msgDAT.msgGPIOBit;			//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;					//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;			//---GPIO的速度
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;				//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;						//---上拉使能

	//---初始化端口
	LL_GPIO_Init(OneWirex->msgDAT.msgGPIOPort, &GPIO_InitStruct);

	//---端口输出高电平
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//---延时函数
	if (pFuncDelayus != NULL)
	{
		OneWirex->msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		OneWirex->msgFuncDelayus = DelayTask_us;
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
UINT8_T OneWire_DeInit(OneWire_HandlerType *OneWirex)
{
	LL_GPIO_DeInit(OneWirex->msgDAT.msgGPIOPort);
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T OneWire_START(OneWire_HandlerType *OneWirex)
{
	UINT8_T _return = OK_0;

	//---设置为输出且输出零
	GPIO_OUT_0(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//>=480
	OneWirex->msgFuncDelayus(500);
	//---输出高电平
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//15us~60us之间
	OneWirex->msgFuncDelayus(50);
	_return = GPIO_GET_STATE(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//60us~240us
	OneWirex->msgFuncDelayus(200);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T OneWire_WriteBit(OneWire_HandlerType *OneWirex, UINT8_T bitVal)
{
	//---控制总线，强制拉低
	GPIO_OUT_0(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//=4
	OneWirex->msgFuncDelayus(4);
	//---发送数据
	if (bitVal)
	{
		//---释放总线
		GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);
	}

	//=60us
	OneWirex->msgFuncDelayus(50);
	//---释放总线
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//=4
	OneWirex->msgFuncDelayus(4);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T OneWire_ReadBit(OneWire_HandlerType *OneWirex)
{
	UINT8_T _return = 0;

	//---控制总线,强制拉低
	GPIO_OUT_0(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//=8
	OneWirex->msgFuncDelayus(8);

	//---释放总线
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);

	//=14
	OneWirex->msgFuncDelayus(14);
	//---读取数据
	if (GPIO_GET_STATE(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit))
	{
		_return = 1;
	}

	//=40
	OneWirex->msgFuncDelayus(40);
	//---释放总线
	GPIO_OUT_1(OneWirex->msgDAT.msgGPIOPort, OneWirex->msgDAT.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T OneWire_WriteByte(OneWire_HandlerType *OneWirex, UINT8_T val)
{
	UINT8_T i = 0;
	for (i = 0; i < 8; i++)
	{
		//---先发送低位数据
		OneWire_WriteBit(OneWirex, (val & 0x01));
		val >>= 1;
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
UINT8_T OneWire_ReadByte(OneWire_HandlerType *OneWirex)
{
	UINT8_T _return = 0, i = 0;
	for (i = 0; i < 8; i++)
	{
		_return |= (OneWire_ReadBit(OneWirex) << i);
	}

	//=5
	OneWirex->msgFuncDelayus(5);

	return _return;
}