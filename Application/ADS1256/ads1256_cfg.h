#ifndef ADS1256_CFG_H_
#define ADS1256_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "spi_task.h"
	#include "delay_task.h"
	#include "gpio_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//---ADS1256------MCU
	//---RST----------NC
	//---DRDY---------GPIO(����)
	//---CS-----------SPI_CS(����)
	//---CLK----------SPI_CLK(����)
	//---DIN----------SPI_MOSI(����)
	//---DOUT---------SPI_MISO(����)
	//////////////////////////////////////////////////////////////////////////////////////
	//===�˳�˯��ģʽ
	#define ADS1256_CMD_WAKEUP							0x00
	//===���ζ�ȡ����
	#define ADS1256_CMD_RDATA							0x01
	//===������ȡ����
	#define ADS1256_CMD_RDATAC							0x03
	//===ֹͣ����������
	#define ADS1256_CMD_SDATAC							0x0F
	//===���Ĵ���
	#define ADS1256_CMD_RREG							0x10
	//===д�Ĵ���
	#define ADS1256_CMD_WREG							0x50
	//===ϵͳ��У׼
	#define ADS1256_CMD_SELFCAL							0xF0
	//===ϵͳƫ����У׼
	#define ADS1256_CMD_SELFOCAL						0xF1
	//===ϵͳ������У׼
	#define ADS1256_CMD_SELFGCAL						0xF2
	//===ϵͳƫ��У׼
	#define ADS1256_CMD_SYSOCAL							0xF3
	//===ϵͳ����У׼
	#define ADS1256_CMD_SYSGCAL							0xF4
	//===ͬ��ADCת��
	#define ADS1256_CMD_SYNC							0xFC
	//===��������ģʽ
	#define ADS1256_CMD_STANDBY							0xFD
	//===�����λ�豸
	#define ADS1256_CMD_REST							0xFE
	//===�˳�˯��ģʽ
	//#defineADS1256_CMD_WAKEUP							0xFF
	
	//===�Ĵ�����ӳ���ַ
	#define ADS1256_REG_STATUS_ADDR						0x00		//[x1]
	#define ADS1256_REG_MUX_ADDR						0x01		//[01]
	#define ADS1256_REG_ADCON_ADDR						0x02		//[20]
	#define ADS1256_REG_DRATE_ADDR						0x03		//[F0]
	#define ADS1256_REG_GPIO_ADDR						0x04		//[E0]
	#define ADS1256_REG_OFC0_ADDR						0x05
	#define ADS1256_REG_OFC1_ADDR						0x06
	#define ADS1256_REG_OFC2_ADDR						0x07
	#define ADS1256_REG_FSC0_ADDR						0x08
	#define ADS1256_REG_FSC1_ADDR						0x09
	#define ADS1256_REG_FSC2_ADDR						0x0A
	
	//===������������
	#define ADS1256_MUXP_AIN0							0x00
	#define ADS1256_MUXP_AIN1							0x10
	#define ADS1256_MUXP_AIN2							0x20
	#define ADS1256_MUXP_AIN3							0x30
	#define ADS1256_MUXP_AIN4							0x40
	#define ADS1256_MUXP_AIN5							0x50
	#define ADS1256_MUXP_AIN6							0x60
	#define ADS1256_MUXP_AIN7							0x70
	#define ADS1256_MUXP_AINCOM							0x80
	
	//===�������ĸ���
	#define ADS1256_MUXN_AIN0							0x00
	#define ADS1256_MUXN_AIN1							0x01
	#define ADS1256_MUXN_AIN2							0x02
	#define ADS1256_MUXN_AIN3							0x03
	#define ADS1256_MUXN_AIN4							0x04
	#define ADS1256_MUXN_AIN5							0x05
	#define ADS1256_MUXN_AIN6							0x06
	#define ADS1256_MUXN_AIN7							0x07
	#define ADS1256_MUXN_AINCOM							0x08
	
	//===ϵͳʱ�ӵ��������
	#define ADS1256_ADCON_CLK_OFF						(0<<5)
	#define ADS1256_ADCON_CLK_1							(1<<5)
	#define ADS1256_ADCON_CLK_2							(2<<5)
	#define ADS1256_ADCON_CLK_4							(3<<5)

	//===������������
	#define ADS1256_ADCON_SENSOR_OFF					(0<<3)
	#define ADS1256_ADCON_SENSOR_0P5UA					(1<<3)
	#define ADS1256_ADCON_SENSOR_2UA					(2<<3)
	#define ADS1256_ADCON_SENSOR_10UA					(3<<3)
	
	//===��������
	#define ADS1256_ADCON_GAIN_1						0x00
	#define ADS1256_ADCON_GAIN_2						0x01
	#define ADS1256_ADCON_GAIN_4						0x02
	#define ADS1256_ADCON_GAIN_8						0x03
	#define ADS1256_ADCON_GAIN_16						0x04
	#define ADS1256_ADCON_GAIN_32						0x05
	#define ADS1256_ADCON_GAIN_64						0x06
	//#define ADS1256_ADCON_GAIN_64						0x07
	
	//===������������
	#define ADS1256_REG_DRATE_MASK						0xF0
	#define ADS1256_DRATE_30000SPS						0xF0
	#define ADS1256_DRATE_15000SPS						0xE0
	#define ADS1256_DRATE_7500SPS						0xD0
	#define ADS1256_DRATE_3750SPS						0xC0
	#define ADS1256_DRATE_2000SPS						0xB0
	#define ADS1256_DRATE_1000SPS						0xA1
	#define ADS1256_DRATE_500SPS						0x92
	#define ADS1256_DRATE_100SPS						0x82
	#define ADS1256_DRATE_60SPS							0x72
	#define ADS1256_DRATE_50SPS							0x63
	#define ADS1256_DRATE_30SPS							0x53
	#define ADS1256_DRATE_25SPS							0x43
	#define ADS1256_DRATE_15SPS							0x33
	#define ADS1256_DRATE_10SPS							0x23
	#define ADS1256_DRATE_5SPS							0x13
	#define ADS1256_DRATE_2P5SPS						0x03

	//===GPIO3����/���������
	#define ADS1256_GPIO3_MODE_OUTPUT					0x7F
	#define ADS1256_GPIO3_MODE_INPUT					0x80
	//===GPIO2����/���������
	#define ADS1256_GPIO2_MODE_OUTPUT					0xBF
	#define ADS1256_GPIO2_MODE_INPUT					0x40
	//===GPIO1����/���������
	#define ADS1256_GPIO1_MODE_OUTPUT					0xDF
	#define ADS1256_GPIO1_MODE_INPUT					0x20
	//===GPIO0����/���������
	#define ADS1256_GPIO0_MODE_OUTPUT					0xEF
	#define ADS1256_GPIO0_MODE_INPUT					0x10

	//===�ο���ѹ
	#define ADS1256_REF_MV								25000
	//===Buffer�ر����ɼ���ѹ
	#define ADS1256_BUFFER_OFF_SAMPLE_RANGE_MV			( ADS1256_REF_MV<<1 )
	//===Buffer�������ɼ���ѹ
	#define ADS1256_BUFFER_ON_SAMPLE_RANGE_MV			( ADS1256_BUFFER_OFF_INPUT_RANGE_MV-2000)

	//===����ṹ��
	typedef struct _ADS1256_HandlerType					ADS1256_HandlerType;

	//===����ָ��ṹ��
	typedef	struct _ADS1256_HandlerType					*pADS1256_HandlerType;

	//===����ṹ��
	struct _ADS1256_HandlerType
	{
		UINT8_T	msgGain;							//---��������
		UINT8_T msgNowChannel;						//---��ǰת����ͨ��
		UINT8_T msgOldChannel;						//---��һ��ת����ͨ��
		UINT8_T msgChannelMode[8];					//---����װ��ģʽ,0---����ģʽ��1---���ģʽ
		INT32_T	msgChannelResult[8];				//---ADCת�����
		UINT8_T msgDelayms;							//---�ȴ�ʱ��,��λ��ms
		UINT8_T msgSleepMode;						//---����ģʽ��0---�����ߣ�1---����ģʽ
		UINT8_T msgDRate;							//---����ת�������ʣ�Ĭ����0xF0
		UINT8_T msgBufferON;						//---�Ƿ�����������0---��������1---����
		void(*msgFuncDelayms)(UINT32_T delay);		//---��ʱ����
		GPIO_HandlerType	msgDRDY;				//---׼�����ź�
		GPIO_HandlerType	msgHWRST;				//---Ӳ����λ�ź�
		SPI_HandlerType		msgSPI;					//---ʹ�õ�SPIģʽ
	};

	//===������
	#define ADS1256_TASK_ONE		pADS1256Device0
	#define ADS1256_TASK_TWO		0
	#define ADS1256_TASK_THREE		0
	
	//===�ⲿ���ýӿ�
	extern ADS1256_HandlerType  g_ADS1256Device0;
	extern pADS1256_HandlerType pADS1256Device0;

	//===��������
	UINT8_T ADS1256_SPI_Init(ADS1256_HandlerType *ADS1256x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T ADS1256_SPI_WaitDRDY(ADS1256_HandlerType *ADS1256x);
	UINT8_T ADS1256_SPI_SW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal);
	UINT8_T ADS1256_SPI_HW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal);
	UINT8_T ADS1256_SPI_WriteReg( ADS1256_HandlerType *ADS1256x, UINT8_T regID, UINT8_T regVal );
	UINT8_T ADS1256_SPI_ReadReg( ADS1256_HandlerType *ADS1256x, UINT8_T regAddr, UINT8_T *pRVal );
	UINT8_T ADS1256_SPI_WriteCmd( ADS1256_HandlerType *ADS1256x, UINT8_T cmd );
	UINT8_T ADS1256_SPI_HardReset( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_SoftReset( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_WAKEUP( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_SDATAC( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_ReadChipID( ADS1256_HandlerType *ADS1256x, UINT8_T *pDeviceID );
	UINT8_T ADS1256_SPI_ReadDRate( ADS1256_HandlerType *ADS1256x, UINT8_T *pDRate );
	UINT8_T ADS1256_SPI_SetDRate( ADS1256_HandlerType *ADS1256x, UINT8_T rate );
	UINT8_T ADS1256_SPI_ReadGain( ADS1256_HandlerType *ADS1256x, UINT8_T *pGain );
	UINT8_T ADS1256_SPI_SetGain( ADS1256_HandlerType *ADS1256x, UINT8_T gain );
	UINT8_T ADS1256_SPI_SetClockOutRate( ADS1256_HandlerType *ADS1256x, UINT8_T clockRate );
	UINT8_T ADS1256_SPI_DisableBuffer( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_EnableBuffer( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_Calibration( ADS1256_HandlerType *ADS1256x, UINT8_T calibCmdReg );
	UINT8_T ADS1256_SPI_SelfCalibration( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_DisabledAutoCalibration( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_EnabledAutoCalibration( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_SetSingleChannal( ADS1256_HandlerType *ADS1256x, UINT8_T ch );
	UINT8_T ADS1256_SPI_SetDifferenceChannal( ADS1256_HandlerType *ADS1256x, UINT8_T ch );
	UINT8_T ADS1256_SPI_SetChannalMode( ADS1256_HandlerType *ADS1256x, UINT8_T ch, UINT8_T isDiff );
	UINT8_T ADS1256_SPI_ReadChannelResult( ADS1256_HandlerType *ADS1256x, UINT8_T ch );
	UINT8_T ADS1256_SPI_CheckDevice(ADS1256_HandlerType *ADS1256x);
	UINT8_T ADS1256_SPI_ConfigInit(ADS1256_HandlerType *ADS1256x);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ADS1256_CFG_H_ */
