#include "usart_cfg.h"

//===全局变量定义
USART_HandlerType  g_USART1 = { 0 };
pUSART_HandlerType pUSART1 = &g_USART1;

#ifdef USE_USART_PRINTF

//===printf函数使用的缓存区
char printfBuffer[USART_PRINTF_SIZE] = { 0 };
#endif

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_StructInit(USART_HandlerType*  USARTx)
{
	//---串口的数据处理流程
	USARTx->msgIndex = 0;
	USARTx->msgRxID = 0;
	USARTx->msgTxID = 0;
	USARTx->msgDeviceID = 0;
	USARTx->msgIDIndex = 0;
	USARTx->msgCmdIndex = 0;
	USARTx->msgDataOneIndex = 0;
	USARTx->msgDataTwoIndex = 0;
	USARTx->msgTxBit = 0;
	USARTx->msgTxPort = NULL;
	USARTx->msg485Bit = 0;
	USARTx->msg485Port = NULL;
	USARTx->msgUSART = NULL;

	//---接收缓存区
	USARTx->msgRxHandler.msgCheckSum = 0;
	USARTx->msgRxHandler.msgCRCFlag = 0;
	USARTx->msgRxHandler.msgStep = 0;
	USARTx->msgRxHandler.msgAddNewLine = 0;
	USARTx->msgRxHandler.msgIndexW = 0;
	USARTx->msgRxHandler.msgIndexR = 0;
	USARTx->msgRxHandler.msgCount = 0;
	USARTx->msgRxHandler.msgSize = 0;
	USARTx->msgRxHandler.msgTaskState = USART_BUSY;
	USARTx->msgRxHandler.msgOverFlow = 0;
	USARTx->msgRxHandler.msgNowTime = 0;
	USARTx->msgRxHandler.msgEndTime = 0;
	USARTx->msgRxHandler.msgMaxTime = 0;
	USARTx->msgRxHandler.pMsgVal = NULL;

	//---发送缓存区
	USARTx->msgTxHandler.msgCheckSum = 0;
	USARTx->msgTxHandler.msgCRCFlag = 0;
	USARTx->msgTxHandler.msgStep = 0;
	USARTx->msgTxHandler.msgAddNewLine = 0;
	USARTx->msgTxHandler.msgIndexW = 0;
	USARTx->msgTxHandler.msgIndexR = 0;
	USARTx->msgTxHandler.msgCount = 0;
	USARTx->msgTxHandler.msgSize = 0;
	USARTx->msgTxHandler.msgTaskState = USART_OK;
	USARTx->msgTxHandler.msgOverFlow = 0;
	USARTx->msgTxHandler.msgNowTime = 0;
	USARTx->msgTxHandler.msgEndTime = 0;
	USARTx->msgTxHandler.msgMaxTime = 0;
	USARTx->msgTxHandler.pMsgVal = NULL;

	//---计数器
	USARTx->msgFuncTimeTick = NULL;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Init(USART_HandlerType*  USARTx, UINT16_T rxSize, UINT8_T* pRxVal, UINT8_T rxCRCFlag, UINT16_T txSize, UINT8_T* pTxVal, UINT8_T txCRCFlag, UINT32_T(*pTimerTick)(void))
{
	USART_StructInit(USARTx);

	USARTx->msgRxHandler.msgCRCFlag = rxCRCFlag;
	USARTx->msgRxHandler.msgSize = rxSize;
	USARTx->msgRxHandler.pMsgVal = pRxVal;

	USARTx->msgTxHandler.msgCRCFlag = txCRCFlag;
	USARTx->msgTxHandler.msgSize = txSize;
	USARTx->msgTxHandler.pMsgVal = pTxVal;

	//---注册计数函数
	USARTx->msgFuncTimeTick = pTimerTick;

	//---端口初始化
	if ((USARTx != NULL) && (USARTx == USART_TASK_ONE))
	{
		USART1_Init(USARTx);
	}
	else if ((USARTx != NULL) && (USARTx == USART_TASK_TWO))
	{
		USART2_Init(USARTx);
	}
	else if ((USARTx != NULL) && (USARTx == USART_TASK_THREE))
	{
		USART3_Init(USARTx);
	}
	else
	{
		return ERROR_2;
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
UINT8_T  USART_GPIOInit(USART_HandlerType*  USARTx, UINT8_T isInput)
{
#ifdef USART_INIT_GPIO
	if (USARTx->msgTxPort != NULL)
	{
		if (isInput == USART_TXGPIO_SET_OUTPUT)
		{
			LL_GPIO_SetPinMode(USARTx->msgTxPort, USARTx->msgTxBit, LL_GPIO_MODE_ALTERNATE);
		}
		else
		{
			LL_GPIO_SetPinMode(USARTx->msgTxPort, USARTx->msgTxBit, LL_GPIO_MODE_INPUT);
		}
	}
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_485GPIOInit(USART_HandlerType*  USARTx, UINT8_T isEnable)
{
	if (USARTx->msg485Port != NULL)
	{
		if (isEnable == USART_485_TX_ENABLE)
		{
			GPIO_OUT_0(USARTx->msg485Port, USARTx->msg485Bit);
		}
		else
		{
			GPIO_OUT_1(USARTx->msg485Port, USARTx->msg485Bit);
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
UINT8_T USART_EnableCRC_RX(USART_HandlerType*USARTx, UINT8_T crcFlag)
{
	USARTx->msgRxHandler.msgCRCFlag = crcFlag;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetCRC_RX(USART_HandlerType*USARTx)
{
	return  USARTx->msgRxHandler.msgCRCFlag;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_EnableCRC_TX(USART_HandlerType*USARTx, UINT8_T crcFlag)
{
	USARTx->msgTxHandler.msgCRCFlag = crcFlag;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetCRC_TX(USART_HandlerType*USARTx)
{
	return  USARTx->msgTxHandler.msgCRCFlag;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_Init(USART_HandlerType*USARTx, UINT8_T isRx)
{
	UINT32_T temp = 0;

	//---获取当前时间
	if (USARTx->msgFuncTimeTick != NULL)
	{
		temp = USARTx->msgFuncTimeTick();
	}

	//---时间节点
	if (isRx)
	{
		USARTx->msgRxHandler.msgNowTime = temp;

		//---同步时钟
		USARTx->msgRxHandler.msgEndTime = USARTx->msgRxHandler.msgNowTime;
	}
	else
	{
		USARTx->msgTxHandler.msgNowTime = temp;

		//---同步时钟
		USARTx->msgTxHandler.msgEndTime = USARTx->msgTxHandler.msgNowTime;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：注销超时计数器
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_DeInit(USART_HandlerType*USARTx, UINT8_T isRx)
{
	if (isRx)
	{
		//---清零溢出标志位
		USARTx->msgRxHandler.msgOverFlow = 0;

		//---清零当前时钟
		USARTx->msgRxHandler.msgEndTime = 0;

		//---同步时钟
		USARTx->msgRxHandler.msgNowTime = USARTx->msgRxHandler.msgEndTime;
	}
	else
	{
		//---清零溢出标志位
		USARTx->msgTxHandler.msgOverFlow = 0;

		//---清零当前时钟
		USARTx->msgTxHandler.msgEndTime = 0;

		//---同步时钟
		USARTx->msgTxHandler.msgNowTime = USARTx->msgTxHandler.msgEndTime;
	}
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 时间计数是否发生超时
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_OVF(USART_HandlerType*USARTx, UINT32_T timeOut, UINT8_T isRx)
{
	UINT32_T temp = 0;

	//---获取当前时间
	if (USARTx->msgFuncTimeTick != NULL)
	{
		temp = USARTx->msgFuncTimeTick() + 2;
	}

	//---时间节点
	if (isRx)
	{
		//---获取当前时间
		USARTx->msgRxHandler.msgEndTime = temp;

		//---判断是否发生超时错误
		if (USARTx->msgRxHandler.msgEndTime > USARTx->msgRxHandler.msgNowTime)
		{
			//---计时器未发生溢出操作
			if ((USARTx->msgRxHandler.msgEndTime - USARTx->msgRxHandler.msgNowTime) > timeOut)
			{
				USARTx->msgRxHandler.msgOverFlow = 1;
				return ERROR_1;
			}
		}
		else if (USARTx->msgRxHandler.msgEndTime < USARTx->msgRxHandler.msgNowTime)
		{
			//---计时器发生溢出操作
			if ((0xFFFFFFFF - USARTx->msgRxHandler.msgNowTime + USARTx->msgRxHandler.msgEndTime) > timeOut)
			{
				USARTx->msgRxHandler.msgOverFlow = 1;
				return ERROR_1;
			}
		}
	}
	else
	{
		//---获取当前时间
		USARTx->msgTxHandler.msgEndTime = temp;

		//---判断是否发生超时错误
		if (USARTx->msgTxHandler.msgEndTime > USARTx->msgTxHandler.msgNowTime)
		{
			//---计时器未发生溢出操作
			if ((USARTx->msgTxHandler.msgEndTime - USARTx->msgTxHandler.msgNowTime) > timeOut)
			{
				USARTx->msgTxHandler.msgOverFlow = 1;
				return ERROR_1;
			}
		}
		else if (USARTx->msgTxHandler.msgEndTime < USARTx->msgTxHandler.msgNowTime)
		{
			//---计时器发生溢出操作
			if ((0xFFFFFFFF - USARTx->msgTxHandler.msgNowTime + USARTx->msgTxHandler.msgEndTime) > timeOut)
			{
				USARTx->msgTxHandler.msgOverFlow = 1;
				return ERROR_1;
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
UINT8_T  USART_TimeOVFTask(USART_HandlerType*USARTx, UINT8_T isRx)
{
	UINT8_T _return = OK_0;

	//---判断是接收还是发送
	if (isRx != 0)
	{
		if ((USARTx->msgRxHandler.msgNowTime != 0) && (USARTx->msgRxHandler.msgMaxTime != 0))
		{
			if (USARTx->msgRxHandler.msgNowTime == 0)
			{
				return OK_0;
			}

			//---获取超时值
			_return = USART_TimeTick_OVF(USARTx, USARTx->msgRxHandler.msgMaxTime, 1);

			//---超时条件判断
			//if ((_return != OK_0) && (USARTx->msgRxHandler.msgNowTime != 0))
			if ((_return != OK_0))
			{
				//---打印超时的串口信息
				USART_Printf(USARTx, "=>>接收串口%d发生超时错误!<<=\r\n", (USARTx->msgIndex - 1));

				//---复位接收数据缓存区
				USART_ReadInit(USARTx);
			}
		}
	}
	else
	{
		if ((USARTx->msgTxHandler.msgNowTime != 0) && (USARTx->msgTxHandler.msgMaxTime != 0))
		{
			if (USARTx->msgTxHandler.msgNowTime == 0)
			{
				return OK_0;
			}

			//---获取超时值
			_return = USART_TimeTick_OVF(USARTx, USARTx->msgTxHandler.msgMaxTime, 0);

			//---超时条件判断，发现某些状态下当前记录的时间值在改写为零的时候，时间记录点没有发生同步更新
			if ((_return != OK_0) && (USARTx->msgTxHandler.msgNowTime != 0))
			{
				//---打印超时的串口信息
				USART_Printf(USARTx, "=>>发送串口%d发生超时错误!<<=\r\n", (USARTx->msgIndex - 1));

				//---复位发送数据缓存区
				USART_WriteInit(USARTx);
			}
			else
			{
				USARTx->msgTxHandler.msgOverFlow = 0;
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
//////说		明： 接受到数据最大长度使用1字节表示
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITRead_8BitsTask(USART_HandlerType*USARTx, UINT8_T val)
{
	//---保存数到的数据
	static UINT8_T tempVal;
	tempVal = val;

	//---数据接收步序
	switch (USARTx->msgRxHandler.msgStep)
	{
		//---接收数据的报头
		case 0:
			if (USARTx->msgRxHandler.msgIndexR == 0)
			{
				if (USARTx->msgRxID == tempVal)
				{
					USARTx->msgRxHandler.pMsgVal[0] = tempVal;
					USARTx->msgRxHandler.msgStep = 1;

					//---记录数据的个数
					USARTx->msgRxHandler.msgCount = 1;

					//---收到第一个符合格式的数据，启动超时计数器
					USART_TimeTick_Init(USARTx, 1);

					//---清零超时标志
					USART_ClearOVF(USARTx, 1);
				}
			}
			break;

			//---接收数据的长度
		case 1:
			if ((tempVal > 0) && (tempVal < (UINT8_T)(USARTx->msgRxHandler.msgSize & 0xFF)))
			{
				USARTx->msgRxHandler.pMsgVal[1] = tempVal;

				//---获取数据的接收长度
				USARTx->msgRxHandler.msgIndexW = tempVal;

				//---记录数据的个数
				USARTx->msgRxHandler.msgCount += 1;

				//---下一步骤
				USARTx->msgRxHandler.msgStep = 2;
			}
			else
			{
				USARTx->msgRxHandler.msgStep = 0;
			}

			//---复位超时计数器
			USART_TimeTick_Init(USARTx, 1);
			break;

			//---接收数据信息
		case 2:
			USARTx->msgRxHandler.pMsgVal[USARTx->msgRxHandler.msgIndexR + 2] = tempVal;
			USARTx->msgRxHandler.msgIndexR++;

			//---记录数据的个数
			USARTx->msgRxHandler.msgCount += 1;

			//---判断是否溢出
			if (USARTx->msgRxHandler.msgIndexR >= USARTx->msgRxHandler.msgSize)
			{
				USARTx->msgRxHandler.msgIndexR = 0;
			}

			//---复位超时计数器
			USART_TimeTick_Init(USARTx, 1);

			//---判断是否接收完成
			if (USARTx->msgRxHandler.msgIndexR >= USARTx->msgRxHandler.msgIndexW)
			{
				USARTx->msgRxHandler.msgStep = 3;
				USARTx->msgRxHandler.msgTaskState = USART_OK;

				//---清零超时计数器
				//USART_TimeTick_DeInit(USARTx, 1);
			}
			break;
		case 3:

			//---清零超时计数器
			//USART_TimeTick_DeInit(USARTx,1);
			break;
		default:

			//USARTx->msgRxHandler.msgNowTime = 0;
			break;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 接受到数据最大长度使用2字节表示
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITRead_16BitsTask(USART_HandlerType*USARTx, UINT8_T val)
{
	//---保存数到的数据
	static UINT8_T tempVal;
	tempVal = val;

	//---数据接收步序
	switch (USARTx->msgRxHandler.msgStep)
	{
		//---接收数据的报头
		case 0:
			if (USARTx->msgRxHandler.msgIndexR == 0)
			{
				if (USARTx->msgRxID == tempVal)
				{
					USARTx->msgRxHandler.pMsgVal[0] = tempVal;
					USARTx->msgRxHandler.msgStep = 1;

					//---记录数据的个数
					USARTx->msgRxHandler.msgCount = 1;

					//---收到第一个符合格式的数据，启动超时计数器
					USART_TimeTick_Init(USARTx, 1);

					//---清零超时标志
					USART_ClearOVF(USARTx, 1);
				}
			}
			break;

			//---接收数据的长度高位
		case 1:
			USARTx->msgRxHandler.pMsgVal[1] = tempVal;
			USARTx->msgRxHandler.msgIndexW = tempVal;
			USARTx->msgRxHandler.msgIndexW <<= 8;
			USARTx->msgRxHandler.msgStep = 2;

			//---记录数据的个数
			USARTx->msgRxHandler.msgCount += 1;

			//---复位超时计数器
			USART_TimeTick_Init(USARTx, 1);
			break;

			//---接收数据的长度低位
		case 2:
			USARTx->msgRxHandler.pMsgVal[2] = tempVal;
			USARTx->msgRxHandler.msgIndexW += tempVal;

			//---记录数据的个数
			USARTx->msgRxHandler.msgCount += 1;

			//---判断数据是否合法
			if ((USARTx->msgRxHandler.msgIndexW > 0) && (USARTx->msgRxHandler.msgIndexW < USARTx->msgRxHandler.msgSize))
			{
				//---记录数据的个数
				USARTx->msgRxHandler.msgCount = USARTx->msgRxHandler.msgIndexW;

				//---下一步骤
				USARTx->msgRxHandler.msgStep = 3;
			}
			else
			{
				USARTx->msgRxHandler.msgStep = 0;
			}

			//---复位超时计数器
			USART_TimeTick_Init(USARTx, 1);
			break;

			//---接收数据信息
		case 3:
			USARTx->msgRxHandler.pMsgVal[USARTx->msgRxHandler.msgIndexR + 3] = tempVal;
			USARTx->msgRxHandler.msgIndexR++;

			//---记录数据的个数
			USARTx->msgRxHandler.msgCount += 1;

			//---判断是否溢出
			if (USARTx->msgRxHandler.msgIndexR >= USARTx->msgRxHandler.msgSize)
			{
				USARTx->msgRxHandler.msgIndexR = 0;
			}

			//---复位超时计数器
			USART_TimeTick_Init(USARTx, 1);

			//---判断是否接收完成
			if (USARTx->msgRxHandler.msgIndexR >= USARTx->msgRxHandler.msgIndexW)
			{
				USARTx->msgRxHandler.msgStep = 4;
				USARTx->msgRxHandler.msgTaskState = USART_OK;

				//---清零超时计数器
				//USART_TimeTick_DeInit(USARTx, 1);
			}
			break;
		case 4:

			//---清零超时计数器
			//USART_TimeTick_DeInit(USARTx,1);
			break;
		default:

			//USARTx->msgRxHandler.msgNowTime = 0;
			break;
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
UINT8_T  USART_ITRead_Task(USART_HandlerType*USARTx, UINT8_T val)
{
	if (USARTx->msgRxHandler.msgSize < 0xFF)
	{
		return USART_ITRead_8BitsTask(USARTx, val);
	}
	else
	{
		return USART_ITRead_16BitsTask(USARTx, val);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_WriteByte(USART_HandlerType*USARTx, UINT8_T  val)
{
	//---发送数据
	LL_USART_TransmitData8(USARTx->msgUSART, val);

	//---等待发送完成
	while (LL_USART_IsActiveFlag_TC(USARTx->msgUSART) == RESET)
	{
		WDT_RESET();
	}

	//---清除发送完成标志位
	LL_USART_ClearFlag_TC(USARTx->msgUSART);
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 发送字符串
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_WriteData(USART_HandlerType*USARTx, char *pVal)
{
	//---设置485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);

	//---切换发送端口为输出模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);

	//---关闭中断
	CLI();
	while (*pVal != '\0')
	{
		USART_PollMode_WriteByte(USARTx, (UINT8_T)*pVal);
		pVal++;
	}

	//---使能中断
	SEI();

	//---设置485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);

	//---切换发送端口为输入模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_ReadByte(USART_HandlerType*USARTx)
{
	//---等待接收完成
	while (LL_USART_IsActiveFlag_RXNE(USARTx->msgUSART) != RESET)
	{
		WDT_RESET();
	}
	return LL_USART_ReceiveData8(USARTx->msgUSART);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_ReadData(USART_HandlerType*USARTx, char *pVal)
{
	while (*pVal != '\0')
	{
		*pVal = USART_PollMode_ReadByte(USARTx);
		pVal++;
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
UINT8_T  USART_ITWrite_TXETask(USART_HandlerType*USARTx)
{
	if (USARTx->msgTxHandler.msgIndexW != 0)
	{
		if (USARTx->msgTxHandler.msgIndexR != USARTx->msgTxHandler.msgIndexW)
		{
			//---发送8Bit的数据
			LL_USART_TransmitData8(USARTx->msgUSART, USARTx->msgTxHandler.pMsgVal[USARTx->msgTxHandler.msgIndexR]);

			//---数据缓存区序号增加
			USARTx->msgTxHandler.msgIndexR++;

			//---校验缓存区是否溢出
			if (USARTx->msgTxHandler.msgIndexR >= USARTx->msgTxHandler.msgSize)
			{
				USARTx->msgTxHandler.msgIndexR = 0;
			}

			//---校验数据是否都填入缓存区
			//if (USARTx->msgTxHandler.msgIndexR >= USARTx->msgTxHandler.msgIndexW)
			if ((USARTx->msgTxHandler.msgIndexR >= USARTx->msgTxHandler.msgIndexW) && (USARTx->msgTxHandler.msgCount > 0) && (USARTx->msgTxHandler.msgIndexR >= USARTx->msgTxHandler.msgCount))
			{
				//---发送完成,发送数据寄存器空中断不使能
				LL_USART_DisableIT_TXE(USARTx->msgUSART);

				//---使能发送完成中断，用于切换TXD端口为输入状态
				LL_USART_EnableIT_TC(USARTx->msgUSART);
			}
		}
		else
		{
			//---发送完成,发送数据寄存器空中断不使能---检查是否存在异常；关闭发送数据寄存器空中断
			LL_USART_DisableIT_TXE(USARTx->msgUSART);
		}
	}
	else
	{
		//---发送完成,发送数据寄存器空中断不使能---检查是否存在异常；关闭发送数据寄存器空中断
		LL_USART_DisableIT_TXE(USARTx->msgUSART);

		//---清空数据发送缓存区
		USART_WriteInit(USARTx);
	}

	//---复位超时计数器
	USART_TimeTick_Init(USARTx, 0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITWrite_TCTask(USART_HandlerType*USARTx)
{
	//---定义了485，由于485响应的延迟问题,增加发送换行符，用于缓冲485的响应
	if ((USARTx->msg485Port != NULL) && (USARTx->msgTxHandler.msgStep == 0))
	{
		//---发送8Bit的数据
		LL_USART_TransmitData8(USARTx->msgUSART, 0x0D);
		USARTx->msgTxHandler.msgStep = 1;
	}
	else if ((USARTx->msg485Port != NULL) && (USARTx->msgTxHandler.msgStep == 1))
	{
		//---发送8Bit的数据
		LL_USART_TransmitData8(USARTx->msgUSART, 0x0A);
		USARTx->msgTxHandler.msgStep = 2;
	}
	else
	{
		//---清零发送步序
		USARTx->msgTxHandler.msgStep = 0;

		//---发送完成,发送数据发送完成中断不使能
		LL_USART_DisableIT_TC(USARTx->msgUSART);

		//---清空数据发送缓存区
		USART_WriteInit(USARTx);

		//---清零超时计数器
		//USART_TimeTick_DeInit(USARTx, 0);
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
UINT8_T USART_RealTime_AddByte(USART_HandlerType*USARTx, UINT8_T val)
{
	if (USARTx->msgTxHandler.pMsgVal != NULL)
	{
		if (USARTx->msgTxHandler.msgIndexW >= USARTx->msgTxHandler.msgSize)
		{
			USARTx->msgTxHandler.msgIndexW = 0;
		}
		USARTx->msgTxHandler.pMsgVal[USARTx->msgTxHandler.msgIndexW] = val;
		USARTx->msgTxHandler.msgIndexW++;

		//---判断发送寄存器空中断是否使能
		if (LL_USART_IsEnabledIT_TXE(USARTx->msgUSART) == 0)
		{
			//---设置485为发送模式
			USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);

			//---切换发送端口为输出模式
			USART_GPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);

			//---使能发送空中断
			LL_USART_EnableIT_TXE(USARTx->msgUSART);
		}
		return OK_0;
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：发送数据的大小
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_RealTime_AddByteSize(USART_HandlerType*USARTx, UINT16_T val)
{
	UINT8_T _return = ERROR_1;
	if (USARTx->msgTxHandler.pMsgVal != NULL)
	{
		if ((USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CHECKSUM) || (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC8))
		{
			val += 1;
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			val += 2;
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			val += 4;
		}

		//---判断是否有设备的ID信息
		if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
		{
			val += 1;
		}
		//---判断是否增加换行符
		if ((USARTx->msgTxHandler.msgAddNewLine==1))
		{
			val += 2;
		}
		//---判断数据的格式
		if (USARTx->msgTxHandler.msgSize > 250)
		{
			USART_RealTime_AddByte(USARTx, (UINT8_T)(val >> 8));
			_return = USART_RealTime_AddByte(USARTx, (UINT8_T)(val & 0xFF));
			val += 3;
		}
		else
		{
			_return = USART_RealTime_AddByte(USARTx, (UINT8_T)(val & 0xFF));
			val += 2;
		}

		//---判断是否有设备的ID信息并填充ID信息
		if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
		{
			_return = USART_RealTime_AddByte(USARTx, USARTx->msgDeviceID);
		}
		
		//---计算发送数据的个数
		USARTx->msgTxHandler.msgCount = val;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：使用CRC，数据的必须从0开始使用
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_RealTime_AddByteCRC(USART_HandlerType*USARTx)
{
	//---校验CRC是否初始化
	if ((USARTx->msgTxHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		UINT32_T crcVal = 0;

		//---判断数据校验模式
		if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			crcVal = CRCTask_CheckSum(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			crcVal = CRCTask_CRC16(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			crcVal = CRCTask_CRC32(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 24));
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 16));
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		return OK_0;
	}
	//---判断是否增加换行符
	if ((USARTx->msgTxHandler.msgAddNewLine==1))
	{
		USART_RealTime_AddByte(USARTx, 0x0D);
		USART_RealTime_AddByte(USARTx, 0x0A);
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 处理数据填充前的操作，约定好报头，和有效数据开始的位置
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_Init(USART_HandlerType*USARTx)
{
	//---等待传输完成
	while (USARTx->msgTxHandler.msgTaskState == USART_BUSY)
	{
		WDT_RESET();
	}
	USARTx->msgTxHandler.msgIndexW = 0;
	//---填充数据报头
	USART_FillMode_AddByte( USARTx, USARTx->msgTxID );
	//---填充数据长度
	USART_FillMode_AddByte( USARTx, 0x00 );
	if (USARTx->msgTxHandler.msgSize > 0xFF)
	{
		USART_FillMode_AddByte( USARTx, 0x00 );
	}
	//---填充多设备通信的设备ID
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		USART_FillMode_AddByte(USARTx, USARTx->msgDeviceID);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：数据填充后在发送
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_AddByte(USART_HandlerType*USARTx, UINT8_T val)
{
	USARTx->msgTxHandler.pMsgVal[USARTx->msgTxHandler.msgIndexW] = val;
	USARTx->msgTxHandler.msgIndexW++;
	if (USARTx->msgTxHandler.msgIndexW >= USARTx->msgTxHandler.msgSize)
	{
		USARTx->msgTxHandler.msgIndexW = 0;
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
UINT8_T USART_FillMode_AddData(USART_HandlerType*USARTx, UINT8_T *pVal, UINT16_T length)
{
	UINT16_T i = 0;
	UINT8_T _return = OK_0;
	for (i = 0; i < length; i++)
	{
		_return = USART_FillMode_AddByte(USARTx, pVal[i]);

		//---判断数据是否填充正确
		if (_return != OK_0)
		{
			break;
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
UINT8_T USART_CRCTask_Read(USART_HandlerType*USARTx)
{
	UINT32_T crcVal = 0;
	UINT32_T crcTemp = 0;

	//---接收数据的个数
	UINT16_T length = USARTx->msgRxHandler.msgIndexW;

	//---校验CRC是否初始化
	if ((USARTx->msgRxHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		//---判断数据校验模式
		if (USARTx->msgRxHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			length -= 1;
			if (USARTx->msgRxHandler.msgSize < 250)
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length);

				//---获取校验和
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 2];

				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxHandler.pMsgVal[2] = (UINT8_T)(length);

				//---获取校验和
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 3];

				//---参与校验和计算的数据总长度
				length += 3;
			}

			//---计算校验和
			crcVal = CRCTask_CheckSum(USARTx->msgRxHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			length -= 1;
			if (USARTx->msgRxHandler.msgSize < 250)
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length);

				//---获取校验和
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 2];

				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxHandler.pMsgVal[2] = (UINT8_T)(length);

				//---获取校验和
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 3];

				//---参与校验和计算的数据总长度
				length += 3;
			}

			//---计算CRC8校验值
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgRxHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			length -= 2;
			if (USARTx->msgRxHandler.msgSize < 250)
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length);

				//---获取CRC16的高位值
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 1];

				//---获取CRC16的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 2];

				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxHandler.pMsgVal[2] = (UINT8_T)(length);

				//---获取CRC16的高位值
				crcTemp = USARTx->msgRxHandler.pMsgVal[length + 2];

				//---获取CRC16的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 3];

				//---参与校验和计算的数据总长度
				length += 3;
			}

			//---计算CRC16校验值
			crcVal = CRCTask_CRC16(USARTx->msgRxHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			length -= 4;
			if (USARTx->msgRxHandler.msgSize < 250)
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length);

				//---获取CRC32的最高位值
				crcTemp = USARTx->msgRxHandler.pMsgVal[length - 1];

				//---获取CRC32的次高位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length];

				//---获取CRC32的次低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 1];

				//---获取CRC32的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 2];

				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxHandler.pMsgVal[2] = (UINT8_T)(length);

				//---获取CRC32的最高位值
				crcTemp = USARTx->msgRxHandler.pMsgVal[length];

				//---获取CRC32的次高位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 1];

				//---获取CRC32的次低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 2];

				//---获取CRC32的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxHandler.pMsgVal[length + 3];
				length += 3;
			}
			crcVal = CRCTask_CRC32(USARTx->msgRxHandler.pMsgVal, length);
		}

		//---比对CRC的值
		if (crcVal != crcTemp)
		{
			return  ERROR_1;
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
UINT8_T USART_CRCTask_Write(USART_HandlerType*USARTx)
{
	//---校验CRC是否初始化
	if ((USARTx->msgTxHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		UINT32_T crcVal = 0;
		//--要发送数据的总长度
		UINT16_T length = USARTx->msgTxHandler.msgIndexW;

		//---判断是否增加换行符
		if ((USARTx->msgTxHandler.msgAddNewLine==1))
		{
			length += 2;
		}
		//---数据大小的整理
		if (USARTx->msgTxHandler.msgSize < 0xFF)
		{
			USARTx->msgTxHandler.pMsgVal[1] = (UINT8_T)(length- 2);
		}
		else
		{
			USARTx->msgTxHandler.pMsgVal[1] = (UINT8_T)((length - 3) >> 8);
			USARTx->msgTxHandler.pMsgVal[2] = (UINT8_T)((length - 3));
		}
		
		//---判断数据校验模式
		if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			//---校验和
			crcVal = CRCTask_CheckSum(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			//---CRC8校验
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			//---CRC16校验
			crcVal = CRCTask_CRC16(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			//---CRC32校验
			crcVal = CRCTask_CRC32(USARTx->msgTxHandler.pMsgVal, USARTx->msgTxHandler.msgIndexW);
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 24));
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 16));
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}

		//---发送数据的大小
		USARTx->msgTxHandler.msgCount = USARTx->msgTxHandler.msgIndexW;
		return OK_0;
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
UINT8_T  USART_FillMode_WriteSTART(USART_HandlerType*USARTx, UINT8_T isNeedID)
{
	//---等待传输完成
	while (USARTx->msgTxHandler.msgTaskState == USART_BUSY)
	{
		WDT_RESET();
	}

	//---设置发送状态为空闲中
	USARTx->msgTxHandler.msgTaskState = USART_BUSY;

	//---判断是否需要填充报头
	if (isNeedID == 1)
	{
		//---填充报头和数据长度
		USARTx->msgTxHandler.pMsgVal[0] = USARTx->msgTxID;
	}

	//---填充设备ID
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		USARTx->msgTxHandler.pMsgVal[USARTx->msgIDIndex] = USARTx->msgDeviceID;
	}
	
	//---发送CRC处理
	USART_CRCTask_Write(USARTx);

	//---判断是否增加换行符
	if (( USARTx->msgTxHandler.msgAddNewLine == 1 ))
	{
		USART_FillMode_AddByte( USARTx, 0x0D );
		USART_FillMode_AddByte( USARTx, 0x0A );
	}
	
	//---数据大小的整理
	if (USARTx->msgTxHandler.msgSize < 0xFF)
	{
		USARTx->msgTxHandler.pMsgVal[1] = (UINT8_T)(USARTx->msgTxHandler.msgIndexW - 2);
	}
	else
	{
		USARTx->msgTxHandler.pMsgVal[1] = (UINT8_T)((USARTx->msgTxHandler.msgIndexW - 3) >> 8);
		USARTx->msgTxHandler.pMsgVal[2] = (UINT8_T)((USARTx->msgTxHandler.msgIndexW - 3));
	}

	USARTx->msgTxHandler.msgCount=USARTx->msgTxHandler.msgIndexW;

	//---设置485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);

	//---切换发送端口为输出模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);

	//---发送数据寄存器空中断使能
	LL_USART_EnableIT_TXE(USARTx->msgUSART);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 获取溢出标志
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetOVF(USART_HandlerType* USARTx, UINT8_T isRx)
{
	if (isRx)
	{
		return USARTx->msgRxHandler.msgOverFlow;
	}
	else
	{
		return USARTx->msgTxHandler.msgOverFlow;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 清除溢出标志
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ClearOVF(USART_HandlerType* USARTx, UINT8_T isRx)
{
	if (isRx)
	{
		USARTx->msgRxHandler.msgOverFlow = 0;
	}
	else
	{
		USARTx->msgTxHandler.msgOverFlow = 0;
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
UINT8_T USART_GetReadState(USART_HandlerType* USARTx)
{
	return USARTx->msgRxHandler.msgTaskState;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ClearReadState(USART_HandlerType* USARTx)
{
	USARTx->msgRxHandler.msgTaskState = USART_BUSY;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetWriteState(USART_HandlerType* USARTx)
{
	return USARTx->msgTxHandler.msgTaskState;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ClearWriteState(USART_HandlerType* USARTx)
{
	USARTx->msgTxHandler.msgTaskState = USART_BUSY;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ReadInit(USART_HandlerType*  USARTx)
{
	//---设置485位接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);

	//---清零步序
	USARTx->msgRxHandler.msgStep = 0;

	//---清除写数组索引
	USARTx->msgRxHandler.msgIndexW = 0;

	//---清除读数组索引
	USARTx->msgRxHandler.msgIndexR = 0;

	//---清零接收数据个数
	USARTx->msgRxHandler.msgCount = 0;

	//---清零接收完成标志
	USARTx->msgRxHandler.msgTaskState = USART_BUSY;

	//---清零超时计数
	USARTx->msgRxHandler.msgNowTime = 0;

	//---清零超时计数
	USARTx->msgRxHandler.msgEndTime = 0;

	//---清零超时标志
	USARTx->msgRxHandler.msgOverFlow = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_WriteInit(USART_HandlerType*  USARTx)
{
	//---设置发送状态为发送完成
	USARTx->msgTxHandler.msgTaskState = USART_OK;

	//---清除写数组索引
	USARTx->msgTxHandler.msgIndexW = 0;

	//---清除读数组索引
	USARTx->msgTxHandler.msgIndexR = 0;

	//---清零发送数据个数
	USARTx->msgTxHandler.msgCount = 0;

	//---清零超时计数
	USARTx->msgTxHandler.msgNowTime = 0;

	//---清零超时计数
	USARTx->msgTxHandler.msgEndTime = 0;

	//---清零超时标志
	USARTx->msgTxHandler.msgOverFlow = 0;

	//---操作步序归零
	USARTx->msgTxHandler.msgStep = 0;

	//---设置485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);

	//---数据发送完成，切换端口为输入模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 校验设备的ID信息
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DeviceID(USART_HandlerType*USARTx)
{
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		if (USARTx->msgRxHandler.pMsgVal[USARTx->msgIDIndex] != USARTx->msgDeviceID)
		{
			return ERROR_1;
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
void USART_Printf(USART_HandlerType*USARTx, char*fmt, ...)
{
#ifdef USE_USART_PRINTF

	//---检查发送寄存器空中断是否使能
	if (LL_USART_IsEnabledIT_TXE(USARTx->msgUSART))
	{
		//---等待发送完成
		while (LL_USART_IsActiveFlag_TXE(USARTx->msgUSART) != RESET)
		{
			WDT_RESET();
		}
	}

	//---检查发送完成中断
	if (LL_USART_IsEnabledIT_TC(USARTx->msgUSART))
	{
		//---等待发送完成
		while ((LL_USART_IsActiveFlag_TC(USARTx->msgUSART) != RESET) && (USARTx->msgTxHandler.msgStep == 0))
		{
			WDT_RESET();
		}
		LL_USART_ClearFlag_TC(USARTx->msgUSART);
	}

	//---定义485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);

	//---切换发送端口为输出模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);

	//---计算数据
	UINT16_T length = 0, i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);

	//---用于向字符串中打印数据、数据格式用户自定义;返回参数是最终生成字符串的长度
	length = (UINT16_T)vsnprintf(printfBuffer, USART_PRINTF_SIZE, fmt, arg_ptr);
	va_end(arg_ptr);

	//---数据发送
	for (i = 0; i < length; i++)
	{
		//---查询的方式发送数据
		USART_PollMode_WriteByte(USARTx, printfBuffer[i]);
	}
	
	//---定义485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);

	//---数据发送完成，切换端口为输入模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：打印系统时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_PrintfClockFreq(USART_HandlerType*USARTx)
{
	LL_RCC_ClocksTypeDef rccClock = { 0 };

	//---获取系统时钟
	LL_RCC_GetSystemClocksFreq(&rccClock);

	//---打印系统时钟
	USART_Printf(USARTx, "SYSCLK=%UHz\r\n", rccClock.SYSCLK_Frequency);

	//---打印系统时钟
	USART_Printf(USARTx, "HCLK=%UHz\r\n", rccClock.HCLK_Frequency);

	//---打印系统时钟
	USART_Printf(USARTx, "PCLK1=%UHz\r\n", rccClock.PCLK1_Frequency);

	//---打印系统时钟
	USART_Printf(USARTx, "PCLK2=%UHz\r\n", rccClock.PCLK2_Frequency);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Clock(USART_TypeDef* USARTx, UINT8_T isEnable)
{
	if (USARTx == USART1)
	{
		if (isEnable)
		{
			//---使能USART1的时钟信号
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		}
		else
		{
			//---不使能USART1的时钟信号
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
		}
		return OK_0;
	}
	if (USARTx == USART2)
	{
		if (isEnable)
		{
			//---使能USART2的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		}
		else
		{
			//---不使能USART2的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
		}
		return OK_0;
	}
#if defined(USART3)
	if (USARTx == USART3)
	{
		if (isEnable)
		{
			//---使能USART3的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
		}
		else
		{
			//---不使能USART3的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);
		}
		return OK_0;
	}
#endif /* USART3 */
#if defined(USART6)
	if (USARTx == USART6)
	{
		if (isEnable)
		{
			//---使能USART6的时钟信号
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
		}
		else
		{
			//---不使能USART6的时钟信号
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART6);
		}
		return OK_0;
	}
#endif /* USART6 */
#if defined(UART4)
	if (USARTx == UART4)
	{
		if (isEnable)
		{
			//---使能USART4的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
		}
		else
		{
			//---不使能USART4的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART4);
		}
		return OK_0;
	}
#endif /* UART4 */
#if defined(UART5)
	if (USARTx == UART5)
	{
		if (isEnable)
		{
			//---使能USART5的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
		}
		else
		{
			//---不使能USART5的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART5);
		}
		return OK_0;
	}
#endif /* UART5 */
#if defined(UART7)
	if (USARTx == UART7)
	{
		if (isEnable)
		{
			//---使能USART7的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);
		}
		else
		{
			//---不使能USART7的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART7);
		}
		return OK_0;
	}
#endif /* UART7 */
#if defined(UART8)
	if (USARTx == UART8)
	{
		if (isEnable)
		{
			//---使能USART8的时钟信号
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART8);
		}
		else
		{
			//---不使能USART8的时钟信号
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART8);
		}
		return OK_0;
	}
#endif /* UART8 */
#if defined(UART9)
	if (USARTx == UART9)
	{
		if (isEnable)
		{
			//---使能USART9的时钟信号
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART9);
		}
		else
		{
			//---不使能USART9的时钟信号
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_UART9);
		}
		return OK_0;
	}
#endif /* UART9 */
#if defined(UART10)
	if (USARTx == UART10)
	{
		if (isEnable)
		{
			//---使能USART10的时钟信号
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART10);
		}
		else
		{
			//---不使能USART10的时钟信号
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_UART10);
		}
		return OK_0;
	}
#endif /* UART10 */
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DeInit(USART_HandlerType*USARTx)
{
	LL_USART_DeInit(USARTx->msgUSART);
	USART_Clock(USARTx->msgUSART, 0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DeviceInit(USART_HandlerType *USARTx, UINT8_T id, UINT8_T idIndex, UINT8_T cmdIndex, UINT8_T d1Index, UINT8_T d2Index)
{
	USARTx->msgDeviceID = id;
	USARTx->msgIDIndex = idIndex;
	USARTx->msgCmdIndex = cmdIndex;
	USARTx->msgDataOneIndex = d1Index;
	USARTx->msgDataTwoIndex = d2Index;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART1_Init(USART_HandlerType*USARTx)
{
	//---USART1 GPIO Configuration
	//---PA9  ------> USART1_TX---端口复用为7
	//---PA10 ------> USART1_RX---端口复用为7
	//---使能端口时钟
	GPIOTask_Clock(GPIOA, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---模式配置
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;

	//---工作速度
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;

	//---复用功能的推完输出
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;

	//---输入上拉使能
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
#ifdef USE_MCU_STM32F1

	//---TX
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//---RX
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#else

	//---端口号
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;

	//---复用功能为USART1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

	//---注销串口的初始化
	LL_USART_DeInit(USART1);

	//---使能USART1的时钟信号
	USART_Clock(USART1, 1);

	//---USART的接口结构体
	LL_USART_InitTypeDef USART_InitStruct = { 0 };

	//---波特率
	USART_InitStruct.BaudRate = 115200;

	//---数据位
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;

	//---停止位
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;

	//---校验位
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;

	//---配置为收发模式
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;

	//---硬件流控制---默认为无
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;

	//---过采样配置
#if defined(USART_CR1_OVER8)

	//---过采样次数---默认配置为16
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
#endif

	//---初始化串口
	LL_USART_Init(USART1, &USART_InitStruct);

	//---串口异步模式配置
	LL_USART_ConfigAsyncMode(USART1);

	//---USART1_IRQ中断配置---中断等级配置
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));

	//---使能中断
	NVIC_EnableIRQ(USART1_IRQn);

	//---使能接收中断
	LL_USART_EnableIT_RXNE(USART1);

	//---使能串口
	LL_USART_Enable(USART1);

	//---配置消息结构体中的信息
	USARTx->msgTxPort = GPIOA;
	USARTx->msgTxBit = LL_GPIO_PIN_9;
	USARTx->msgUSART = USART1;

	//---串口序号
	USARTx->msgIndex = 1 + 1;

	//---校验是否需要超时函数
	if (USARTx->msgFuncTimeTick != NULL)
	{
		USARTx->msgRxHandler.msgMaxTime = 100;
		USARTx->msgTxHandler.msgMaxTime = 100;
	}
	else
	{
		USARTx->msgRxHandler.msgMaxTime = 0;
		USARTx->msgTxHandler.msgMaxTime = 0;
	}

	//---配置CRC的等级
	USARTx->msgTxHandler.msgCRCFlag = USART_CRC_NONE;

	//---配置报头和报尾
	USARTx->msgRxID = 0x55;
	USARTx->msgTxID = 0x5A;

	//---初始化485端口--推完输出模式，配置为接收模式
	//////
	//----------------
	/////
	//---定义485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);

	//---设置TX端口为输入模式
	USART_GPIOInit(USARTx, USART_TXGPIO_SET_INPUT);

	//---打印初始化信息
	USART_Printf(USARTx, "=>>串口1的初始化<<=\r\n");
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART2_Init(USART_HandlerType*USARTx)
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
UINT8_T USART3_Init(USART_HandlerType*USARTx)
{
	return OK_0;
}