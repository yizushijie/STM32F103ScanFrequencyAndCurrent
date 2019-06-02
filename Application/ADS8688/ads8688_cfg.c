#include "ads8688_cfg.h"

//===ȫ�ֱ�������
ADS8688_HandlerType  g_ADS8688Device0 = { 0 };
pADS8688_HandlerType  pADS8688Device0 = &g_ADS8688Device0;

//===ͳһ�����ֽ�����
UINT8_T(*ADS8688_SPI_SEND_CMD)(ADS8688_HandlerType *, UINT8_T, UINT8_T *);
//===ͳһ������������
UINT8_T(*ADS8688_SPI_SEND_ARRAY)(ADS8688_HandlerType*, UINT8_T*, UINT8_T*,UINT16_T);

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS8688_SPI_Device0_Init(ADS8688_HandlerType *ADS8688x)
{

	//---��λ�ź�
	ADS8688x->msgHWRST.msgGPIOPort = NULL;
	ADS8688x->msgHWRST.msgGPIOBit = LL_GPIO_PIN_0;

	//---GPIOʱ��ʹ��
	if (ADS8688x->msgHWRST.msgGPIOPort != NULL)
	{
		GPIOTask_Clock(ADS8688x->msgHWRST.msgGPIOPort, 1);
	}

	//---CS
	ADS8688x->msgSPI.msgCS.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgCS.msgGPIOBit = LL_GPIO_PIN_4;

	//---SCK
	ADS8688x->msgSPI.msgSCK.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgSCK.msgGPIOBit = LL_GPIO_PIN_5;

	//---MISO  SD0
	ADS8688x->msgSPI.msgMISO.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgMISO.msgGPIOBit = LL_GPIO_PIN_6;

	//---MOSI SDI
	ADS8688x->msgSPI.msgMOSI.msgGPIOPort = GPIOA;
	ADS8688x->msgSPI.msgMOSI.msgGPIOBit = LL_GPIO_PIN_7;

	//---GPIO������
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO�ĳ�ʼ��
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;					//---����״̬Ϊ���ģʽ
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;		//---GPIO���ٶ�
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		//---���ģʽ---�������
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;						//---����ʹ��

	//---��ʼ��RST
	if (ADS8688x->msgHWRST.msgGPIOPort != NULL)
	{
		GPIO_InitStruct.Pin = ADS8688x->msgHWRST.msgGPIOBit;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		LL_GPIO_Init(ADS8688x->msgHWRST.msgGPIOPort, &GPIO_InitStruct);
		GPIO_OUT_1(ADS8688x->msgHWRST.msgGPIOPort, ADS8688x->msgHWRST.msgGPIOBit);
	}

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

	UINT8_T i = 0;
	for (i = 0; i < ADS8688_CHANNEL_MAX; i++)
	{
		ADS8688x->msgChannelRange[i] = 0;
		ADS8688x->msgIsPositive[i] = 0;
		ADS8688x->msgChannelADCResult[i] = 0;
		ADS8688x->msgChannelPowerResult[i] = 0;
		ADS8688_SPI_ChannelRange(ADS8688x, i);
	}

	ADS8688x->msgChipID = 0x28;
	//---Ĭ����0xFF
	ADS8688x->msgAutoSeqEn = 0xFF;
	//---����������
	ADS8688x->msgSPI.msgPluseWidth = 2;
	//---ʱ�ӿ���Ϊ�͵�ƽ
	ADS8688x->msgSPI.msgCPOL = 0;
	//---���ݲ����ڵڶ���ʱ�ӱ���
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
	if (ADS8688x->msgSPI.msgCPOH ==0)
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	}
	else
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	}
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;							//---�������
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;		//---ϵͳʱ��4��Ƶ
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
//////��		�ܣ������ֽ�����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SW_SendCmd(ADS8688_HandlerType *ADS8688x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MSW_WriteAndReadByteMSB(&(ADS8688x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�������������
//////���������
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_SW_SendArray(ADS8688_HandlerType* ADS8688x, UINT8_T* pWVal, UINT8_T* pRVal, UINT16_T length)
{
	//---���ݷ���
	return SPITask_MSW_WriteAndReadDataMSB(&(ADS8688x->msgSPI), pWVal, pRVal, length);
}
///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������ֽ�����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_HW_SendCmd(ADS8688_HandlerType *ADS8688x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MHW_PollMode_WriteAndReadByte(&(ADS8688x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�������������
//////���������
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_HW_SendArray(ADS8688_HandlerType* ADS8688x, UINT8_T *pWVal, UINT8_T* pRVal,UINT16_T length)
{
	//---���ݷ���
	return SPITask_MHW_PollMode_WriteAndReadData(&(ADS8688x->msgSPI), pWVal, pRVal,length);
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
		ADS8688_SPI_SEND_ARRAY = ADS8688_SPI_HW_SendArray;
	}
	else
	{
		ADS8688x->msgSPI.msgModelIsHW = 0;
		ADS8688_SPI_SW_Init(ADS8688x);
		ADS8688_SPI_SEND_CMD = ADS8688_SPI_SW_SendCmd;
		ADS8688_SPI_SEND_ARRAY = ADS8688_SPI_SW_SendArray;
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

	//---����ADS8688
	return ADS8688_SPI_ConfigInit(ADS8688x);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�дADS8688����Ĵ���
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_WriteCommandReg(ADS8688_HandlerType *ADS8688x, UINT32_T cmd)
{
	UINT8_T _return = OK_0;
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	//---д��λ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (UINT8_T)(cmd >> 8), NULL);
	//---д��λ
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, (UINT8_T)(cmd & 0xFF), NULL);
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Program Registerд����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_WriteProgramReg(ADS8688_HandlerType *ADS8688x, UINT8_T addr, UINT8_T val)
{
	UINT8_T _return = OK_0;
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}

	//---д��ַ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (addr << 1) | 0x01, NULL);
	//---д����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, val, NULL);

	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Program Register������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ReadProgramReg(ADS8688_HandlerType *ADS8688x, UINT8_T addr, UINT8_T *pVal)
{
	UINT8_T _return = OK_0;
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	//---д��ַ
	_return = ADS8688_SPI_SEND_CMD(ADS8688x, (addr << 1) & 0xFE, NULL);
	//---����λ����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, pVal);
	pVal++;
	//---����λ����
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, pVal);
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ReadCommandBack(ADS8688_HandlerType* ADS8688x, UINT8_T* pVal)
{
	return ADS8688_SPI_ReadProgramReg(ADS8688x, ADS8688_CMD_READ_BACK, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�����������ѡ�е�״̬
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_NO_OP(ADS8688_HandlerType* ADS8688x)
{
	return ADS8688_SPI_WriteCommandReg(ADS8688x, ADS8688_CMD_REG_NO_OP);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_STDBY(ADS8688_HandlerType* ADS8688x)
{
	return ADS8688_SPI_WriteCommandReg(ADS8688x, ADS8688_CMD_REG_STDBY);
}


///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�
//////�������:
//////�������:
//////˵		�����˳���ģʽ��ִ��AUTO_RST����MAN_CH_n�������Ҫ�ȴ�����20us�Ա�֤�������ݵ�ADת��
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_PWRDN(ADS8688_HandlerType* ADS8688x)
{
	return ADS8688_SPI_WriteCommandReg(ADS8688x, ADS8688_CMD_REG_PWR_DN);
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
	if (ADS8688x->msgHWRST.msgGPIOPort!=NULL)
	{
		GPIO_OUT_0(ADS8688x->msgHWRST.msgGPIOPort, ADS8688x->msgHWRST.msgGPIOBit);
		ADS8688x->msgFuncDelayms(1);
		GPIO_OUT_1(ADS8688x->msgHWRST.msgGPIOPort, ADS8688x->msgHWRST.msgGPIOBit);
		ADS8688x->msgFuncDelayms(1);
	}
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
	return ADS8688_SPI_WriteCommandReg(ADS8688x, ADS8688_CMD_REG_RST);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ��豸��λ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_Reset(ADS8688_HandlerType* ADS8688x)
{
	UINT8_T _return = OK_0;
	if (ADS8688x->msgHWRST.msgGPIOPort != NULL)
	{
		_return = ADS8688_SPI_HardReset(ADS8688x);
	}
	else
	{
		_return = ADS8688_SPI_SoftReset(ADS8688x);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������Զ�ɨ��ģʽ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_AUTORST(ADS8688_HandlerType* ADS8688x)
{
	return ADS8688_SPI_WriteCommandReg(ADS8688x, ADS8688_CMD_REG_AUTO_RST);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�ͨ��ѡ��
//////�������:
//////�������:
//////˵		�����˳���ģʽ��ִ��AUTO_RST����MAN_CH_n�������Ҫ�ȴ�����20us�Ա�֤�������ݵ�ADת��
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ManualChannel(ADS8688_HandlerType* ADS8688x,UINT16_T manualCHCmd)
{
	return ADS8688_SPI_WriteCommandReg(ADS8688x, manualCHCmd);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ������豸ID
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_WriteChipID(ADS8688_HandlerType* ADS8688x, UINT8_T devid)
{
	devid <<= 6;
	ADS8688x->msgChipID = (ADS8688x->msgChipID & 0x2F) | (devid & 0xC0);
	return ADS8688_SPI_WriteProgramReg(ADS8688x, ADS8688_PROG_REG_FEATURE_SELECT, ADS8688x->msgChipID);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ���ȡ�豸ID
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ReadChipID(ADS8688_HandlerType* ADS8688x, UINT8_T *pDevID)
{
	return ADS8688_SPI_ReadProgramReg(ADS8688x, ADS8688_PROG_REG_FEATURE_SELECT, pDevID);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�У���豸ID
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_CheckChipID(ADS8688_HandlerType* ADS8688x)
{
	UINT8_T tempChipID = 0x00;
	UINT8_T _return = OK_0;
	_return = ADS8688_SPI_ReadChipID(ADS8688x, &tempChipID);
	//---У���ȡ��ID��Ϣ
	if (tempChipID!=ADS8688x->msgChipID)
	{
		_return += ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ�̽���豸�Ƿ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_DetectionDevice(ADS8688_HandlerType* ADS8688x)
{
	UINT8_T tempAutoSeqEn[2] = { 0 };
	UINT8_T _return = OK_0;
	_return=ADS8688_SPI_ReadProgramReg(pADS8688Device0, ADS8688_PROG_REG_AUTO_SEQ_EN, tempAutoSeqEn);
	//---У�����Ƿ���ȷ
	if (tempAutoSeqEn[1]!=ADS8688x->msgAutoSeqEn)
	{
		_return += ERROR_1;
	}
	else
	{
		//---У���豸��ID��Ϣ
		_return = ADS8688_SPI_CheckChipID(ADS8688x);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������Զ�ɨ������ͨ��
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_AUTOSEQEN(ADS8688_HandlerType* ADS8688x, UINT8_T seq)
{
	ADS8688x->msgAutoSeqEn = seq;
	return ADS8688_SPI_WriteProgramReg(ADS8688x, ADS8688_PROG_REG_AUTO_SEQ_EN, seq);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ͨ������ģʽ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ChannelPWRDN(ADS8688_HandlerType* ADS8688x, UINT8_T ch)
{
	ADS8688x->msgChannelPWRDN = ch;
	return ADS8688_SPI_WriteProgramReg(ADS8688x, ADS8688_PROG_REG_CH_PWR_DN, ch);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ͨ������ģʽ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_FeatureSelect(ADS8688_HandlerType* ADS8688x, UINT8_T feature)
{
	ADS8688x->msgFeature = feature;
	return ADS8688_SPI_WriteProgramReg(ADS8688x, ADS8688_PROG_REG_FEATURE_SELECT, feature);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����ͨ�������̷�Χ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ChannelRangeSelect(ADS8688_HandlerType* ADS8688x, UINT8_T chReg, UINT8_T range)
{
	UINT8_T ch = ADS8688_CHANNEL_MAX;
	switch (chReg)
	{
		case ADS8688_PROG_REG_CH0_SET_RANGE:
			ADS8688x->msgChannelRange[0] = range;
			ch = 0;
			break;
		case ADS8688_PROG_REG_CH1_SET_RANGE:
			ADS8688x->msgChannelRange[1] = range;
			ch = 1;
			break;
		case ADS8688_PROG_REG_CH2_SET_RANGE:
			ADS8688x->msgChannelRange[2] = range;
			ch = 2;
			break;
		case ADS8688_PROG_REG_CH3_SET_RANGE:
			ADS8688x->msgChannelRange[3] = range;
			ch = 3;
			break;
		case ADS8688_PROG_REG_CH4_SET_RANGE:
			ADS8688x->msgChannelRange[4] = range;
			ch = 4;
			break;
		case ADS8688_PROG_REG_CH5_SET_RANGE:
			ADS8688x->msgChannelRange[5] = range;
			ch = 5;
			break;
		case ADS8688_PROG_REG_CH6_SET_RANGE:
			ADS8688x->msgChannelRange[6] = range;
			ch = 6;
			break;
		case ADS8688_PROG_REG_CH7_SET_RANGE:
			ADS8688x->msgChannelRange[7] = range;
			ch = 7;
			break;
		default:
			return ERROR_1;
	}

	//---ÿ��ͨ�������̵�����
	ADS8688_SPI_ChannelRange(ADS8688x, ch);
	//---����ͨ��������
	return ADS8688_SPI_WriteProgramReg(ADS8688x, chReg, range);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����ÿͨ�������̺��������ֵ
//////���������
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ChannelRange(ADS8688_HandlerType* ADS8688x, UINT8_T chIndex)
{
	UINT8_T _return = OK_0;
	if (chIndex==ADS8688_CHANNEL_MAX)
	{
		_return = ERROR_1;
	}
	else
	{
		switch (ADS8688x->msgChannelRange[chIndex])
		{
			case ADS8688_RANGE_5120MV_5120MV:
				ADS8688x->msgChannelRangeIsPositive[chIndex] = 0;
				ADS8688x->msgChannelRangeFullUVX1000[chIndex] = 5120000000;
				ADS8688x->msgChannelRangeBaseUVX1000[chIndex] = 156250;
				break;
			case ADS8688_RANGE_2560MV_2560MV:
				ADS8688x->msgChannelRangeIsPositive[chIndex] = 0;
				ADS8688x->msgChannelRangeFullUVX1000[chIndex] = 2560000000;
				ADS8688x->msgChannelRangeBaseUVX1000[chIndex] = 78125;
				break;
			case ADS8688_RANGE_0MV_10240MV:
				ADS8688x->msgChannelRangeIsPositive[chIndex] = 1;
				ADS8688x->msgChannelRangeFullUVX1000[chIndex] = 10240000000;
				ADS8688x->msgChannelRangeBaseUVX1000[chIndex] = 156250;
				break;
			case ADS8688_RANGE_0MV_5120MV:
				ADS8688x->msgChannelRangeIsPositive[chIndex] = 1;
				ADS8688x->msgChannelRangeFullUVX1000[chIndex] = 5120000000;
				ADS8688x->msgChannelRangeBaseUVX1000[chIndex] = 78125;
				break;
			case ADS8688_RANGE_10240MV_10240MV:
			default:
				ADS8688x->msgChannelRangeIsPositive[chIndex] = 0;
				ADS8688x->msgChannelRangeFullUVX1000[chIndex] = 10240000000;
				ADS8688x->msgChannelRangeBaseUVX1000[chIndex] = 312500;
				break;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ���ȡ�Զ�ɨ��ͨ�����е�ADת������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T  ADS8688_SPI_GetAutoRSTResult(ADS8688_HandlerType *ADS8688x, UINT8_T chNum)
{
	UINT8_T i = 0;
	UINT8_T adcRTemp[4] = {0};
	UINT8_T adcWTemp[4] = { 0 };
	UINT8_T _return = 0;
	UINT8_T rstMode = ADS8688x->msgAutoSeqEn;
	for (i = 0; i < chNum; i++)
	{
		//---�ж��Ƿ�ʹ���Զ�RSTɨ�蹦��
		if (rstMode & 0x01)
		{
			adcWTemp[3] = i;
			//---׼����ȡ����
			if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
			{
				GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
			}
			_return=ADS8688_SPI_SEND_ARRAY(ADS8688x, adcWTemp, adcRTemp, 4);
			//_return=SPITask_MHW_PollMode_WriteAndReadData(&ADS8688x->msgSPI, adcWTemp, adcRTemp, 4);
			//---�������ݵĶ�ȡ
			if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
			{
				GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
			}
			//---�����ȡ������
			ADS8688x->msgChannelADCResult[i] = adcRTemp[2];
			ADS8688x->msgChannelADCResult[i] = (ADS8688x->msgChannelADCResult[i] << 8) + adcRTemp[3];
			//---����������
			ADS8688_SPI_CalcChannelPower(ADS8688x, i);
		}
		rstMode >>= 1;
	}
	return  _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ���ȡ�յ�ɨ��ͨ����ֵ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T  ADS8688_SPI_GetManualChannelResult(ADS8688_HandlerType* ADS8688x, UINT16_T manualChannel)
{
	UINT8_T _return = OK_0;
	UINT8_T adcRTemp[4] = { 0 };
	UINT8_T adcWTemp[4] = { 0 };
	UINT8_T adcChannel = 0;
	//---�����ֶ�ɨ��ͨ��
	_return = ADS8688_SPI_ManualChannel(ADS8688x, manualChannel);
	//---׼����ȡ���õ�ɨ��ͨ����ֵ
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_0(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	/*
	//---д��λ
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, NULL);
	//---д��λ
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, NULL);
	//---ADת�������λ��ǰ����λ�ں�
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, &adcTemp[0]);
	_return += ADS8688_SPI_SEND_CMD(ADS8688x, 0x00, &adcTemp[1]);
	*/
	_return = ADS8688_SPI_SEND_ARRAY(ADS8688x, adcWTemp, adcRTemp, 4);
	//---������ȡ���õ�ɨ��ͨ����ֵ
	if (ADS8688x->msgSPI.msgCS.msgGPIOPort != NULL)
	{
		GPIO_OUT_1(ADS8688x->msgSPI.msgCS.msgGPIOPort, ADS8688x->msgSPI.msgCS.msgGPIOBit);
	}
	adcChannel = (UINT8_T)(manualChannel >> 10) & 0x0F;
	//---�ж��Ƿ���AUXͨ������
	if (adcChannel<8)
	{
		ADS8688x->msgChannelADCResult[adcChannel] = adcRTemp[2];
		ADS8688x->msgChannelADCResult[adcChannel] = (ADS8688x->msgChannelADCResult[adcChannel] << 8) + adcRTemp[3];
		//---����������
		ADS8688_SPI_CalcChannelPower(ADS8688x, adcChannel);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ͨ��ADC�Ĳɼ��������ADCͨ���ĵ�ѹֵ
//////���������
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_CalcChannelPower(ADS8688_HandlerType* ADS8688x, UINT8_T chIndex)
{
	//---����������
	ADS8688x->msgIsPositive[chIndex] = 0;
	UINT64_T calcPower = ADS8688x->msgChannelADCResult[chIndex];
	calcPower *= ADS8688x->msgChannelRangeBaseUVX1000[chIndex];
	//---�ж�ADC������������˫���Ի��ǵ�����
	if (ADS8688x->msgChannelRangeIsPositive[chIndex]==0)
	{
		if ((ADS8688x->msgChannelADCResult[chIndex]&0x8000)!=0)
		{
			ADS8688x->msgIsPositive[chIndex] = 2;
			ADS8688x->msgChannelPowerResult[chIndex] = (UINT32_T)((calcPower-ADS8688x->msgChannelRangeFullUVX1000[chIndex]) / 1000);
		}
		else
		{
			ADS8688x->msgIsPositive[chIndex] = 1;
			ADS8688x->msgChannelPowerResult[chIndex] = (UINT32_T)((ADS8688x->msgChannelRangeFullUVX1000[chIndex]-calcPower ) / 1000);
		}
	}
	else
	{
		ADS8688x->msgIsPositive[chIndex] = 2;
		ADS8688x->msgChannelPowerResult[chIndex] = (UINT32_T)(calcPower/ 1000);
	}
	return OK_0;
}


///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��	    �ܣ����ó�ʼ��ADS8688
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS8688_SPI_ConfigInit(ADS8688_HandlerType* ADS8688x)
{
	UINT8_T _return = OK_0;
	UINT8_T tempBuffer[3] = { 0 };
	//---��λоƬ
	_return = ADS8688_SPI_Reset(ADS8688x);
	//---����ɨ�跽ʽ
	_return=ADS8688_SPI_AUTOSEQEN(ADS8688x, 0xFF);
	//---��ȡ���õ�ֵ
	_return=ADS8688_SPI_ReadProgramReg(ADS8688x,ADS8688_PROG_REG_AUTO_SEQ_EN, tempBuffer);
	//---�ж�У��
	if (ADS8688x->msgAutoSeqEn!=tempBuffer[1])
	{
		_return=ERROR_1;
	}
	else
	{
		ADS8688_SPI_ChannelRangeSelect(ADS8688x, ADS8688_PROG_REG_CH0_SET_RANGE, ADS8688_RANGE_2560MV_2560MV);
		//---���ù���ģʽ
		ADS8688_SPI_AUTORST(ADS8688x);
	}
	return _return;
}