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
	#define ADS8688_CMD_REG_MO_OP				0x0000
	#define ADS8688_CMD_REG_STDBY				0x8200
	#define ADS8688_CMD_REG_PWR_DN				0x8300
	#define ADS8688_CMD_REG_RST					0x8500
	#define ADS8688_CMD_REG_AUTO_RST			0xA000
	#define ADS8688_CMD_REG_MAN_CH_0			0xC000
	#define ADS8688_CMD_REG_MAN_CH_1			0xC400
	#define ADS8688_CMD_REG_MAN_CH_2			0xC800
	#define ADS8688_CMD_REG_MAN_CH_3			0xCC00
	#define ADS8688_CMD_REG_MAN_CH_4			0xD000
	#define ADS8688_CMD_REG_MAN_CH_5			0xD400
	#define ADS8688_CMD_REG_MAN_CH_6			0xD800
	#define ADS8688_CMD_REG_MAN_CH_7			0xDC00
	#define ADS8688_CMD_REG_MAN_AUX				0xE000
	
	//===��̼Ĵ���
	#define ADS8688_PROG_REG_AUTO_SEQ_EN		0x01		//[FF]
	#define ADS8688_PROG_REG_CH_PWR_DN			0x02		//[00]
	#define ADS8688_PROG_REG_FEATURE_SELECT		0x03		//[00]
	#define ADS8688_PROG_REG_CH0_SET_RANGE		0x05		//[00]
	#define ADS8688_PROG_REG_CH1_SET_RANGE		0x06		//[00]
	#define ADS8688_PROG_REG_CH2_SET_RANGE		0x07		//[00]
	#define ADS8688_PROG_REG_CH3_SET_RANGE		0x08		//[00]
	#define ADS8688_PROG_REG_CH4_SET_RANGE		0x09		//[00]
	#define ADS8688_PROG_REG_CH5_SET_RANGE		0x0A		//[00]
	#define ADS8688_PROG_REG_CH6_SET_RANGE		0x0B		//[00]
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
	
	//===����������+/-10.24V
	#define ADS8688_RANGE_10240MV_10240MV		0x00
	
	//===����������+/-5.12V
	#define ADS8688_RANGE_5120MV_5120MV			0x01
	
	//===����������+/-2.56V
	#define ADS8688_RANGE_2560MV_2560MV			0x02
	
	//===����������0��10.24V
	#define ADS8688_RANGE_0MV_10240MV			0x05
	
	//===����������0��5.12V
	#define ADS8688_RANGE_0MV_5120MV			0x06
	
	//===����ṹ��
	typedef struct _ADS8688_HandlerType		ADS8688_HandlerType;

	//===����ָ��ṹ��
	typedef	struct _ADS8688_HandlerType		*pADS8688_HandlerType;

	//===����ṹ��
	struct _ADS8688_HandlerType
	{
		UINT8_T msgDelayms;							//---�ȴ�ʱ��,��λ��ms
		UINT16_T msgCHRange[8];						//---ADͨ�������̲ο�
		UINT16_T msgCHReseult[8];					//---ADͨ���Ĳ������
		void(*msgFuncDelayms)(UINT32_T delay);	//---��ʱ����
		SPI_HandlerType		msgSPI;					//---ʹ�õ�SPIģʽ
	};

	//===������
	#define ADS8688_TASK_ONE		pADS8688Device0
	#define ADS8688_TASK_TWO		0
	#define ADS8688_TASK_THREE		0
	
	//===�ⲿ���ýӿ�
	extern ADS8688_HandlerType  g_ADS8688Device0;
	extern pADS8688_HandlerType  pADS8688Device0;

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ADS8688_CFG_H_ */
