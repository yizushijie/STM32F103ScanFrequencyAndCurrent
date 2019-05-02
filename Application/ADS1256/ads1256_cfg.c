#include "ads1256_cfg.h"

//===全局变量定义
ADS1256_HandlerType  g_ADS1256Device0 = { 0 };
pADS1256_HandlerType pADS1256Device0 = &g_ADS1256Device0;

UINT8_T(*ADS1256_SPI_SEND_CMD)(ADS1256_HandlerType *, UINT8_T, UINT8_T *);

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device0_Init(ADS1256_HandlerType *ADS1256x)
{
	//---DRDY
	ADS1256x->msgDRDY.msgGPIOPort = GPIOA;
	ADS1256x->msgDRDY.msgGPIOBit = LL_GPIO_PIN_3;
	
	//---GPIO时钟使能
	if (ADS1256x->msgDRDY.msgGPIOPort!=NULL)
	{
		GPIOTask_Clock(ADS1256x->msgDRDY.msgGPIOPort, 1);
	}

	//---复位信号
	ADS1256x->msgHWRST.msgGPIOPort = NULL;
	ADS1256x->msgHWRST.msgGPIOBit = LL_GPIO_PIN_0;

	//---GPIO时钟使能
	if (ADS1256x->msgHWRST.msgGPIOPort!=NULL)
	{
		GPIOTask_Clock(ADS1256x->msgHWRST.msgGPIOPort, 1);
	}

	//---GPIO的配置
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO的初始化
	GPIO_InitStruct.Pin = ADS1256x->msgDRDY.msgGPIOBit;			//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;					//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;			//---GPIO的速度
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;						//---上拉使能
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0; //---端口复用模式
#endif

	//---初始化DRDY
	LL_GPIO_Init(ADS1256x->msgDRDY.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_1(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit);

	//---初始化RST
	if (ADS1256x->msgHWRST.msgGPIOPort != NULL)
	{
		GPIO_InitStruct.Pin = ADS1256x->msgHWRST.msgGPIOBit;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		LL_GPIO_Init(ADS1256x->msgHWRST.msgGPIOPort, &GPIO_InitStruct);
		GPIO_OUT_1(ADS1256x->msgHWRST.msgGPIOPort, ADS1256x->msgHWRST.msgGPIOBit);
	}

	//---CS
	ADS1256x->msgSPI.msgCS.msgGPIOPort = GPIOA;
	ADS1256x->msgSPI.msgCS.msgGPIOBit = LL_GPIO_PIN_4;

	//---SCK
	ADS1256x->msgSPI.msgSCK.msgGPIOPort = GPIOA;
	ADS1256x->msgSPI.msgSCK.msgGPIOBit = LL_GPIO_PIN_5;

	//---MISO
	ADS1256x->msgSPI.msgMISO.msgGPIOPort = GPIOA;
	ADS1256x->msgSPI.msgMISO.msgGPIOBit = LL_GPIO_PIN_6;

	//---MOSI
	ADS1256x->msgSPI.msgMOSI.msgGPIOPort = GPIOA;
	ADS1256x->msgSPI.msgMOSI.msgGPIOBit = LL_GPIO_PIN_7;

	//---复用模式
#ifndef USE_MCU_STM32F1

	//---端口复用模式
	ADS1256x->msgSPI.msgGPIOAlternate = LL_GPIO_AF_5;
#endif

	//---SPI序号
	ADS1256x->msgSPI.msgSPIx = SPI1;
#ifndef USE_MCU_STM32F1

	//---SPI的协议
	ADS1256x->msgSPI.msgStandard = LL_SPI_PROTOCOL_MOTOROLA;
#endif

	//---ADS1256数据转换的速率
	ADS1256x->msgDRate = 0xF0;

	//---定义脉冲宽度
	ADS1256x->msgSPI.msgPluseWidth = 0;

	//---时钟空闲为低电平
	ADS1256x->msgSPI.msgCPOL = 0;

	//---数据采样在第一个时钟边沿
	ADS1256x->msgSPI.msgCPOH = 1;

}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device1_Init(ADS1256_HandlerType *ADS1256x)
{

}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device2_Init(ADS1256_HandlerType *ADS1256x)
{

}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_HW_Init(ADS1256_HandlerType *ADS1256x)
{
	//---注销当前的所有配置
	SPITask_DeInit(&(ADS1256x->msgSPI));

	//---硬件端口的配置---硬件实现
	SPITask_MHW_GPIO_Init(&(ADS1256x->msgSPI));

	//---硬件SPI的初始化
	LL_SPI_InitTypeDef SPI_InitStruct = {0};

	//---SPI的模式配置
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;						//---主机模式
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;				//---8位数据

	//---时钟极性的设置
	if (ADS1256x->msgSPI.msgCPOL == 0)
	{
		//---CLK空闲时为低电平 (CLK空闲是只能是低电平)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
		GPIO_OUT_0(ADS1256x->msgSPI.msgSCK.msgGPIOPort, ADS1256x->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		//---CLK空闲时为高电平 (CLK空闲是只能是低电平)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	}

	//---数据采样的时钟边沿位置
	if (ADS1256x->msgSPI.msgCPOL == 0)
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

	//---初始化查询方式的SPI
	SPITask_MHW_PollMode_Init(&(ADS1256x->msgSPI), SPI_InitStruct);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SW_Init(ADS1256_HandlerType *ADS1256x)
{
	SPITask_DeInit(&(ADS1256x->msgSPI));
	
	//---硬件端口的配置---软件实现
	SPITask_MSW_GPIO_Init(&(ADS1256x->msgSPI));
	
	//---时钟线的极性
	if (ADS1256x->msgSPI.msgCPOL == 0)
	{
		GPIO_OUT_0(ADS1256x->msgSPI.msgSCK.msgGPIOPort, ADS1256x->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(ADS1256x->msgSPI.msgSCK.msgGPIOPort, ADS1256x->msgSPI.msgSCK.msgGPIOBit);
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
UINT8_T ADS1256_SPI_Init(ADS1256_HandlerType *ADS1256x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW)
{
	//---使用的DHT11的端口
	if ((ADS1256x != NULL) && (ADS1256x == ADS1256_TASK_ONE))
	{
		ADS1256_SPI_Device0_Init(ADS1256x);
	}
	else if ((ADS1256x != NULL) && (ADS1256x == ADS1256_TASK_TWO))
	{
		ADS1256_SPI_Device1_Init(ADS1256x);
	}
	else if ((ADS1256x != NULL) && (ADS1256x == ADS1256_TASK_THREE))
	{
		ADS1256_SPI_Device2_Init(ADS1256x);
	}
	else
	{
		return ERROR_1;
	}

	//---判断初始化的方式
	if (isHW != 0)
	{
		ADS1256x->msgSPI.msgModelIsHW = 1;
		ADS1256_SPI_HW_Init(ADS1256x);
		ADS1256_SPI_SEND_CMD = ADS1256_SPI_HW_SendCmd;
	}
	else
	{
		ADS1256x->msgSPI.msgModelIsHW = 0;
		ADS1256_SPI_SW_Init(ADS1256x);
		ADS1256_SPI_SEND_CMD = ADS1256_SPI_SW_SendCmd;
	}

	//---注册ms延时时间
	if (pFuncDelayms!=NULL)
	{
		ADS1256x->msgFuncDelayms = pFuncDelayms;
	}
	else
	{
		ADS1256x->msgFuncDelayms = DelayTask_ms;
	}

	//---注册us延时函数
	if (pFuncDelayus!=NULL)
	{
		ADS1256x->msgSPI.msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		ADS1256x->msgSPI.msgFuncDelayus = DelayTask_us;
	}

	//---注册滴答函数
	ADS1256x->msgSPI.msgFuncTimeTick = pFuncTimerTick;

	//---配置默认参数
	return ADS1256_SPI_ConfigInit(ADS1256x);

	//return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待内部操作完成。 自校准时间较长，需要等待
//////输入参数:
//////输出参数: 0---成功；非0---失败
//////说		明：检查准备好操作，检查DRDY端口从高到低的脉冲，有些状态下命令不响应的问题
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WaitDRDY(ADS1256_HandlerType *ADS1256x)
{
	//---获取时间标签
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;

	//---获取当前时间节拍
	if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
	{
		//nowTime = W25QXXx->msgSPI.msgFuncTick();
		oldTime = ADS1256x->msgSPI.msgFuncTimeTick();
	}

	//---解析GPIO是否存在
	if (ADS1256x->msgDRDY.msgGPIOPort == NULL)
	{
		return ERROR_5;
	}

	//---读取操作完成，检查高电平
	while (1)
	{
		if (GPIO_GET_STATE(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit) != 0)
		{
			break;
		}
		if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
		{
			//---当前时间
			nowTime = ADS1256x->msgSPI.msgFuncTimeTick();

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
				return ERROR_1;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 400000)
			{
				//---发送发生超时错误
				return ERROR_2;
			}
		}
	}

	//---读取操作完成，检查低电平
	while (1)
	{
		if (GPIO_GET_STATE(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit) == 0)
		{
			break;
		}
		if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
		{
			//---当前时间
			nowTime = ADS1256x->msgSPI.msgFuncTimeTick();

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
				return ERROR_3;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 400000)
			{
				//---发送发生超时错误
				return ERROR_4;
			}
		}
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
UINT8_T ADS1256_SPI_SW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---数据发送
	return SPITask_MSW_SendByteMSB(&(ADS1256x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_HW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---数据发送
	return SPITask_MHW_PollMode_WriteByte(&(ADS1256x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WriteReg(ADS1256_HandlerType *ADS1256x, UINT8_T regID, UINT8_T regVal)
{
	UINT8_T _return = OK_0;
	//---读取准备信号
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---校验准备信号
	if (_return == OK_0)
	{
		//---使能写操作
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---写寄存器的命令, 并发送寄存器地址
		_return=ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_WREG | regID, NULL);
		_return <<= 1;
		//---寄存器个数 - 1, 此处写1个寄存器
		_return|=ADS1256_SPI_SEND_CMD(ADS1256x, 0x00, NULL);
		_return <<= 1;
		//---发送寄存器值
		_return|=ADS1256_SPI_SEND_CMD(ADS1256x, regVal, NULL);
	}
	//---不使能通讯，放在最外层，避免发生通讯一直使能
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadReg(ADS1256_HandlerType *ADS1256x, UINT8_T regAddr, UINT8_T *pRVal)
{
	UINT8_T _return = OK_0;
	//---读取准备信号
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---校验准备信号
	if (_return == OK_0)
	{
		//---使能写操作
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---写寄存器的命令, 并发送寄存器地址
		_return=ADS1256_SPI_SEND_CMD( ADS1256x, ADS1256_CMD_RREG | ( regAddr & 0x0F ), NULL );
		_return <<= 1;
		//---寄存器个数 - 1, 此处写1个寄存器
		_return|=ADS1256_SPI_SEND_CMD( ADS1256x, 0x00, NULL );
		_return <<= 1;
		//---必须延迟才能读取芯片返回数据
		ADS1256x->msgSPI.msgFuncDelayus( 10 );
		//---读寄存器值
		_return|=ADS1256_SPI_SEND_CMD( ADS1256x, 0xFF, pRVal );
	}
	//---不使能通讯，放在最外层，避免发生通讯一直使能
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);

	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送单字节命令
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WriteCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd)
{
	UINT8_T _return = OK_0;
	//---读取准备信号
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---校验准备信号
	if (_return == OK_0)
	{
		//---使能写操作
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---发送命令
		_return=ADS1256_SPI_SEND_CMD( ADS1256x, cmd, NULL );
	}
	//---不使能通讯，放在最外层，避免发生通讯一直使能
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：硬件复位设备
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_HardReset(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T _return = OK_0, dRate = 0;
	if (ADS1256x->msgHWRST.msgGPIOPort != NULL)
	{
		GPIO_OUT_0( ADS1256x->msgHWRST.msgGPIOPort, ADS1256x->msgHWRST.msgGPIOBit );
		ADS1256x->msgFuncDelayms( 1 );
		GPIO_OUT_1( ADS1256x->msgHWRST.msgGPIOPort, ADS1256x->msgHWRST.msgGPIOBit );
		ADS1256x->msgFuncDelayms( 1 );

		//---读取默认的转换速率，默认值是0xF0
		_return = ADS1256_SPI_ReadDRate( ADS1256x, &dRate );
		//---校验复位是否成功
		if ((_return!=OK_0)||(dRate!=0xF0))
		{
			_return = ERROR_1;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：软件复位设备
//////输入参数:
//////输出参数:
//////说		明：通过读取默认转换速率寄存器的值，判断是否未复位成功；复位之后，默认值是0xF0
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SoftReset(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T _return = OK_0, dRate = 0;
	//---发送软件复位命令
	_return=ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_REST);
	//---校验复位是否成功
	if (_return==OK_0)
	{
		//---读取默认的转换速率，默认值是0xF0
		_return = ADS1256_SPI_ReadDRate( ADS1256x, &dRate );
		if ((_return!=OK_0)||(dRate!=0xF0))
		{
			_return = ERROR_1;
		}
	}
	ADS1256x->msgDRate = 0xF0;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：进入休眠模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_Standby(ADS1256_HandlerType *ADS1256x)
{
	//---发送休眠模式命令
	ADS1256x->msgSleepMode = 1;
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_STANDBY);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：唤醒
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WAKEUP(ADS1256_HandlerType *ADS1256x)
{
	//---发送唤醒命令
	ADS1256x->msgSleepMode = 0;
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_WAKEUP);;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：停止连续数据命令
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SDATAC(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_SDATAC);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取设备的ID信息
//////输入参数:
//////输出参数:
//////说		明：ADS1256的ID是3
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadChipID(ADS1256_HandlerType *ADS1256x,UINT8_T *pDeviceID)
{
	UINT8_T _return = OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, pDeviceID);
	//---右移4位
	*pDeviceID >>= 4;
	//---判断设备的ID是否符合
	if (*pDeviceID!=0x03)
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：不使能自动校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_DisableAutoCalibration(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T statusReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return == OK_0)
	{
		statusReg &= 0xFB;
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：使能自动校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_EnableAutoCalibration(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T statusReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return == OK_0)
	{
		statusReg |= 0x04;
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
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
UINT8_T ADS1256_SPI_DisableBuffer(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T statusReg = 0,_return=OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return==OK_0)
	{
		statusReg &= 0xFD;
		_return=ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
		ADS1256x->msgBufferON = 0;
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
UINT8_T ADS1256_SPI_EnableBuffer( ADS1256_HandlerType *ADS1256x )
{
	UINT8_T statusReg = 0,_return=OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return==OK_0)
	{
		statusReg |= 0x02;
		_return=ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
		ADS1256x->msgBufferON = 1;
	}
	return _return;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：配置通道号。多路复用。AIN- 固定接地（ACOM)；单端输入
//////输入参数: ch : 通道号， 0-7
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetSingleChannal(ADS1256_HandlerType *ADS1256x, UINT8_T ch)
{
	if (ch > 7)
	{
		return ERROR_1;
	}
	//---当前设置的通道
	ADS1256x->msgNowChannel = ch;
	ADS1256x->msgChannelMode[ch] = 0;
	//---Bit3 = 1, AINN 固定接 AINCOM
	return ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (ch << 4) | (1 << 3));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：差分输入
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetDifferenceChannal(ADS1256_HandlerType *ADS1256x, UINT8_T ch)
{
	UINT8_T _return = OK_0;

	if ((ch == 0)||(ch==4))
	{
		if (ch==4)
		{
			ch -= 4;
		}
		//---差分输入 AIN0， AIN1
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (0 << 4) | 1);	/* 差分输入 AIN0， AIN1 */
	}
	else if ((ch == 1)||(ch==5))
	{
		if (ch==5)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---差分输入 AIN2， AIN3
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (2 << 4) | 3);	/* 差分输入 AIN2， AIN3 */
	}
	else if ((ch == 2)||(ch==6))
	{
		if (ch == 6)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---差分输入 AIN4， AIN5
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (4 << 4) | 5);	/* 差分输入 AIN4， AIN5 */
	}
	else if ((ch == 3)||(ch==7))
	{
		if (ch == 7)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---差分输入 AIN6， AIN7
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (6 << 4) | 7);	/* 差分输入 AIN6， AIN7 */
	}
	else
	{
		return ERROR_2;
	}
	//ADS1256x->msgNowChannel = ch;
	ADS1256x->msgChannelMode[ch] = 1;
	ADS1256x->msgChannelMode[ch + 1] = 1;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：配置通道的模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetChannalMode(ADS1256_HandlerType *ADS1256x, UINT8_T ch, UINT8_T isDiff)
{
	if (isDiff)
	{
		return ADS1256_SPI_SetDifferenceChannal(ADS1256x, ch);
	}
	else
	{
		return ADS1256_SPI_SetSingleChannal(ADS1256x, ch);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置时钟输出的速率
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetClockOutRate(ADS1256_HandlerType *ADS1256x, UINT8_T clockRate)
{
	UINT8_T adconReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_ADCON_ADDR, &adconReg);
	if (_return == OK_0)
	{
		adconReg &= 0x1F;
		adconReg |= clockRate;
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_ADCON_ADDR, adconReg);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：传感器检测电流
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SensorDetect(ADS1256_HandlerType *ADS1256x, UINT8_T sensorDetect)
{
	UINT8_T adconReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_ADCON_ADDR, &adconReg);
	if (_return == OK_0)
	{
		adconReg &= 0x67;
		adconReg |= sensorDetect;
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_ADCON_ADDR, adconReg);
	}
	return _return;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取增益
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadGain(ADS1256_HandlerType *ADS1256x, UINT8_T *pGain)
{
	UINT8_T _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_ADCON_ADDR, pGain);
	*pGain &= 0x78;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置增益
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetGain(ADS1256_HandlerType *ADS1256x, UINT8_T gain)
{
	UINT8_T adconReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_ADCON_ADDR, &adconReg);
	if (_return == OK_0)
	{
		adconReg &= 0x78;
		adconReg |= gain;
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_ADCON_ADDR, adconReg);
	}
	return _return;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取AD数据转换速率
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadDRate(ADS1256_HandlerType *ADS1256x, UINT8_T *pDRate)
{
	return ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_DRATE_ADDR, pDRate);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置AD数据转换速率
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetDRate(ADS1256_HandlerType *ADS1256x, UINT8_T rate)
{
	ADS1256x->msgDRate = rate;
	return ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_DRATE_ADDR, rate);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置GPIO输出方式，输入模式还是输出模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetGPIOMode(ADS1256_HandlerType *ADS1256x, UINT8_T gpioMode)
{
	UINT8_T gpioReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_GPIO_ADDR, &gpioReg);
	if (_return == OK_0)
	{
		switch (gpioMode)
		{
			case ADS1256_GPIO3_MODE_OUTPUT:
				gpioReg &= ADS1256_GPIO3_MODE_OUTPUT;
				break;
			case ADS1256_GPIO3_MODE_INPUT:
				gpioReg |= ADS1256_GPIO3_MODE_INPUT;
				break;
			case ADS1256_GPIO2_MODE_OUTPUT:
				gpioReg &= ADS1256_GPIO2_MODE_OUTPUT;
				break;
			case ADS1256_GPIO2_MODE_INPUT:
				gpioReg |= ADS1256_GPIO2_MODE_INPUT;
				break;
			case ADS1256_GPIO1_MODE_OUTPUT:
				gpioReg &= ADS1256_GPIO1_MODE_OUTPUT;
				break;
			case ADS1256_GPIO1_MODE_INPUT:
				gpioReg |= ADS1256_GPIO1_MODE_INPUT;
				break;
			case ADS1256_GPIO0_MODE_OUTPUT:
				gpioReg &= ADS1256_GPIO0_MODE_OUTPUT;
				break;
			case ADS1256_GPIO0_MODE_INPUT:
				gpioReg |= ADS1256_GPIO0_MODE_INPUT;
				break;
			default:
				gpioReg &= 0xEF;
				break;
		}
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_GPIO_ADDR, gpioReg);
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
UINT8_T ADS1256_SPI_ReadGPIOInputState(ADS1256_HandlerType *ADS1256x, UINT8_T gpioIndex,UINT8_T *pGPIOState)
{
	UINT8_T gpioReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_GPIO_ADDR, &gpioReg);
	if (_return == OK_0)
	{
		switch (gpioIndex)
		{
			case 0x00:
				gpioReg &= 0x01;
				break;
			case 0x01:
				gpioReg &= 0x02;
				break;
			case 0x02:
				gpioReg &= 0x04;
				break;
			case 0x03:
                gpioReg &= 0x08;
				break;
			default:
				break;
		}
		*pGPIOState = gpioReg;
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
UINT8_T ADS1256_SPI_SetGPIOOutState(ADS1256_HandlerType *ADS1256x, UINT8_T gpioIndex,UINT8_T isHighLevel)
{
	UINT8_T gpioReg = 0, _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_GPIO_ADDR, &gpioReg);
	if (_return == OK_0)
	{
		switch (gpioIndex)
		{
			case 0x00:
				if (isHighLevel)
				{
					gpioReg |= 0x01;
				}
				else
				{
					gpioReg &= 0xFE;
				}
				break;
			case 0x01:
				if (isHighLevel)
				{
					gpioReg |= 0x02;
				}
				else
				{
					gpioReg &= 0xFD;
				}
				break;
			case 0x02:
				if (isHighLevel)
				{
					gpioReg |= 0x04;
				}
				else
				{
					gpioReg &= 0xFB;
				}
				break;
			case 0x03:
				if (isHighLevel)
				{
					gpioReg |= 0x08;
				}
				else
				{
					gpioReg &= 0xF7;
				}
				break;
			default:
				gpioReg &= 0xF0;
				break;
		}
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_GPIO_ADDR, gpioReg);
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
UINT8_T ADS1256_SPI_DisabledAutoCalibration(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T statusReg = 0,_return=OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return == OK_0)
	{
		statusReg &= 0xFB;
		_return= ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
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
UINT8_T ADS1256_SPI_EnabledAutoCalibration(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T statusReg = 0,_return=OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, &statusReg);
	if (_return == OK_0)
	{
		statusReg |= 0x04;
		_return=ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_STATUS_ADDR, statusReg);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_Calibration(ADS1256_HandlerType *ADS1256x, UINT8_T calibCmdReg)
{
	//---发送校准命令
	ADS1256_SPI_WriteCmd(ADS1256x, calibCmdReg);
	//---等待校准完成
	return ADS1256_SPI_WaitDRDY(ADS1256x);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：偏移和增益自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：偏移自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_OffsetSelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFOCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：增益自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_GainSelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFGCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：系统偏移自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SysOffsetCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SYSOCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：系统增益自校准
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SysGainCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SYSGCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待ADC装换结果完成
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WaitResultOK(ADS1256_HandlerType* ADS1256x,UINT8_T dRate)
{
	switch (dRate)
	{
		case ADS1256_DRATE_15000SPS:
			ADS1256x->msgSPI.msgFuncDelayus(67);
			break;
		case ADS1256_DRATE_7500SPS :
			ADS1256x->msgSPI.msgFuncDelayus(134);
			break;
		case ADS1256_DRATE_3750SPS :
			ADS1256x->msgSPI.msgFuncDelayus(268);
			break;
		case ADS1256_DRATE_2000SPS :
			ADS1256x->msgSPI.msgFuncDelayus(500);
			break;
		case ADS1256_DRATE_1000SPS :
			ADS1256x->msgFuncDelayms(1);
			break;
		case ADS1256_DRATE_500SPS  :
			ADS1256x->msgFuncDelayms(2);
			break;
		case ADS1256_DRATE_100SPS  :
			ADS1256x->msgFuncDelayms(10);
			break;
		case ADS1256_DRATE_60SPS   :
			ADS1256x->msgFuncDelayms(17);
			break;
		case ADS1256_DRATE_50SPS   :
			ADS1256x->msgFuncDelayms(20);
			break;
		case ADS1256_DRATE_30SPS   :
			ADS1256x->msgFuncDelayms(34);
			break;
		case ADS1256_DRATE_25SPS   :
			ADS1256x->msgFuncDelayms(40);
			break;
		case ADS1256_DRATE_15SPS   :
			ADS1256x->msgFuncDelayms(67);
			break;
		case ADS1256_DRATE_10SPS   :
			ADS1256x->msgFuncDelayms(100);
			break;
		case ADS1256_DRATE_5SPS	   :
			ADS1256x->msgFuncDelayms(200);
			break;
		case ADS1256_DRATE_2P5SPS  :
			ADS1256x->msgFuncDelayms(400);
			break;
		case ADS1256_DRATE_30000SPS:
			ADS1256x->msgSPI.msgFuncDelayus(34);
		default:
			break;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取ADC通道转换的结果
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadChannelResult(ADS1256_HandlerType *ADS1256x, UINT8_T ch)
{
	UINT8_T _return = OK_0;
	UINT8_T temp[3] = { 0 };

	//---当ADS1256_DRDY为低时才能写寄存器
	if (ch < 7)
	{
		if (ADS1256x->msgChannelMode[ch] == 0)
		{
			//---设置单端输入通道
			ADS1256_SPI_SetSingleChannal(ADS1256x, ch);
		}
		else
		{
			//---设置单端输入通道
			ADS1256_SPI_SetDifferenceChannal(ADS1256x, ch);
		}
		
		//---延时等待转换完成,速率慢转换时间长，避免发生超时错误
		if (ADS1256x->msgDRate< ADS1256_DRATE_15SPS)
		{
			ADS1256x->msgFuncDelayms(0x100- ADS1256x->msgDRate);
		}
		
		//---检查读操作是否准备完成
		_return=ADS1256_SPI_WaitDRDY(ADS1256x);

		//---使能通讯
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		
		//---判断是不是出于休眠模式
		if (ADS1256x->msgSleepMode!=0)
		{
			//---发送命令
			ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_SYNC, NULL);

			//---必须延迟才能读取芯片返回数据
			if (ADS1256x->msgSPI.msgFuncDelayus != NULL)
			{
				ADS1256x->msgSPI.msgFuncDelayus(5);
			}
			ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_WAKEUP, NULL);
		}
		
		//---必须延迟才能读取芯片返回数据
		ADS1256x->msgSPI.msgFuncDelayus(5);

		ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_RDATA, NULL);

		//---必须延迟才能读取芯片返回数据
		ADS1256x->msgSPI.msgFuncDelayus(25);

		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[0]);
		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[1]);
		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[2]);

		//---不使能通讯
		GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] = temp[0];
		ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] = (ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] << 8) + temp[1];
		ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] = (ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] << 8) + temp[2];

		//---if MSB=1,
		if (ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] > 0x7FFFFF)
		{
			//---do 2's complement
			ADS1256x->msgChannelResult[ADS1256x->msgOldChannel] -= 0x1000000;
		}

		//---更新下一次设置通道时候，读取的ADC结果
		ADS1256x->msgOldChannel = ADS1256x->msgNowChannel;
	}
	return _return;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验设备，避免设备发生异常，用于初始化确认
//////输入参数:
//////输出参数: 0---设备初始化正常；非0---设备初始化异常
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_CheckDevice(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T tempDRate = 0;
	UINT8_T _return = OK_0;
	//---读取数据转换速率
	_return= ADS1256_SPI_ReadDRate(ADS1256x, &tempDRate);
	if (_return!=OK_0)
	{
		_return = ERROR_1;
	}
	else
	{
		if (ADS1256x->msgDRate!=tempDRate)
		{
			_return = ERROR_2;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：ADS1256的配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ConfigInit(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T _return = OK_0;
	_return=ADS1256_SPI_SoftReset(ADS1256x);
	//---判断是否复位完成
	if (_return!=OK_0)
	{
		_return = ERROR_1;
		goto GoToExit;
	}
	//---设置时钟
	_return = ADS1256_SPI_SetDRate(ADS1256x, ADS1256_DRATE_2000SPS);
	//---校验时钟的设置
	_return = ADS1256_SPI_CheckDevice(ADS1256x);
	//---判断时钟是否设置正确
	if (_return!=OK_0)
	{
		_return = ERROR_2;
		goto GoToExit;
	}
	//---使能自动校准
	_return= ADS1256_SPI_EnabledAutoCalibration(ADS1256x);
	if (_return!=OK_0)
	{
		_return = ERROR_3;
		goto GoToExit;
	}
	//---设置增益
	_return = ADS1256_SPI_SetGain(ADS1256x, ADS1256_ADCON_GAIN_1);
	if (_return!=OK_0)
	{
		_return = ERROR_4;
		goto GoToExit;
	}
	//---使能自校准
	_return = ADS1256_SPI_SelfCalibration(ADS1256x);
	if (_return!=OK_0)
	{
		_return = ERROR_5;
	}
	GoToExit:
	return _return; 
}
