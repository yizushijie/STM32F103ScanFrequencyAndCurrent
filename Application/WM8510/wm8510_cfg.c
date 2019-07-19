#include "wm8510_cfg.h"

//===变量定义
WM8510_HandlerType  g_WM8510Device0 = { 0 };
pWM8510_HandlerType pWM8510Device0 = &g_WM8510Device0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void WM8510_I2C_StructInit(WM8510_HandlerType *WM8510x)
{
	WM8510x->mclkDIV = 1;
	WM8510x->bclkDIV = 1;
	WM8510x->preDIV = 0;
	WM8510x->pllN = 0;

	WM8510x->R0[0] = 0x00;
	WM8510x->R0[1] = 0x00;

	WM8510x->R1[0] = 0x02;
	WM8510x->R1[1] = 0x23;

	WM8510x->nowR6[0] = 0x0D;
	WM8510x->nowR6[1] = 0x41;
	WM8510x->lastR6[0] = 0x0D;
	WM8510x->lastR6[1] = 0x41;

	WM8510x->nowR36[0] = 0x00;

	WM8510x->nowR36[1] = 0x00;
	WM8510x->lastR36[0] = 0x00;
	WM8510x->lastR36[1] = 0x00;

	WM8510x->nowR37[0] = 0x00;
	WM8510x->nowR37[1] = 0x00;
	WM8510x->lastR37[0] = 0x00;
	WM8510x->lastR37[1] = 0x00;

	WM8510x->nowR38[0] = 0x00;
	WM8510x->nowR38[1] = 0x00;
	WM8510x->lastR38[0] = 0x00;
	WM8510x->lastR38[1] = 0x00;

	WM8510x->nowR39[0] = 0x00;
	WM8510x->nowR39[1] = 0x00;
	WM8510x->lastR39[0] = 0x00;
	WM8510x->lastR39[1] = 0x00;

#ifdef USE_WM8510_RESET
	WM8510x->freqChangeCount = 0;
#endif

	WM8510x->freqAdd = 0;
	WM8510x->pllK = 0;
	WM8510x->pllFreq = WM8510_MCLK_HZ;
	WM8510x->refOSC = WM8510_MCLK_HZ;
	WM8510x->freqHz = 0;
	WM8510x->freqKHz = 0;
	WM8510x->pllRate = 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_Device0_Init(WM8510_HandlerType *WM8510x)
{
	WM8510x->msgI2C.msgI2Cx = NULL;
	WM8510x->msgI2C.msgSCL.msgGPIOPort = GPIOB;
	WM8510x->msgI2C.msgSCL.msgGPIOBit = LL_GPIO_PIN_6;
	WM8510x->msgI2C.msgSDA.msgGPIOPort = GPIOB;
	WM8510x->msgI2C.msgSDA.msgGPIOBit = LL_GPIO_PIN_7;
	WM8510x->msgI2C.msgModelIsHW = 0;
	WM8510x->msgI2C.msgPluseWidth = 0;
	WM8510x->msgI2C.msgFuncDelayus = NULL;
	WM8510x->msgI2C.msgAddr = WM8510_WADDR;
	WM8510x->msgI2C.msgClockSpeed = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_Device1_Init(WM8510_HandlerType *WM8510x)
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
UINT8_T WM8510_I2C_Device2_Init(WM8510_HandlerType *WM8510x)
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
UINT8_T WM8510_I2C_Init(WM8510_HandlerType *WM8510x, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C)
{
	UINT8_T _return = OK_0;
	WM8510_I2C_StructInit(WM8510x);
	if ((WM8510x != NULL) && (WM8510x == WM8510_TASK_ONE))
	{
		WM8510_I2C_Device0_Init(WM8510x);
	}
	else if ((WM8510x != NULL) && (WM8510x == WM8510_TASK_TWO))
	{
		WM8510_I2C_Device1_Init(WM8510x);
	}
	else if ((WM8510x != NULL) && (WM8510x == WM8510_TASK_THREE))
	{
		WM8510_I2C_Device2_Init(WM8510x);
	}
	else
	{
		return ERROR_1;
	}

	//---判断是硬件I2C还是软件I2C
	if (isHWI2C)
	{
		//_return= I2CTask_MHW_Init(&(WM8510x->msgI2C), pFuncDelay);
		WM8510x->msgI2C.msgModelIsHW = 1;
	}
	else
	{
		_return = I2CTask_MSW_Init(&(WM8510x->msgI2C), pFuncDelayus);
		WM8510x->msgI2C.msgModelIsHW = 1;
	}
	_return = WM8510_I2C_START(WM8510x);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 0---WM8510初始化成功，1---初始化失败
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_START(WM8510_HandlerType *WM8510x)
{
	UINT8_T _return = OK_0;

	//---发送命令
	WM8510_I2C_SendCMD(WM8510x, WM8510x->R0);

	//---配置R0寄存器
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->R0);
	if (_return != 0)
	{
		goto GoToExit;
	}

	//---配置R0寄存器
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->R1);
GoToExit:
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_DeInit(WM8510_HandlerType *WM8510x)
{
	//---注销I2C设备
	if (WM8510x->msgI2C.msgModelIsHW == 1)
	{
		return ERROR_1;
	}
	else
	{
		return I2CTask_MSW_DeInit(&(WM8510x->msgI2C));
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 软件模拟I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_SWI2C_WriteReg(WM8510_HandlerType *WM8510x, UINT8_T *pVal, UINT8_T length)
{
	UINT8_T _return = OK_0;
	UINT8_T i = 0;

	//---启动并发送写操作
	_return = I2CTask_MSW_START(&(WM8510x->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}

	//---发送命令
	for (i = 0; i < length; i++)
	{
		//---发送数据
		I2CTask_MSW_SendByte(&(WM8510x->msgI2C), pVal[i]);

		//---读取ACK
		_return = I2CTask_MSW_ReadACK(&(WM8510x->msgI2C));

		//---判断应答
		if (_return)
		{
			_return = ERROR_3 + i;
			goto GoToExit;
		}
	}
GoToExit:

	//---发送停止信号
	I2CTask_MSW_STOP(&(WM8510x->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 硬件I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_HWI2C_WriteReg(WM8510_HandlerType *WM8510x, UINT8_T *pVal, UINT8_T length)
{
	return ERROR_1;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_SendCMD(WM8510_HandlerType *WM8510x, UINT8_T *pVal)
{
	if (WM8510x->msgI2C.msgModelIsHW == 0)
	{
		//---硬件I2C
		return WM8510_HWI2C_WriteReg(WM8510x, pVal, 2);
	}
	else
	{
		//---软件模拟I2C
		return WM8510_SWI2C_WriteReg(WM8510x, pVal, 2);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 计算输出频率的补偿值，每100KHz偏移10Hz
//////输入参数:
//////输出参数:
//////说		明：不适用1:1.5的分频比偏移是10，使用之后偏移是15
//////////////////////////////////////////////////////////////////////////////
void WM8510_Calc_FreqAdd(WM8510_HandlerType *WM8510x, UINT32_T freq)
{
	//WM8510x->freqAdd = ( freq / 1000 );
	//WM8510x->freqAdd *= 2;
	WM8510x->freqAdd = 0;//( WM8510x->freqAdd + 50 ) / 100;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void WM8510_Calc_PllRate(WM8510_HandlerType *WM8510x, UINT32_T freq)
{
	//---计算中，这个频率会发生改变，故每次使用的时候需要重新赋值
	//WM8510x->pllFreq = WM8510_MCLK_HZ;
	//---为了兼容兼容晶振误差，修改为可配置
	WM8510x->pllFreq = WM8510x->refOSC;

	//---补偿值，每次归零
	WM8510x->freqAdd = 0;

	//---最大频率
	if (freq >= WM8510_OUT_MAX_FREQ)
	{
		//---最大输出频率
		freq = WM8510_OUT_MAX_FREQ;

		//---最大值的补偿
		WM8510x->freqAdd = WM8510_OUT_MAX_FREQ_ADD;
	}

	//---最小频率
	else if (freq < WM8510_OUT_MIN_FREQ)
	{
		//---最小输出频率
		freq = WM8510_OUT_MIN_FREQ;

		//---最小值的补偿
		WM8510x->freqAdd = WM8510_OUT_MIN_FREQ_ADD;
	}

	//---最大和最下之间
	else
	{
		//---计算频率补偿值
		WM8510_Calc_FreqAdd(WM8510x, freq);
	}

	//---设定输出的频率
	WM8510x->freqHz = freq;
	WM8510x->freqKHz = (WM8510x->freqHz / 1000);

	//---补偿之后的实际频率值
	freq += WM8510x->freqAdd;

	//===参数计算，如果过WM8510的时钟晶振不是12M，这些参数需要重新计算
	if (freq > PDIV0_MDIV1_BDIV1_MIN)				//18000000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_1;
		WM8510x->bclkDIV = BCLK_DIV_1;
	}

	//---开启这部分会导致输出的波形的占空比不在是1:1，可能会影响最终的测试结果
	//else if (freq > PDIV0_MDIV1D5_BDIV1_MIN)		//12000000
	//{
	//	WM8510x->preDIV = 0;
	//	WM8510x->mclkDIV = MCLK_DIV_1D5;
	//	WM8510x->bclkDIV = BCLK_DIV_1;
	//	freq *= 3;// 乘以1.5
	//	freq >>= 1;
	//}
	else if (freq > PDIV0_MDIV2_BDIV1_MIN)		//9000000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_2;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq <<= 1;	// 乘以2
	}
	else if (freq > PDIV0_MDIV3_BDIV1_MIN)		//6000000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_3;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq *= 3;	// 乘以3
	}
	else if (freq > PDIV0_MDIV4_BDIV1_MIN)		//4500000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_4;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq <<= 2;	// 乘以4
	}
	else if (freq > PDIV0_MDIV6_BDIV1_MIN)		//3000000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_6;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq *= 6;	// 乘以6
	}
	else if (freq > PDIV0_MDIV8_BDIV1_MIN)		//2250000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq <<= 3;	// 乘以8
	}
	else if (freq > PDIV0_MDIV12_BDIV1_MIN)		//1500000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_1;
		freq *= 12;	// 乘以12
	}
	else if (freq > PDIV0_MDIV8_BDIV2_MIN)		//1125000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_2;
		freq <<= 4;	// 乘以16
	}
	else if (freq > PDIV0_MDIV12_BDIV2_MIN)		//750000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_2;
		freq *= 24;	// 乘以24
	}
	else if (freq > PDIV0_MDIV8_BDIV4_MIN)		//562500
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_4;
		freq <<= 5;	// 乘以32
	}
	else if (freq > PDIV0_MDIV12_BDIV4_MIN)		//375000
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_4;
		freq *= 48;	// 乘以48
	}
	else if (freq > PDIV0_MDIV8_BDIV8_MIN)		//281250
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_8;
		freq <<= 6;	// 乘以64
	}
	else if (freq > PDIV0_MDIV12_BDIV8_MIN)		//187500
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_8;
		freq *= 96;	// 乘以96
	}
	else if (freq > PDIV0_MDIV4_BDIV32_MIN)		//140625
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_4;
		WM8510x->bclkDIV = BCLK_DIV_32;
		freq <<= 7;	// 乘以128
	}
	else if (freq > PDIV0_MDIV6_BDIV32_MIN)		//93750
	{
		WM8510x->preDIV = 0;
		WM8510x->mclkDIV = MCLK_DIV_6;
		WM8510x->bclkDIV = BCLK_DIV_32;
		freq *= 192;	// 乘以192
	}
	else if (freq > PDIV1_MDIV8_BDIV16_MIN)		//70313
	{
		WM8510x->preDIV = 1;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_16;
		freq <<= 7;	// 乘以128
		WM8510x->pllFreq >>= 1;
	}
	else if (freq > PDIV1_MDIV12_BDIV16_MIN)	//46875
	{
		WM8510x->preDIV = 1;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_16;
		freq *= 192;	// 乘以192
		WM8510x->pllFreq >>= 1;
	}
	else if (freq > PDIV1_MDIV8_BDIV32_MIN)		//35156
	{
		WM8510x->preDIV = 1;
		WM8510x->mclkDIV = MCLK_DIV_8;
		WM8510x->bclkDIV = BCLK_DIV_32;
		freq <<= 8;	// 乘以256
		WM8510x->pllFreq >>= 1;
	}
	else
	{
		WM8510x->preDIV = 1;
		WM8510x->mclkDIV = MCLK_DIV_12;
		WM8510x->bclkDIV = BCLK_DIV_32;
		freq *= 384;	// 乘以384
		WM8510x->pllFreq >>= 1;
	}
	freq <<= 2;	// 乘以4

	//---输出与输入的比值(5到16之间)
	WM8510x->pllRate = (float)freq / WM8510x->pllFreq;

	//---取整数
	WM8510x->pllN = (UINT8_T)WM8510x->pllRate;

	//---取得K值
	WM8510x->pllK = (UINT32_T)((WM8510x->pllRate - WM8510x->pllN) * (float)0xFFFFFF);

	//===WM8510的数据格式是高位在前，地位在后
	//---主机模式且是分频PLL输出的值
	WM8510x->nowR6[0] = (6 << 1) | 0x01;
	WM8510x->nowR6[1] = (UINT8_T)WM8510x->mclkDIV << 5;
	WM8510x->nowR6[1] |= (UINT8_T)WM8510x->bclkDIV << 2;

	//---BCLK输出时钟
	WM8510x->nowR6[1] |= 0x01;

	WM8510x->nowR36[0] = (36 << 1);
	WM8510x->nowR36[1] = (WM8510x->preDIV == 0) ? 0 : (1 << 4);
	WM8510x->nowR36[1] |= WM8510x->pllN;

	WM8510x->nowR37[0] = (37 << 1);
	WM8510x->nowR37[1] = (UINT8_T)((WM8510x->pllK & 0xFC0000) >> 18);

	WM8510x->nowR38[0] = (UINT8_T)((38 << 1) | ((WM8510x->pllK & (UINT32_T)0x20000) == 0 ? 0 : 1));
	WM8510x->nowR38[1] = (UINT8_T)((WM8510x->pllK & 0x1FE00) >> 9);

	WM8510x->nowR39[0] = (UINT8_T)((39 << 1) | ((WM8510x->pllK & (UINT32_T)0x100) == 0 ? 0 : 1));
	WM8510x->nowR39[1] = (UINT8_T)(WM8510x->pllK & 0xFF);

