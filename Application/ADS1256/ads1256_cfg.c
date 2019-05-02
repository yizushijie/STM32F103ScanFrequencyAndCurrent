#include "ads1256_cfg.h"

//===ȫ�ֱ�������
ADS1256_HandlerType  g_ADS1256Device0 = { 0 };
pADS1256_HandlerType pADS1256Device0 = &g_ADS1256Device0;

UINT8_T(*ADS1256_SPI_SEND_CMD)(ADS1256_HandlerType *, UINT8_T, UINT8_T *);

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device0_Init(ADS1256_HandlerType *ADS1256x)
{
	//---DRDY
	ADS1256x->msgDRDY.msgGPIOPort = GPIOA;
	ADS1256x->msgDRDY.msgGPIOBit = LL_GPIO_PIN_3;
	
	//---GPIOʱ��ʹ��
	if (ADS1256x->msgDRDY.msgGPIOPort!=NULL)
	{
		GPIOTask_Clock(ADS1256x->msgDRDY.msgGPIOPort, 1);
	}

	//---��λ�ź�
	ADS1256x->msgHWRST.msgGPIOPort = NULL;
	ADS1256x->msgHWRST.msgGPIOBit = LL_GPIO_PIN_0;

	//---GPIOʱ��ʹ��
	if (ADS1256x->msgHWRST.msgGPIOPort!=NULL)
	{
		GPIOTask_Clock(ADS1256x->msgHWRST.msgGPIOPort, 1);
	}

	//---GPIO������
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO�ĳ�ʼ��
	GPIO_InitStruct.Pin = ADS1256x->msgDRDY.msgGPIOBit;			//---��Ӧ��GPIO������
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;					//---����״̬Ϊ���ģʽ
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;			//---GPIO���ٶ�
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		//---���ģʽ---�������
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;						//---����ʹ��
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0; //---�˿ڸ���ģʽ
#endif

	//---��ʼ��DRDY
	LL_GPIO_Init(ADS1256x->msgDRDY.msgGPIOPort, &GPIO_InitStruct);
	GPIO_OUT_1(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit);

	//---��ʼ��RST
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

	//---����ģʽ
#ifndef USE_MCU_STM32F1

	//---�˿ڸ���ģʽ
	ADS1256x->msgSPI.msgGPIOAlternate = LL_GPIO_AF_5;
#endif

	//---SPI���
	ADS1256x->msgSPI.msgSPIx = SPI1;
#ifndef USE_MCU_STM32F1

	//---SPI��Э��
	ADS1256x->msgSPI.msgStandard = LL_SPI_PROTOCOL_MOTOROLA;
#endif

	//---ADS1256����ת��������
	ADS1256x->msgDRate = 0xF0;

	//---�����������
	ADS1256x->msgSPI.msgPluseWidth = 0;

	//---ʱ�ӿ���Ϊ�͵�ƽ
	ADS1256x->msgSPI.msgCPOL = 0;

	//---���ݲ����ڵ�һ��ʱ�ӱ���
	ADS1256x->msgSPI.msgCPOH = 1;

}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device1_Init(ADS1256_HandlerType *ADS1256x)
{

}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void ADS1256_SPI_Device2_Init(ADS1256_HandlerType *ADS1256x)
{

}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_HW_Init(ADS1256_HandlerType *ADS1256x)
{
	//---ע����ǰ����������
	SPITask_DeInit(&(ADS1256x->msgSPI));

	//---Ӳ���˿ڵ�����---Ӳ��ʵ��
	SPITask_MHW_GPIO_Init(&(ADS1256x->msgSPI));

	//---Ӳ��SPI�ĳ�ʼ��
	LL_SPI_InitTypeDef SPI_InitStruct = {0};

	//---SPI��ģʽ����
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;						//---����ģʽ
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;				//---8λ����

	//---ʱ�Ӽ��Ե�����
	if (ADS1256x->msgSPI.msgCPOL == 0)
	{
		//---CLK����ʱΪ�͵�ƽ (CLK������ֻ���ǵ͵�ƽ)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
		GPIO_OUT_0(ADS1256x->msgSPI.msgSCK.msgGPIOPort, ADS1256x->msgSPI.msgSCK.msgGPIOBit);
	}
	else
	{
		//---CLK����ʱΪ�ߵ�ƽ (CLK������ֻ���ǵ͵�ƽ)
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	}

	//---���ݲ�����ʱ�ӱ���λ��
	if (ADS1256x->msgSPI.msgCPOL == 0)
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	}
	else
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	}
	
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;							//---��������
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;		//---ϵͳʱ��256��Ƶ
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;						//---��λ��ǰ
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;	//---Ӳ��CRC��ʹ��
	SPI_InitStruct.CRCPoly = 7;

	//---��ʼ����ѯ��ʽ��SPI
	SPITask_MHW_PollMode_Init(&(ADS1256x->msgSPI), SPI_InitStruct);

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SW_Init(ADS1256_HandlerType *ADS1256x)
{
	SPITask_DeInit(&(ADS1256x->msgSPI));
	
	//---Ӳ���˿ڵ�����---����ʵ��
	SPITask_MSW_GPIO_Init(&(ADS1256x->msgSPI));
	
	//---ʱ���ߵļ���
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_Init(ADS1256_HandlerType *ADS1256x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW)
{
	//---ʹ�õ�DHT11�Ķ˿�
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

	//---�жϳ�ʼ���ķ�ʽ
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

	//---ע��ms��ʱʱ��
	if (pFuncDelayms!=NULL)
	{
		ADS1256x->msgFuncDelayms = pFuncDelayms;
	}
	else
	{
		ADS1256x->msgFuncDelayms = DelayTask_ms;
	}

	//---ע��us��ʱ����
	if (pFuncDelayus!=NULL)
	{
		ADS1256x->msgSPI.msgFuncDelayus = pFuncDelayus;
	}
	else
	{
		ADS1256x->msgSPI.msgFuncDelayus = DelayTask_us;
	}

	//---ע��δ���
	ADS1256x->msgSPI.msgFuncTimeTick = pFuncTimerTick;

	//---����Ĭ�ϲ���
	return ADS1256_SPI_ConfigInit(ADS1256x);

	//return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ�
//////�������:
//////�������: 0---�ɹ�����0---ʧ��
//////˵		�������׼���ò��������DRDY�˿ڴӸߵ��͵����壬��Щ״̬�������Ӧ������
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WaitDRDY(ADS1256_HandlerType *ADS1256x)
{
	//---��ȡʱ���ǩ
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;

	//---��ȡ��ǰʱ�����
	if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
	{
		//nowTime = W25QXXx->msgSPI.msgFuncTick();
		oldTime = ADS1256x->msgSPI.msgFuncTimeTick();
	}

	//---����GPIO�Ƿ����
	if (ADS1256x->msgDRDY.msgGPIOPort == NULL)
	{
		return ERROR_5;
	}

	//---��ȡ������ɣ����ߵ�ƽ
	while (1)
	{
		if (GPIO_GET_STATE(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit) != 0)
		{
			break;
		}
		if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
		{
			//---��ǰʱ��
			nowTime = ADS1256x->msgSPI.msgFuncTimeTick();

			//---�жϵδ�ʱ�Ƿ����������
			if (nowTime < oldTime)
			{
				cnt = (0xFFFFFFFF - oldTime + nowTime);
			}
			else
			{
				cnt = nowTime - oldTime;
			}

			//---�ж��Ƿ�ʱ
			if (cnt > 100)
			{
				//---���ͷ�����ʱ����
				return ERROR_1;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 400000)
			{
				//---���ͷ�����ʱ����
				return ERROR_2;
			}
		}
	}

	//---��ȡ������ɣ����͵�ƽ
	while (1)
	{
		if (GPIO_GET_STATE(ADS1256x->msgDRDY.msgGPIOPort, ADS1256x->msgDRDY.msgGPIOBit) == 0)
		{
			break;
		}
		if (ADS1256x->msgSPI.msgFuncTimeTick != NULL)
		{
			//---��ǰʱ��
			nowTime = ADS1256x->msgSPI.msgFuncTimeTick();

			//---�жϵδ�ʱ�Ƿ����������
			if (nowTime < oldTime)
			{
				cnt = (0xFFFFFFFF - oldTime + nowTime);
			}
			else
			{
				cnt = nowTime - oldTime;
			}

			//---�ж��Ƿ�ʱ
			if (cnt > 100)
			{
				//---���ͷ�����ʱ����
				return ERROR_3;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 400000)
			{
				//---���ͷ�����ʱ����
				return ERROR_4;
			}
		}
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
UINT8_T ADS1256_SPI_SW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MSW_SendByteMSB(&(ADS1256x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_HW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal)
{
	//---���ݷ���
	return SPITask_MHW_PollMode_WriteByte(&(ADS1256x->msgSPI), cmd, pRVal);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WriteReg(ADS1256_HandlerType *ADS1256x, UINT8_T regID, UINT8_T regVal)
{
	UINT8_T _return = OK_0;
	//---��ȡ׼���ź�
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---У��׼���ź�
	if (_return == OK_0)
	{
		//---ʹ��д����
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---д�Ĵ���������, �����ͼĴ�����ַ
		_return=ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_WREG | regID, NULL);
		_return <<= 1;
		//---�Ĵ������� - 1, �˴�д1���Ĵ���
		_return|=ADS1256_SPI_SEND_CMD(ADS1256x, 0x00, NULL);
		_return <<= 1;
		//---���ͼĴ���ֵ
		_return|=ADS1256_SPI_SEND_CMD(ADS1256x, regVal, NULL);
	}
	//---��ʹ��ͨѶ����������㣬���ⷢ��ͨѶһֱʹ��
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadReg(ADS1256_HandlerType *ADS1256x, UINT8_T regAddr, UINT8_T *pRVal)
{
	UINT8_T _return = OK_0;
	//---��ȡ׼���ź�
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---У��׼���ź�
	if (_return == OK_0)
	{
		//---ʹ��д����
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---д�Ĵ���������, �����ͼĴ�����ַ
		_return=ADS1256_SPI_SEND_CMD( ADS1256x, ADS1256_CMD_RREG | ( regAddr & 0x0F ), NULL );
		_return <<= 1;
		//---�Ĵ������� - 1, �˴�д1���Ĵ���
		_return|=ADS1256_SPI_SEND_CMD( ADS1256x, 0x00, NULL );
		_return <<= 1;
		//---�����ӳٲ��ܶ�ȡоƬ��������
		ADS1256x->msgSPI.msgFuncDelayus( 10 );
		//---���Ĵ���ֵ
		_return|=ADS1256_SPI_SEND_CMD( ADS1256x, 0xFF, pRVal );
	}
	//---��ʹ��ͨѶ����������㣬���ⷢ��ͨѶһֱʹ��
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);

	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ����͵��ֽ�����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WriteCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd)
{
	UINT8_T _return = OK_0;
	//---��ȡ׼���ź�
	_return=ADS1256_SPI_WaitDRDY(ADS1256x);
	//---У��׼���ź�
	if (_return == OK_0)
	{
		//---ʹ��д����
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		//---��������
		_return=ADS1256_SPI_SEND_CMD( ADS1256x, cmd, NULL );
	}
	//---��ʹ��ͨѶ����������㣬���ⷢ��ͨѶһֱʹ��
	GPIO_OUT_1(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�Ӳ����λ�豸
//////�������:
//////�������:
//////˵		����
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

		//---��ȡĬ�ϵ�ת�����ʣ�Ĭ��ֵ��0xF0
		_return = ADS1256_SPI_ReadDRate( ADS1256x, &dRate );
		//---У�鸴λ�Ƿ�ɹ�
		if ((_return!=OK_0)||(dRate!=0xF0))
		{
			_return = ERROR_1;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�������λ�豸
//////�������:
//////�������:
//////˵		����ͨ����ȡĬ��ת�����ʼĴ�����ֵ���ж��Ƿ�δ��λ�ɹ�����λ֮��Ĭ��ֵ��0xF0
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SoftReset(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T _return = OK_0, dRate = 0;
	//---����������λ����
	_return=ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_REST);
	//---У�鸴λ�Ƿ�ɹ�
	if (_return==OK_0)
	{
		//---��ȡĬ�ϵ�ת�����ʣ�Ĭ��ֵ��0xF0
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
//////��		����
//////��		�ܣ���������ģʽ
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_Standby(ADS1256_HandlerType *ADS1256x)
{
	//---��������ģʽ����
	ADS1256x->msgSleepMode = 1;
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_STANDBY);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_WAKEUP(ADS1256_HandlerType *ADS1256x)
{
	//---���ͻ�������
	ADS1256x->msgSleepMode = 0;
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_WAKEUP);;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ֹͣ������������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SDATAC(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_WriteCmd(ADS1256x, ADS1256_CMD_SDATAC);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ���ȡ�豸��ID��Ϣ
//////�������:
//////�������:
//////˵		����ADS1256��ID��3
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadChipID(ADS1256_HandlerType *ADS1256x,UINT8_T *pDeviceID)
{
	UINT8_T _return = OK_0;
	_return=ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_STATUS_ADDR, pDeviceID);
	//---����4λ
	*pDeviceID >>= 4;
	//---�ж��豸��ID�Ƿ����
	if (*pDeviceID!=0x03)
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ���ʹ���Զ�У׼
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�ʹ���Զ�У׼
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�����ͨ���š���·���á�AIN- �̶��ӵأ�ACOM)����������
//////�������: ch : ͨ���ţ� 0-7
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetSingleChannal(ADS1256_HandlerType *ADS1256x, UINT8_T ch)
{
	if (ch > 7)
	{
		return ERROR_1;
	}
	//---��ǰ���õ�ͨ��
	ADS1256x->msgNowChannel = ch;
	ADS1256x->msgChannelMode[ch] = 0;
	//---Bit3 = 1, AINN �̶��� AINCOM
	return ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (ch << 4) | (1 << 3));
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��������
//////�������:
//////�������:
//////˵		����
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
		//---������� AIN0�� AIN1
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (0 << 4) | 1);	/* ������� AIN0�� AIN1 */
	}
	else if ((ch == 1)||(ch==5))
	{
		if (ch==5)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---������� AIN2�� AIN3
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (2 << 4) | 3);	/* ������� AIN2�� AIN3 */
	}
	else if ((ch == 2)||(ch==6))
	{
		if (ch == 6)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---������� AIN4�� AIN5
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (4 << 4) | 5);	/* ������� AIN4�� AIN5 */
	}
	else if ((ch == 3)||(ch==7))
	{
		if (ch == 7)
		{
			ch -= 4;
		}
		ch <<= 1;
		//---������� AIN6�� AIN7
		_return = ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_MUX_ADDR, (6 << 4) | 7);	/* ������� AIN6�� AIN7 */
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
//////��		����
//////��		�ܣ�����ͨ����ģʽ
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�����ʱ�����������
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�������������
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ���ȡ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadGain(ADS1256_HandlerType *ADS1256x, UINT8_T *pGain)
{
	UINT8_T _return = OK_0;
	_return = ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_ADCON_ADDR, pGain);
	*pGain &= 0x78;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ���������
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ���ȡAD����ת������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadDRate(ADS1256_HandlerType *ADS1256x, UINT8_T *pDRate)
{
	return ADS1256_SPI_ReadReg(ADS1256x, ADS1256_REG_DRATE_ADDR, pDRate);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����AD����ת������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SetDRate(ADS1256_HandlerType *ADS1256x, UINT8_T rate)
{
	ADS1256x->msgDRate = rate;
	return ADS1256_SPI_WriteReg(ADS1256x, ADS1256_REG_DRATE_ADDR, rate);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�����GPIO�����ʽ������ģʽ�������ģʽ
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ���У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_Calibration(ADS1256_HandlerType *ADS1256x, UINT8_T calibCmdReg)
{
	//---����У׼����
	ADS1256_SPI_WriteCmd(ADS1256x, calibCmdReg);
	//---�ȴ�У׼���
	return ADS1256_SPI_WaitDRDY(ADS1256x);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ƫ�ƺ�������У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ƫ����У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_OffsetSelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFOCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�������У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_GainSelfCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SELFGCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ϵͳƫ����У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SysOffsetCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SYSOCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ϵͳ������У׼
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_SysGainCalibration(ADS1256_HandlerType *ADS1256x)
{
	return ADS1256_SPI_Calibration(ADS1256x, ADS1256_CMD_SYSGCAL);
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��ȴ�ADCװ��������
//////�������:
//////�������:
//////˵		����
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
//////��		����
//////��		�ܣ���ȡADCͨ��ת���Ľ��
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ReadChannelResult(ADS1256_HandlerType *ADS1256x, UINT8_T ch)
{
	UINT8_T _return = OK_0;
	UINT8_T temp[3] = { 0 };

	//---��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	if (ch < 7)
	{
		if (ADS1256x->msgChannelMode[ch] == 0)
		{
			//---���õ�������ͨ��
			ADS1256_SPI_SetSingleChannal(ADS1256x, ch);
		}
		else
		{
			//---���õ�������ͨ��
			ADS1256_SPI_SetDifferenceChannal(ADS1256x, ch);
		}
		
		//---��ʱ�ȴ�ת�����,������ת��ʱ�䳤�����ⷢ����ʱ����
		if (ADS1256x->msgDRate< ADS1256_DRATE_15SPS)
		{
			ADS1256x->msgFuncDelayms(0x100- ADS1256x->msgDRate);
		}
		
		//---���������Ƿ�׼�����
		_return=ADS1256_SPI_WaitDRDY(ADS1256x);

		//---ʹ��ͨѶ
		GPIO_OUT_0(ADS1256x->msgSPI.msgCS.msgGPIOPort, ADS1256x->msgSPI.msgCS.msgGPIOBit);
		
		//---�ж��ǲ��ǳ�������ģʽ
		if (ADS1256x->msgSleepMode!=0)
		{
			//---��������
			ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_SYNC, NULL);

			//---�����ӳٲ��ܶ�ȡоƬ��������
			if (ADS1256x->msgSPI.msgFuncDelayus != NULL)
			{
				ADS1256x->msgSPI.msgFuncDelayus(5);
			}
			ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_WAKEUP, NULL);
		}
		
		//---�����ӳٲ��ܶ�ȡоƬ��������
		ADS1256x->msgSPI.msgFuncDelayus(5);

		ADS1256_SPI_SEND_CMD(ADS1256x, ADS1256_CMD_RDATA, NULL);

		//---�����ӳٲ��ܶ�ȡоƬ��������
		ADS1256x->msgSPI.msgFuncDelayus(25);

		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[0]);
		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[1]);
		ADS1256_SPI_SEND_CMD(ADS1256x, 0xFF, &temp[2]);

		//---��ʹ��ͨѶ
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

		//---������һ������ͨ��ʱ�򣬶�ȡ��ADC���
		ADS1256x->msgOldChannel = ADS1256x->msgNowChannel;
	}
	return _return;
}
///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�У���豸�������豸�����쳣�����ڳ�ʼ��ȷ��
//////�������:
//////�������: 0---�豸��ʼ����������0---�豸��ʼ���쳣
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_CheckDevice(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T tempDRate = 0;
	UINT8_T _return = OK_0;
	//---��ȡ����ת������
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
//////��		����
//////��		�ܣ�ADS1256������
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T ADS1256_SPI_ConfigInit(ADS1256_HandlerType *ADS1256x)
{
	UINT8_T _return = OK_0;
	_return=ADS1256_SPI_SoftReset(ADS1256x);
	//---�ж��Ƿ�λ���
	if (_return!=OK_0)
	{
		_return = ERROR_1;
		goto GoToExit;
	}
	//---����ʱ��
	_return = ADS1256_SPI_SetDRate(ADS1256x, ADS1256_DRATE_2000SPS);
	//---У��ʱ�ӵ�����
	_return = ADS1256_SPI_CheckDevice(ADS1256x);
	//---�ж�ʱ���Ƿ�������ȷ
	if (_return!=OK_0)
	{
		_return = ERROR_2;
		goto GoToExit;
	}
	//---ʹ���Զ�У׼
	_return= ADS1256_SPI_EnabledAutoCalibration(ADS1256x);
	if (_return!=OK_0)
	{
		_return = ERROR_3;
		goto GoToExit;
	}
	//---��������
	_return = ADS1256_SPI_SetGain(ADS1256x, ADS1256_ADCON_GAIN_1);
	if (_return!=OK_0)
	{
		_return = ERROR_4;
		goto GoToExit;
	}
	//---ʹ����У׼
	_return = ADS1256_SPI_SelfCalibration(ADS1256x);
	if (_return!=OK_0)
	{
		_return = ERROR_5;
	}
	GoToExit:
	return _return; 
}