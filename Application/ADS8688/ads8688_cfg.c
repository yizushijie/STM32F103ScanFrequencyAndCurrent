#include "ads8688_cfg.h"

//===ȫ�ֱ�������
ADS8688_HandlerType  g_ADS8688Device0 = { 0 };
pADS8688_HandlerType  pADS8688Device0 = &g_ADS8688Device0;

//===ͳһ��������
UINT8_T(*ADS8688_SPI_SEND_CMD)(ADS8688_HandlerType *, UINT8_T, UINT8_T *);

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS8688_SPI_Device0_Init(ADS8688_HandlerType *ADS8688x)
{
	//---CS
	ADS8688x->msgSPI.msgCS.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgCS.msgGPIOBit = LL_GPIO_PIN_4;

	//---SCK
	ADS8688x->msgSPI.msgSCK.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgSCK.msgGPIOBit = LL_GPIO_PIN_5;

	//---MISO
	ADS8688x->msgSPI.msgMISO.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgMISO.msgGPIOBit = LL_GPIO_PIN_6;

	//---MOSI
	ADS8688x->msgSPI.msgMOSI.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgMOSI.msgGPIOBit = LL_GPIO_PIN_7;

	//---����ģʽ
#ifndef USE_MCU_STM32F1

	//---�˿ڸ���ģʽ
	ADS8688x->msgSPI.msgGPIOAlternate = LL_GPIO_AF_5;
#endif

	//---SPI���
	ADS8688x->msgSPI.msgSPIx = SPI1;
#ifndef USE_MCU_STM32F1

	//---SPI��Э��
	ADS8688x->msgSPI.msgStandard = LL_SPI_PROTOCOL_MOTOROLA;
#endif

	//---����������
	ADS8688x->msgSPI.msgPluseWidth = 2;

	//---ʱ�ӿ���Ϊ�͵�ƽ
	ADS8688x->msgSPI.msgCPOL = 0;

	//---���ݲ����ڵ�һ��ʱ�ӱ���
	ADS8688x->msgSPI.msgCPOH = 1;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS8688_SPI_Device1_Init(ADS8688_HandlerType *ADS8688x)
{
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS8688_SPI_Device2_Init(ADS8688_HandlerType *ADS8688x)
{
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_HW_Init(ADS8688_HandlerType *ADS8688x)
{
	//---ע����ǰ����������
	SPITask_DeInit(&(ADS8688x->msgSPI));

	//---Ӳ���˿ڵ�����---Ӳ��ʵ��
	SPITask_MHW_GPIO_Init(&(ADS8688x->msgSPI));

	//---Ӳ��SPI�ĳ�ʼ��
	LL_SPI_InitTypeDef SPI_InitStruct = {0};

	//---SPI��ģʽ����
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;						//---����ģʽ
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;				//---8λ����
																	//---ʱ�Ӽ��Ե�����
	if (ADS8688x->msgSPI.msgCPOL == 0)
	{
		//---CLK����ʱΪ�͵�ƽ (CLK������ֻ���ǵ͵�ƽ)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
		GPIO_OUT_0(ADS8688x->msgSPI.msgSCK.msgGPIOPort, ADS8688x->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		//---CLK����ʱΪ�ߵ�ƽ (CLK������ֻ���ǵ͵�ƽ)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	}

	//---���ݲ�����ʱ�ӱ���λ��
	if (ADS8688x->msgSPI.msgCPOL == 0)
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	}
	else
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	}
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;							//---�������
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;		//---ϵͳʱ��256��Ƶ
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;						//---��λ��ǰ
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;	//---Ӳ��CRC��ʹ��
	SPI_InitStruct.CRCPoly = 7;

	//---��ʼ����ѯ��ʽ��SPI
	SPITask_MHW_PollMode_Init(&(ADS8688x->msgSPI), SPI_InitStruct);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SW_Init(ADS8688_HandlerType *ADS8688x)
{
	SPITask_DeInit(&(ADS8688x->msgSPI));

	//---Ӳ���˿ڵ�����---���ʵ��
	SPITask_MSW_GPIO_Init(&(ADS8688x->msgSPI));

	//---ʱ���ߵļ���
	if (ADS8688x->msgSPI.msgCPOL == 0)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgSCK.msgGPIOPort, ADS8688x->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(ADS8688x->msgSPI.msgSCK.msgGPIOPort, ADS8688x->msgSPI.msgSCK.msgGPIOBit);
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SW_SendCmd(ADS8688_HandlerType *ADS8688x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MSW_SendByteMSB(&(ADS8688x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_HW_SendCmd(ADS8688_HandlerType *ADS8688x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MHW_PollMode_WriteByte(&(ADS8688x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_Init(ADS8688_HandlerType *ADS8688x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW)
{
	//---ʹ�õ�DHT11�Ķ˿�
	if ((ADS8688x != NULL) && (ADS8688x == ADS8688_TASK_ONE))
	{
		ADS8688_SPI_Device0_Init(ADS8688x);
	}
	else if ((ADS8688x != NULL) && (ADS8688x == ADS8688_TASK_TWO))
	{
		ADS8688_SPI_Device1_Init(ADS8688x);
	}
	else if ((ADS8688x != NULL) && (ADS8688x == ADS8688_TASK_THREE))
	{
		ADS8688_SPI_Device2_Init(ADS8688x);
	}
	else
	{
		return ERROR_1;
	}

	//---�жϳ�ʼ���ķ�ʽ
	if (isHW != 0)
	{
		ADS8688x->msgSPI.msgModelIsHW = 1;
		ADS8688_SPI_HW_Init(ADS8688x);
		ADS8688_SPI_SEND_CMD = ADS8688_SPI_HW_SendCmd;
	}
	else
	{
		ADS8688x->msgSPI.msgModelIsHW = 0;
		ADS8688_SPI_SW_Init(ADS8688x);
		ADS8688_SPI_SEND_CMD = ADS8688_SPI_SW_SendCmd;
	}

	//---ע��ms��ʱʱ��
	if (pFuncDelayms != NULL)
	{
		ADS8688x->msgFuncDelayms = pFuncDelayms;
	}
	else
	{
		ADS8688x->msgFuncDelayms = DelayTask_ms;
	}

	//---ע��us��ʱ����
	if (pFuncDelayus != NULL)
	{
		ADS8688x->msgSPI.msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		ADS8688x->msgSPI.msgFuncDelayus = DelayTask_us;
	}

	//---ע��δ���
	ADS8688x->msgSPI.msgFuncTimeTick = pFuncTimerTick;

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�дADS8688����Ĵ���
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_WriteCmdReg(ADS8688_HandlerType *ADS8688x, UINT32_T cmd)
{
	UINT8_T _return = OK_0;
	GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);

	//---д��λ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (UINT8_T)(cmd >> 8), NULL);

	//---д��λ
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, (UINT8_T)(cmd & 0xFF), NULL);

	GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������Զ�ɨ��ģʽ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_AutoRSTMode(ADS8688_HandlerType *ADS8688x)
{
	return ADS8688_SPI_WriteCmdReg(ADS8688x, ADS8688_CMD_REG_AUTO_RST);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Program Registerд����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_WriteProgReg(ADS8688_HandlerType *ADS8688x, UINT8_T addr, UINT8_T val)
{
	UINT8_T _return = OK_0;
	GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);

	//---д��ַ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (addr << 1) | 0x01, NULL);

	//---д����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, val, NULL);

	GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Program Register������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ReadProgReg(ADS8688_HandlerType *ADS8688x, UINT8_T addr, UINT8_T *pVal)
{
	UINT8_T _return = OK_0;
	GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);

	//---д��ַ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (addr << 1) & 0xFE, NULL);

	//---����λ����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0xFF, pVal);
	pVal++;

	//---����λ����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0xFF, pVal);

	GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������Զ�ɨ������ͨ��
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SetAutoScanSequence(ADS8688_HandlerType *ADS8688x, UINT8_T seq)
{
	return ADS8688_SPI_WriteProgReg(ADS8688x, ADS8688_PROG_REG_AUTO_SEQ_EN, seq);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����ͨ�������̷�Χ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SetCHRange(ADS8688_HandlerType *ADS8688x, UINT8_T chReg, UINT8_T range)
{
	switch (chReg)
	{
		case ADS8688_PROG_REG_CH0_SET_RANGE:
			ADS8688x->msgCHRange[0] = range;
			break;
		case ADS8688_PROG_REG_CH1_SET_RANGE:
			ADS8688x->msgCHRange[1] = range;
			break;
		case ADS8688_PROG_REG_CH2_SET_RANGE:
			ADS8688x->msgCHRange[2] = range;
			break;
		case ADS8688_PROG_REG_CH3_SET_RANGE:
			ADS8688x->msgCHRange[3] = range;
			break;
		case ADS8688_PROG_REG_CH4_SET_RANGE:
			ADS8688x->msgCHRange[4] = range;
			break;
		case ADS8688_PROG_REG_CH5_SET_RANGE:
			ADS8688x->msgCHRange[5] = range;
			break;
		case ADS8688_PROG_REG_CH6_SET_RANGE:
			ADS8688x->msgCHRange[6] = range;
			break;
		case ADS8688_PROG_REG_CH7_SET_RANGE:
			ADS8688x->msgCHRange[7] = range;
			break;
		default:
			return ERROR_1;
	}
	return ADS8688_SPI_WriteProgReg(ADS8688x, chReg, range);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Ӳ����λ�豸
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T  ADS8688_SPI_HardReset(ADS8688_HandlerType *ADS8688x)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������λ�豸
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T  ADS8688_SPI_SoftReset(ADS8688_HandlerType *ADS8688x)
{
	return ADS8688_SPI_WriteCmdReg(ADS8688x, ADS8688_CMD_REG_RST);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ���ȡ�Զ�ɨ��ͨ�����е�ADת������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T  ADS8688_SPI_GetAutoRSTMode_ADData(ADS8688_HandlerType *ADS8688x, UINT8_T chNum)
{
	UINT8_T i = 0;
	UINT8_T temp = 0;
	UINT8_T _return = 0;
	for (i = 0; i < chNum; i++)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);

		//---д��λ
		_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, NULL);

		//---д��λ
		_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, NULL);

		//---ADת�������λ��ǰ����λ�ں�
		_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0xFF, &temp);
		ADS8688x->msgCHReseult[i] = temp;
		_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0xFF, &temp);
		ADS8688x->msgCHReseult[i] = (ADS8688x->msgCHReseult[i] << 8) + temp;

		GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	return  _return;
}