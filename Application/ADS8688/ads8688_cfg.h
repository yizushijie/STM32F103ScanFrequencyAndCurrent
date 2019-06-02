#ifndef ADS8688_CFG_H_
#define ADS8688_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "delay_task.h"
	#include "gpio_task.h"
	#include "spi_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===����Ĵ���
	//===��������ǰ�Ĳ���ģʽ
	#define ADS8688_CMD_REG_NO_OP				0x0000
	//===�豸�����ڴ���ģʽ
	#define ADS8688_CMD_REG_STDBY				0x8200
	//===�豸�ѹرչ���
	#define ADS8688_CMD_REG_PWR_DN				0x8300
	//===�Ĵ�������ΪĬ��
	#define ADS8688_CMD_REG_RST					0x8500
	//===�����Զ�ģʽ�¸�λ
	#define ADS8688_CMD_REG_AUTO_RST			0xA000
	//===ͨ��0
	#define ADS8688_CMD_REG_MAN_CH_0			0xC000
	//===ͨ��1
	#define ADS8688_CMD_REG_MAN_CH_1			0xC400
	//===ͨ��2
	#define ADS8688_CMD_REG_MAN_CH_2			0xC800
	//===ͨ��3
	#define ADS8688_CMD_REG_MAN_CH_3			0xCC00
	//===ͨ��4
	#define ADS8688_CMD_REG_MAN_CH_4			0xD000
	//===ͨ��5
	#define ADS8688_CMD_REG_MAN_CH_5			0xD400
	//===ͨ��6
	#define ADS8688_CMD_REG_MAN_CH_6			0xD800
	//===ͨ��7
	#define ADS8688_CMD_REG_MAN_CH_7			0xDC00
	//===AUXͨ��
	#define ADS8688_CMD_REG_MAN_AUX				0xE000
	
	//===��̼Ĵ���
	//===�Զ�Squencing���ã�Ĭ��ֵΪ0xFF - BITXʹCHX
	#define ADS8688_PROG_REG_AUTO_SEQ_EN		0x01		//[FF]
	//===ͨ�����磻Ĭ��Ϊ0x00 - BITX��CHX�ϵ�
	#define ADS8688_PROG_REG_CH_PWR_DN			0x02		//[00]
	//����ѡ��Ĭ��Ϊ0x00;//�ջ���IDΪbit 7-6��ALARM����Ϊbit 4��SDO���ݸ�ʽΪbit 2-0λ
	#define ADS8688_PROG_REG_FEATURE_SELECT		0x03		//[00]

	//===����ͨ��0���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH0_SET_RANGE		0x05		//[00]
	//===����ͨ��1���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH1_SET_RANGE		0x06		//[00]
	//===����ͨ��2���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH2_SET_RANGE		0x07		//[00]
	//===����ͨ��3���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH3_SET_RANGE		0x08		//[00]
	//===����ͨ��4���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH4_SET_RANGE		0x09		//[00]
	//===����ͨ��5���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH5_SET_RANGE		0x0A		//[00]
	//===����ͨ��6���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH6_SET_RANGE		0x0B		//[00]
	//===����ͨ��7���뷶Χ��Ĭ��Ϊ0x00 -λ3-0ѡ��Χ
	#define ADS8688_PROG_REG_CH7_SET_RANGE		0x0C		//[00]

	#define ADS8688_PROG_REG_CH0_HS				0x15		//[00]
	#define ADS8688_PROG_REG_CH0_HT_MSB			0x16		//[FF]
	#define ADS8688_PROG_REG_CH0_HT_LSB			0x17		//[FF]
	#define ADS8688_PROG_REG_CH0_LT_MSB			0x18		//[00]
	#define ADS8688_PROG_REG_CH0_LT_LSB			0x19		//[00]

	#define ADS8688_PROG_REG_CH7_HS				0x38		//[00]
	#define ADS8688_PROG_REG_CH7_HT_MSB			0x39		//[FF]
	#define ADS8688_PROG_REG_CH7_HT_LSB			0x3A		//[FF]
	#define ADS8688_PROG_REG_CH7_LT_MSB			0x3B		//[00]
	#define ADS8688_PROG_REG_CH7_LT_LSB			0x3C		//[00]
	
	//===�����ȡ
	#define ADS8688_CMD_READ_BACK				0x3F		//[00]
	
	//===���뷶Χ-2.5 / + 2.5 Vref��+/- 10.24V
	#define ADS8688_RANGE_10240MV_10240MV		0x00
	//===���뷶Χ-1.25 / + 1.25 Vref��+/- 5.12V
	#define ADS8688_RANGE_5120MV_5120MV			0x01
	//===���뷶Χ��-0.625 / 0.625 + Vref��+/- 2.56V
	#define ADS8688_RANGE_2560MV_2560MV			0x02
	//===���뷶Χ��2.5 Vref��10.24V
	#define ADS8688_RANGE_0MV_10240MV			0x05
	//===���뷶Χ��1.25 Vref��5.12V
	#define ADS8688_RANGE_0MV_5120MV			0x06
	//===ADC�Ĳ���ͨ����
	#define	ADS8688_CHANNEL_MAX					8	
	
	//===����ṹ��
	typedef struct _ADS8688_HandlerType		ADS8688_HandlerType;

	//===����ָ��ṹ��
	typedef	struct _ADS8688_HandlerType		*pADS8688_HandlerType;

	//===����ṹ��
	struct _ADS8688_HandlerType
	{
		UINT8_T  msgDelayms;												//---�ȴ�ʱ��,��λ��ms
		UINT8_T  msgChipID;													//---�豸��ID��Ϣ
		UINT8_T  msgAutoSeqEn;												//---ͨ��AUTO_SEQ_EN������̽���豸�Ĵ���
		UINT8_T  msgChannelPWRDN;											//---ͨ������ģʽ
		UINT8_T  msgFeature;												//---�������ò���
		UINT8_T  msgChannelMode[ADS8688_CHANNEL_MAX];						//---����װ��ģʽ,0---δѡ��ʹ�ܣ�1---ѡ��ʹ��
		UINT8_T	 msgIsPositive[ADS8688_CHANNEL_MAX];						//---0---�����ݣ�1---�Ǹ�����2---����ֵ
		UINT8_T  msgChannelRangeIsPositive[ADS8688_CHANNEL_MAX];			//---0---���������̣�1---��������
		UINT8_T  msgChannelRange[ADS8688_CHANNEL_MAX];						//---ADͨ�������̲ο�
		UINT32_T msgChannelRangeBaseUVX1000[ADS8688_CHANNEL_MAX];			//---ADͨ����ÿ��BIT����ĵ�ѹֵ����λ��uv��ʮ��
		UINT64_T msgChannelRangeFullUVX1000[ADS8688_CHANNEL_MAX];			//---ADͨ���������̵ĵ�ѹֵ����λ��uv��ʮ��
		UINT32_T msgChannelADCResult[ADS8688_CHANNEL_MAX];					//---ADͨ���Ĳ������
		UINT32_T msgChannelPowerResult[ADS8688_CHANNEL_MAX];				//---ADͨ���Ĳ����ĵ�ѹ���
		void(*msgFuncDelayms)(UINT32_T delay);								//---��ʱ����
		SPI_HandlerType		msgSPI;											//---ʹ�õ�SPIģʽ
		GPIO_HandlerType	msgHWRST;										//---Ӳ����λ�ź�
	};

	//===������
	#define ADS8688_TASK_ONE		pADS8688Device0
	#define ADS8688_TASK_TWO		0
	#define ADS8688_TASK_THREE		0
	
	//===�ⲿ���ýӿ�
	extern ADS8688_HandlerType  g_ADS8688Device0;
	extern pADS8688_HandlerType  pADS8688Device0;

	//===��������
	UINT8_T ADS8688_SPI_Init(ADS8688_HandlerType* ADS8688x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T ADS8688_SPI_WriteCommandReg(ADS8688_HandlerType* ADS8688x, UINT32_T cmd);
	UINT8_T ADS8688_SPI_WriteProgramReg(ADS8688_HandlerType* ADS8688x, UINT8_T addr, UINT8_T val);
	UINT8_T ADS8688_SPI_ReadProgramReg(ADS8688_HandlerType* ADS8688x, UINT8_T addr, UINT8_T* pVal);
	UINT8_T ADS8688_SPI_AUTORST(ADS8688_HandlerType* ADS8688x);
	UINT8_T ADS8688_SPI_AUTOSEQEN(ADS8688_HandlerType* ADS8688x, UINT8_T seq);
	UINT8_T ADS8688_SPI_ChannelRangeSelect(ADS8688_HandlerType* ADS8688x, UINT8_T chReg, UINT8_T range);
	UINT8_T ADS8688_SPI_ChannelRange(ADS8688_HandlerType* ADS8688x, UINT8_T chIndex);
	UINT8_T ADS8688_SPI_Reset(ADS8688_HandlerType* ADS8688x);
	UINT8_T ADS8688_SPI_GetAutoRSTResult(ADS8688_HandlerType* ADS8688x, UINT8_T chNum);
	UINT8_T ADS8688_SPI_CalcChannelPower(ADS8688_HandlerType* ADS8688x, UINT8_T chIndex);
	UINT8_T ADS8688_SPI_ConfigInit(ADS8688_HandlerType* ADS8688x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ADS8688_CFG_H_ */