#ifdef USE_WM8510_RESET

	//---WM8510的输出变化一次
	WM8510x->freqChangeCount += 1;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：多次修改WM8510的时钟输出会导致输出异常，最好每次都要设计寄存器
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_SetFreqHz(WM8510_HandlerType *WM8510x, UINT32_T freq)
{
	UINT8_T _return = OK_0;

	//---判断是否需要更新寄存器的值
	if (freq != WM8510x->freqHz)
	{
		//---计算各个寄存器的值
		WM8510_Calc_PllRate(WM8510x, freq);
	}

	//---判断是否需要进行频率的复位
#ifdef USE_WM8510_RESET
	if (WM8510x->freqChangeCount >= WM8510_FREQ_CHANGE_MAX)
	{
		//---执行设备复位操作
		WM8510_I2C_Reset(WM8510x);
	}
#endif

	//---判断寄存器是否发生改变，不改变则不更新数据
	if (memcmp(WM8510x->nowR6, WM8510x->lastR6, 2) != 0)
	{
		WM8510x->lastR6[0] = WM8510x->nowR6[0];
		WM8510x->lastR6[1] = WM8510x->nowR6[1];
		_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR6);
		if (_return != OK_0)
		{
			_return = ERROR_1;
			goto GoToExit;
		}
	}

	//---判断寄存器的值是否发生改变
	if (memcmp(WM8510x->nowR36, WM8510x->lastR36, 2) != 0)
	{
		WM8510x->lastR36[0] = WM8510x->nowR36[0];
		WM8510x->lastR36[1] = WM8510x->nowR36[1];
		_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR36);
		if (_return != OK_0)
		{
			_return = ERROR_2;
			goto GoToExit;
		}
	}

	//---判断寄存器的值是否发生改变
	if (memcmp(WM8510x->nowR37, WM8510x->lastR37, 2) != 0)
	{
		WM8510x->lastR37[0] = WM8510x->nowR37[0];
		WM8510x->lastR37[1] = WM8510x->nowR37[1];
		_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR37);
		if (_return != OK_0)
		{
			_return = ERROR_3;
			goto GoToExit;
		}
	}

	//---判断寄存器的值是否发生改变
	if (memcmp(WM8510x->nowR38, WM8510x->lastR38, 2) != 0)
	{
		WM8510x->lastR38[0] = WM8510x->nowR38[0];
		WM8510x->lastR38[1] = WM8510x->nowR38[1];
		_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR38);
		if (_return != OK_0)
		{
			_return = ERROR_4;
			goto GoToExit;
		}
	}

	//---判断寄存器的值是否发生改变
	if (memcmp(WM8510x->nowR39, WM8510x->lastR39, 2) != 0)
	{
		WM8510x->lastR39[0] = WM8510x->nowR39[0];
		WM8510x->lastR39[1] = WM8510x->nowR39[1];
		_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR39);
		if (_return != OK_0)
		{
			_return = ERROR_5;
		}
	}
