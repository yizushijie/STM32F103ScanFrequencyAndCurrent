#include "isp_cfg.h"

//===全局变量的定义
ISP_HandlerType  g_ISPDevice0 = { 0 };
pISP_HandlerType pISPDevice0 = &g_ISPDevice0;

//===统一发送函数
UINT8_T(*ISP_SEND_CMD)(ISP_HandlerType *, UINT8_T, UINT8_T, UINT8_T, UINT8_T);

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_HW_Init(ISP_HandlerType *ISPx)
{
	//---注销当前的所有配置
	SPITask_DeInit(&(ISPx->msgSPI));
	
	//---硬件端口的配置---硬件实现
	SPITask_MHW_GPIO_Init(&(ISPx->msgSPI));

	//---硬件SPI的初始化
	LL_SPI_InitTypeDef SPI_InitStruct={0};

	//---SPI的模式配置
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;						//---主机模式
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;				//---8位数据
	
	//---时钟空闲时的极性
	if(ISPx->msgSPI.msgCPOL==0)
	{
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;				//---CLK空闲时为低电平 (CLK空闲是只能是低电平)
		GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;				//---CLK空闲时为高电平 (CLK空闲是只能是低电平)
	}
	
	//---数据采样的时钟边沿位置
	if (ISPx->msgSPI.msgCPOL == 0)
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	}
	else
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	}
	
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;							//---软件控制
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;		//---系统时钟256分频
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;						//---高位在前
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;	//---硬件CRC不使能
	SPI_InitStruct.CRCPoly = 7;
	SPITask_MHW_PollMode_Init(&(ISPx->msgSPI), SPI_InitStruct);
	ISPx->msgInit = 1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SW_Init(ISP_HandlerType *ISPx)
{
	SPITask_DeInit(&(ISPx->msgSPI));

	//---硬件端口的配置---软件实现
	SPITask_MSW_GPIO_Init(&(ISPx->msgSPI));

	//---时钟空闲时的极性
	if (ISPx->msgSPI.msgCPOL == 0)
	{
		GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
	}

	//---除片选信号输出高电平，其余端口都输出低电平，默认的初始化是高电平，在这里需要改动
	GPIO_OUT_0(ISPx->msgSPI.msgMOSI.msgGPIOPort, ISPx->msgSPI.msgMOSI.msgGPIOBit);
	GPIO_OUT_0(ISPx->msgSPI.msgMISO.msgGPIOPort, ISPx->msgSPI.msgMISO.msgGPIOBit);

	ISPx->msgInit = 1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Device0_Init(ISP_HandlerType *ISPx)
{
	//---设定初始化状态
	ISPx->msgInit = 0;

	//---设定硬件的时钟
	ISPx->msgSetClok = ISP_SCK_KHZ_32;//ISP_SCK_PRE_256;//

	//---初始化
	ISPx->msgDelayms = 0;

	//---初始化
	ISPx->msgHideAddr = 0;

	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);

	//---CS
	ISPx->msgSPI.msgCS.msgGPIOPort = GPIOA;
	ISPx->msgSPI.msgCS.msgGPIOBit = LL_GPIO_PIN_4;

	//---SCK
	ISPx->msgSPI.msgSCK.msgGPIOPort = GPIOA;
	ISPx->msgSPI.msgSCK.msgGPIOBit = LL_GPIO_PIN_5;

	//---MISO
	ISPx->msgSPI.msgMISO.msgGPIOPort = GPIOA;
	ISPx->msgSPI.msgMISO.msgGPIOBit = LL_GPIO_PIN_6;

	//---MOSI
	ISPx->msgSPI.msgMOSI.msgGPIOPort = GPIOA;
	ISPx->msgSPI.msgMOSI.msgGPIOBit = LL_GPIO_PIN_7;

	//---复用模式
#ifndef USE_MCU_STM32F1
	ISPx->msgSPI.msgGPIOAlternate = LL_GPIO_AF_5;
#endif

	//---SPI序号
	ISPx->msgSPI.msgSPIx = SPI1;
#ifndef USE_MCU_STM32F1

	//---SPI的协议
	ISPx->msgSPI.msgStandard = LL_SPI_PROTOCOL_MOTOROLA;
#endif
	
	//---时钟空闲为低电平
	ISPx->msgSPI.msgCPOL=0;
	//---数据采样在第一个时钟边沿
	ISPx->msgSPI.msgCPOH=0;
	
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Device1_Init(ISP_HandlerType *ISPx)
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
UINT8_T ISP_Device2_Init(ISP_HandlerType *ISPx)
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
UINT8_T ISP_Init(ISP_HandlerType *ISPx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	//---使用的DHT11的端口
	if ((ISPx != NULL) && (ISPx == ISP_TASK_ONE))
	{
		ISP_Device0_Init(ISPx);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_TWO))
	{
		ISP_Device1_Init(ISPx);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_THREE))
	{
		ISP_Device2_Init(ISPx);
	}
	else
	{
		return ERROR_1;
	}

	//---注册ms的延时函数
	if (pFuncDelayms!=NULL)
	{
		ISPx->msgFuncDelayms = pFuncDelayms;
	}
	else
	{
		ISPx->msgFuncDelayms = DelayTask_ms;
	}

	//---注册us延时函数
	if (pFuncDelayus!=NULL)
	{
		ISPx->msgSPI.msgFuncDelayus = pFuncDelayus;

	}
	else
	{
		ISPx->msgSPI.msgFuncDelayus = DelayTask_us;
	}
	
	//---注册滴答函数
	ISPx->msgSPI.msgFuncTimeTick = pFuncTimerTick;
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_DeInit(ISP_HandlerType *ISPx)
{
	SPITask_DeInit(&(ISPx->msgSPI));
	ISPx->msgInit = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SetClock(ISP_HandlerType *ISPx, UINT8_T clok)
{
	switch (clok)
	{
		case ISP_SCK_KHZ_0_5:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 1000;
			break;
		case ISP_SCK_KHZ_1:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 500;
			break;
		case ISP_SCK_KHZ_2:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 250;
			break;
		case ISP_SCK_KHZ_4:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 125;
			break;
		case ISP_SCK_KHZ_8:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 64;
			break;
		case ISP_SCK_KHZ_16:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 32;
			break;
		case ISP_SCK_KHZ_32:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 16;
			break;
		case ISP_SCK_KHZ_64:
			ISPx->msgSPI.msgModelIsHW = 0;
			ISPx->msgSPI.msgPluseWidth = 8;
			break;
		case ISP_SCK_PRE_256:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV256;
			break;
		case ISP_SCK_PRE_128:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV128;
			break;
		case ISP_SCK_PRE_64:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV64;
			break;
		case ISP_SCK_PRE_32:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV32;
			break;
		case ISP_SCK_PRE_16:
		#ifdef USE_MCU_STM32
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV16;
		#endif
			break;
		case ISP_SCK_PRE_8:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV8;
			break;
		case ISP_SCK_PRE_4:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV4;
			break;
		case ISP_SCK_PRE_2:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV2;
			break;
		default:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV256;
			break;
	}
	if (ISPx->msgSPI.msgModelIsHW != 1)
	{
		//---第一次需要初始化端口，或则从模拟方式切换到硬件方式，也需要重新初始化一下端口
		if ((ISPx->msgInit == 0) || (ISP_SEND_CMD == ISP_SW_SendCmd))
		{
			ISP_HW_Init(ISPx);
		}

		//---设置SPI的硬件时钟
		SPITask_MHW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgClockSpeed);

		//---传递发送命令函数
		ISP_SEND_CMD = ISP_HW_SendCmd;
	}
	else
	{
		//---第一次需要初始化端口，或则从硬件方式切换到模拟方式，也需要重新初始化一下端口
		if ((ISPx->msgInit == 0) || (ISP_SEND_CMD == ISP_HW_SendCmd))
		{
			ISP_SW_Init(ISPx);
		}

		//---设置软件模拟的脉宽
		SPITask_MSW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgPluseWidth);

		//---传递发送命令函数
		ISP_SEND_CMD = ISP_SW_SendCmd;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SW_SendCmd(ISP_HandlerType *ISPx, UINT8_T val1, UINT8_T Val2, UINT8_T val3, UINT8_T val4)
{
	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);

	//---填充发送缓存区
	ISPx->msgWriteByte[0] = val1;
	ISPx->msgWriteByte[1] = Val2;
	ISPx->msgWriteByte[2] = val3;
	ISPx->msgWriteByte[3] = val4;

	//---数据发送，高位在前
	return SPITask_MSW_WriteAndReadDataMSB(&(ISPx->msgSPI), ISPx->msgWriteByte, ISPx->msgReadByte, ISP_COMM_MAX_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_HW_SendCmd(ISP_HandlerType *ISPx, UINT8_T val1, UINT8_T Val2, UINT8_T val3, UINT8_T val4)
{
	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);

	//---填充发送缓存区
	ISPx->msgWriteByte[0] = val1;
	ISPx->msgWriteByte[1] = Val2;
	ISPx->msgWriteByte[2] = val3;
	ISPx->msgWriteByte[3] = val4;

	//---数据发送
	return SPITask_MHW_PollMode_WriteAndReadData(&(ISPx->msgSPI), ISPx->msgWriteByte, ISPx->msgReadByte, ISP_COMM_MAX_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_PreEnterProg(ISP_HandlerType *ISPx)
{
	//---设置端口CS端口为输出模式
	GPIO_SET_WRITE(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);

	//---首先拉低时钟线
	GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);

	//---打开电源
	//POWER_DUT_ON;
	//---首先拉低时钟线
	GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
	ISPx->msgFuncDelayms(1);
	GPIO_OUT_1(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
	ISPx->msgFuncDelayms(1);
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
	ISPx->msgFuncDelayms(1);
	//---解除64K的限制
	ISPx->msgHideAddr = 0xFF;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_EnterProg(ISP_HandlerType *ISPx)
{
	UINT8_T count = ISP_SCK_AUTO_MAX_COUNT;

	//---设置时钟
	ISP_SetClock(ISPx, ISPx->msgSetClok);

	//---编程模式准备
	ISP_PreEnterProg(ISPx);

	//---自动获取合适的速度
	while (count--)
	{
		//---发送编程命令
		ISP_SEND_CMD(ISPx, 0xAC, 0x53, 0x00, 0x00);

		//---检查是否进入成功
		if (ISPx->msgReadByte[2] == 0x53)
		{
			return OK_0;
		}

		//---降速处理
		if (ISPx->msgSetClok >= ISP_SCK_KHZ_2)
		{
			ISPx->msgSetClok = ISPx->msgSetClok;
		}

		//---设置时钟
		ISP_SetClock(ISPx, ISPx->msgSetClok);

		//---置位时钟线和片选端
		GPIO_OUT_1(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
		GPIO_OUT_1(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
		ISPx->msgFuncDelayms(1);

		//---清零时钟线和片选端
		GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
		GPIO_OUT_0(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
		ISPx->msgFuncDelayms(1);
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ExitProg(ISP_HandlerType *ISPx)
{
	ISP_DeInit(ISPx);

	//---恢复时钟的速度
	ISPx->msgSetClok = ISP_SCK_MAX_CLOCK;

	//---解除64K的限制
	ISPx->msgHideAddr = 0xFF;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadReady(ISP_HandlerType *ISPx)
{
	UINT8_T _return = 0;

	//---获取时间标签
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;
	if (ISPx->msgSPI.msgFuncTimeTick != NULL)
	{
		//nowTime = ISPx->msgSPI.msgFuncTick();
		oldTime = ISPx->msgSPI.msgFuncTimeTick();
	}

	//---查询忙标志位
	while (1)
	{
		//---发送读取忙标志的命令
		_return = ISP_SEND_CMD(ISPx, 0xF0, 0x00, 0x00, 0x00);

		//---检查是否读取到忙标志
		if (_return)
		{
			break;
		}
		else
		{
			if ((ISPx->msgReadByte[3] & 0x01) == 0x00)
			{
				_return = OK_0;
				break;
			}
			else
			{
				if (ISPx->msgSPI.msgFuncTimeTick != NULL)
				{
					//if ((ISPx->msgSPI.msgFuncTick() - nowTime) > 100)
					//{
					//	//---发送发生超时错误
					//	_return = ERROR_2;
					//	break;
					//}
					//---当前时间
					nowTime = ISPx->msgSPI.msgFuncTimeTick();

					//---判断滴答定时是否发生溢出操作
					if (nowTime < oldTime)
					{
						cnt = (0xFFFFFFFF - oldTime + nowTime);
					}
					else
					{
						cnt = nowTime - oldTime;
					}

					//---判断是否超时
					if (cnt > 100)
					{
						//---发送发生超时错误
						_return = ERROR_2;
						break;
					}
				}
				else
				{
					nowTime++;
					if (nowTime > 100000)
					{
						//---发送发生超时错误
						_return = ERROR_3;
						break;
					}
				}
			}
		}
		WDT_RESET();
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
UINT8_T ISP_EraseChip(ISP_HandlerType *ISPx)
{
	UINT8_T _return = 0;

	//---发送擦除命令
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0x80, 0x00, 0x00);
	if (_return == 0)
	{
		//---擦除之后的等待延时
		ISPx->msgFuncDelayms(10 + ISPx->msgDelayms);
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
UINT8_T ISP_ReadChipID(ISP_HandlerType *ISPx, UINT8_T *pVal)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	for (i = 0; i < 3; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0x30, 0x00, i, 0x0);
		if (_return != 0)
		{
			_return = (i + 2);
			break;
		}
		pVal[i] = ISPx->msgReadByte[3];
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
UINT8_T ISP_ReadChipCalibration(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0x38, 0x00, i, 0x00);
		if (_return != 0)
		{
			_return = (i + 2);
			break;
		}
		pVal[i] = ISPx->msgReadByte[3];
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
UINT8_T ISP_ReadChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = 0;

	//---读取熔丝位低位
	_return = ISP_SEND_CMD(ISPx, 0x50, 0x00, 0x00, 0x00);
	if (_return != 0x00)
	{
		return ERROR_2;
	}

	//---保存低位值
	*(pVal++) = ISPx->msgReadByte[3];

	//---读取熔丝位高位
	_return = ISP_SEND_CMD(ISPx, 0x58, 0x08, 0x00, 0x00);
	if (_return != 0x00)
	{
		return ERROR_3;
	}

	//---保存高位值
	*(pVal++) = ISPx->msgReadByte[3];

	//---读取熔丝位拓展位
	if (isNeedExternFuse != 0x00)
	{
		//---读取熔丝的拓展位值
		_return = ISP_SEND_CMD(ISPx, 0x50, 0x08, 0x00, 0x00);

		//---保存拓展位值
		*(pVal) = ISPx->msgReadByte[3];
	}
	else
	{
		*pVal = 0xFF;
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
UINT8_T ISP_ReadChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal)
{
	//---读取加密位
	UINT8_T _return = ISP_SEND_CMD(ISPx, 0x58, 0x00, 0x00, 0x00);
	*(pVal) = ISPx->msgReadByte[3];
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipRom(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T addr, UINT16_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;

	//---判断数据长度是否是偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_2;
	}

	//---用移位运算当做除2运算
	length >>= 1;
	for (i = 0; i < length; i++)
	{
		//---ROM页低位数据
		_return = ISP_SEND_CMD(ISPx, 0x30, 0x00, (addr + i), 0x00);
		if (_return != 0)
		{
			return ERROR_3;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}

		//---ROM页高字节数据
		_return = ISP_SEND_CMD(ISPx, 0x38, 0x00, (addr + i), 0x00);
		if (_return != 0)
		{
			return 4;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
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
UINT8_T ISP_WriteChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = 0;

	//---写入熔丝位低位
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA0, 0x00, pVal[0]);
	if (_return != 0x00)
	{
		return ERROR_2;
	}

	//---写入之后延时等待
	ISPx->msgFuncDelayms(6 + ISPx->msgDelayms);

	//---保存低位值
	*(pVal++) = ISPx->msgReadByte[3];

	//---读取熔丝位高位
	//---写入熔丝位高位
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA8, 0x00, pVal[1]);
	if (_return != 0x00)
	{
		return ERROR_3;
	}

	//---写入之后延时等待
	ISPx->msgFuncDelayms(6 + ISPx->msgDelayms);

	//---写入熔丝位拓展位
	if (isNeedExternFuse != 0x00)
	{
		//---写入拓展位
		_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA4, 0x00, pVal[2]);

		//---判断写入是否成功
		if (_return == OK_0)
		{
			//---写入之后延时等待
			ISPx->msgFuncDelayms(6 + ISPx->msgDelayms);
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
UINT8_T ISP_WriteChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal)
{
	//---写入加密位
	UINT8_T _return = ISP_SEND_CMD(ISPx, 0xAC, 0xE0, 0x00, pVal[0]);

	//---判断写入是否成功
	if (_return == OK_0)
	{
		//---写入之后延时等待
		ISPx->msgFuncDelayms(6 + ISPx->msgDelayms);
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
UINT8_T ISP_ReadChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0xA0, highAddr, lowAddr, 0x00);
		if (_return != OK_0)
		{
			break;
		}

		//---保存读取的数据
		*(pVal++) = ISPx->msgReadByte[3];

		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
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
UINT8_T ISP_ReadChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_ReadChipEepromAddr(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0xC0, highAddr, lowAddr, pVal[i]);
		if (_return != OK_0)
		{
			break;
		}

		//---写入之后延时等待
		ISPx->msgFuncDelayms(5 + ISPx->msgDelayms);

		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
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
UINT8_T ISP_WriteChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_WriteChipEepromAddr(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：如果写入的数据是空数据，那么就跳过写入
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		if (pVal[i] != 0xFF)
		{
			_return = ISP_SEND_CMD(ISPx, 0xC0, highAddr, lowAddr, pVal[i]);
			if (_return != OK_0)
			{
				break;
			}

			//---写入之后延时等待
			ISPx->msgFuncDelayms(5 + ISPx->msgDelayms);
		}

		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
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
UINT8_T ISP_WriteChipEepromLongAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_WriteChipEepromAddrWithJumpEmpty(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateExternAddr(ISP_HandlerType *ISPx, UINT8_T addr)
{
	UINT8_T _return = OK_0;
	if (ISPx->msgHideAddr != addr)
	{
		ISPx->msgHideAddr = addr;
		_return = ISP_SEND_CMD(ISPx, 0x4D, 0x00, addr, 0x00);
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
UINT8_T ISP_UpdateExternLongAddr(ISP_HandlerType *ISPx, UINT32_T addr)
{
	return ISP_UpdateExternAddr(ISPx, (UINT8_T)(addr >> 16));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;

	//---判断数据长度是否为偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_2;
	}

	//---用移位运算当做除2运算
	length >>= 1;

	//---更新拓展位
	_return = ISP_UpdateExternAddr(ISPx, externAddr);
	if (_return != OK_0)
	{
		return ERROR_3;
	}
	for (i = 0; i < length; i++)
	{
		//---读取低位数据
		_return = ISP_SEND_CMD(ISPx, 0x20, highAddr, lowAddr, 0x00);
		if (_return != OK_0)
		{
			return ERROR_4;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}

		//---读取高位数据
		_return = ISP_SEND_CMD(ISPx, 0x28, highAddr, lowAddr, 0x00);
		if (_return != 0x00)
		{
			return ERROR_5;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
			if (highAddr == 0x00)
			{
				externAddr++;

				//---更新拓展位
				_return = ISP_UpdateExternAddr(ISPx, externAddr);
				if (_return != 0x00)
				{
					return ERROR_6;
				}
			}
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
UINT8_T ISP_ReadChipFlashLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT32_T addr, UINT16_T length)
{
	return  ISP_ReadChipFlashAddr(ISPx, pVal, (UINT8_T)(addr >> 16), (UINT8_T)(addr >> 8), (UINT8_T)(addr), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipFlashBuffer(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T index, UINT16_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;

	//---判断数据长度是否为偶数
	if ((length & 0x01) != 0x00)
	{
		return ERROR_2;
	}

	//---用移位运算当做除2运算
	length >>= 1;
	for (i = 0; i < length; i++)
	{
		//---更新低位地址的缓存区
		_return = ISP_SEND_CMD(ISPx, 0x40, 0x00, (index + i), *(pVal++));
		if (_return != 0x00)
		{
			return ERROR_3;
		}

		//---更新高位地址的缓存区
		_return = ISP_SEND_CMD(ISPx, 0x48, 0x00, (index + i), *(pVal++));
		if (_return != 0x00)
		{
			return ERROR_4;
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
UINT8_T ISP_UpdateChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr)
{
	UINT8_T _return = ISP_UpdateExternAddr(ISPx, externAddr);
	if (_return == OK_0)
	{
		_return = ISP_SEND_CMD(ISPx, 0x4C, highAddr, lowAddr, 0x00);
		if (_return == OK_0)
		{
			//---写入之后延时等待
			ISPx->msgFuncDelayms(5 + ISPx->msgDelayms);
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
UINT8_T ISP_UpdateChipFlashLongAddr(ISP_HandlerType *ISPx, UINT32_T addr)
{
	return ISP_UpdateChipFlashAddr(ISPx, (UINT8_T)(addr >> 16), (UINT8_T)(addr >> 8), (UINT8_T)(addr));
}