GoToExit:
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  WM8510_I2C_SetFreqKHz(WM8510_HandlerType *WM8510x, float freq)
{
	UINT32_T temp = (UINT32_T)(freq * 1000);
	return WM8510_I2C_SetFreqHz(WM8510x, temp);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  WM8510_I2C_SetFreqMHz(WM8510_HandlerType *WM8510x, float freq)
{
	UINT32_T temp = (UINT32_T)(freq * 1000000);
	return WM8510_I2C_SetFreqHz(WM8510x, temp);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明： 获取WM8510要输出的频率
//////////////////////////////////////////////////////////////////////////////
UINT32_T WM8510_GetFreqHz(WM8510_HandlerType *WM8510x)
{
	return WM8510x->freqHz;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明： 复位WM8510的设置
//////////////////////////////////////////////////////////////////////////////
void WM8510_I2C_Reset(WM8510_HandlerType *WM8510x)
{
	//---等待初始化成功
	while (WM8510_I2C_START(WM8510x) != 0);

	//---寄存器值设置为零
	memset(WM8510x->lastR6, 0, 2);
	memset(WM8510x->lastR36, 0, 2);
	memset(WM8510x->lastR37, 0, 2);
	memset(WM8510x->lastR38, 0, 2);
	memset(WM8510x->lastR39, 0, 2);

	//---寄存器值设置为零
	memset(WM8510x->nowR6, 0, 2);
	memset(WM8510x->nowR36, 0, 2);
	memset(WM8510x->nowR37, 0, 2);
	memset(WM8510x->nowR38, 0, 2);
	memset(WM8510x->nowR39, 0, 2);

	//---设置为默认值
	WM8510x->pllFreq = WM8510x->refOSC;

	//---输出频率归零
	WM8510x->freqHz = 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：重新设置与输出频率有关的寄存器
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_SetFreqHzWithAllFreqReg(WM8510_HandlerType *WM8510x, UINT32_T freq)
{
	UINT8_T _return = OK_0;

	//---判断是否需要更新寄存器的值
	if (freq != WM8510x->freqHz)
	{
		//---计算各个寄存器的值
		WM8510_Calc_PllRate(WM8510x, freq);
	}

	//---判断是否需要进行频率的复位
#ifdef USE_WM8510_RESET
	if (WM8510x->freqChangeCount >= WM8510_FREQ_CHANGE_MAX)
	{
		//---执行设备复位操作
		WM8510_I2C_Reset(WM8510x);
	}
#endif

	//---设置寄存器R6
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR6);
	if (_return != OK_0)
	{
		_return = ERROR_1;
		goto GoToExit;
	}

	//---设置寄存器R36
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR36);
	if (_return != OK_0)
	{
		_return = ERROR_2;
		goto GoToExit;
	}

	//---设置寄存器R37
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR37);
	if (_return != OK_0)
	{
		_return = ERROR_3;
		goto GoToExit;
	}

	//---设置寄存器38
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR38);
	if (_return != OK_0)
	{
		_return = ERROR_4;
		goto GoToExit;
	}

	//---设置寄存器R39
	_return = WM8510_I2C_SendCMD(WM8510x, WM8510x->nowR39);
	if (_return != OK_0)
	{
		_return = ERROR_5;
	}
GoToExit:
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 校准输出的频率
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_CalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x)
{
	TimerTask_CalcFreq_Task(1);

	//---获取当前输出的频率
	UINT32_T freq = TimerTask_GetFreqKHz(); //(UINT32_T)(TimerTask_GetFreqKHz()/10);
	if (freq > WM8510x->freqKHz)
	{
		//---冗余计算
		if ((freq - 10) <= WM8510x->freqKHz)
		{
			return OK_0;
		}
	#if defined(USE_MCU_STM32)&&defined(USE_MCU_STM32F1)

		//---F1最大采集的频率是32M
		else if (freq > 32000)
		{
			return OK_0;
		}
	#endif
		return ERROR_1;
	}
	else
	{
		//---冗余计算
		if ((freq + 10) >= WM8510x->freqKHz)
		{
			return OK_0;
		}
	#if defined(USE_MCU_STM32)&&defined(USE_MCU_STM32F1)

		//---F1最大采集的频率是32M
		else if (freq > 32000)
		{
			return OK_0;
		}
	#endif
		return ERROR_2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_SetFreqHzWithAllFreqRegAndCalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x, UINT32_T freq)
{
	if (WM8510_I2C_SetFreqHzWithAllFreqReg(WM8510x, freq) == OK_0)
	{
		return WM8510_CalibrateFreqKHzOutPut(WM8510x);
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：使用定时器的外部计数模式，进行WM8510有源晶振输出频率的校准，默认MCU的时钟是准确的，
//////输入参数:
//////输出参数:
//////说		明：中间使用了一下冗余计算，目的是将有源晶振的频率向12M拟合，测试数据来源于示波器的测量
//////////////////////////////////////////////////////////////////////////////
UINT8_T WM8510_I2C_CalibrateClock(WM8510_HandlerType *WM8510x)
{
	UINT8_T _return = OK_0;
	UINT32_T outFreqKHz = WM8510_MCLK_KHZ;

	//---设置WM8510输出外部参考时钟
	if (WM8510_I2C_SetFreqHzWithAllFreqReg(WM8510x, outFreqKHz * 1000) == OK_0)
	{
		//---开启时钟校准程序
		TimerTask_CalcFreq_Task(1);

		//---获取当前输出的频率
		UINT32_T freq = (TimerTask_GetFreqKHz()-1);
		
		if(freq!=0xFFFF)
		{
			//outFreqKHz *= 10;

			//---频率大小的绝对误差
			UINT32_T freqDelta = ABS_SUB(outFreqKHz, freq);
			//---绝对误差的最小下限
			if(freqDelta==10)
			{
				freqDelta-=2;
			}
			//---12M有源晶振的误差补偿,实际频率小于12M
			if (freqDelta < 10)
			{
				WM8510x->refOSC = WM8510_MCLK_HZ - (freqDelta * 50);
			}

			//---实际频率大于12M
			else
			{
				WM8510x->refOSC = WM8510_MCLK_HZ + ((freqDelta + 5)) * 10;
			}

		}
		
		//---复位设备
		WM8510_I2C_Reset(WM8510x);
	}
	else
	{
		_return = ERROR_2;
	}
	return _return;
}