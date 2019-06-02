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
	//---RST----------NC(为了保证复位的彻底性，最好加上)
	//---DRDY---------GPIO(必须)  PA3
	//---CS-----------SPI_CS(必须) PA4
	//---CLK----------SPI_CLK(必须) PA5
	//---DIN----------SPI_MOSI(必须) PA7
	//---DOUT---------SPI_MISO(必须) PA6
	//---自动量产切换一次之后，耗时113ms
	//////////////////////////////////////////////////////////////////////////////////////
	//===退出睡眠模式
	#define ADS1256_CMD_WAKEUP							0x00
	//===单次读取数据
	#define ADS1256_CMD_RDATA							0x01
	//===连续读取数据
	#define ADS1256_CMD_RDATAC							0x03
	//===停止连续读数据
	#define ADS1256_CMD_SDATAC							0x0F
	//===读寄存器
	#define ADS1256_CMD_RREG							0x10
	//===写寄存器
	#define ADS1256_CMD_WREG							0x50
	//===系统自校准
	#define ADS1256_CMD_SELFCAL							0xF0
	//===系统偏移自校准
	#define ADS1256_CMD_SELFOCAL						0xF1
	//===系统增益自校准
	#define ADS1256_CMD_SELFGCAL						0xF2
	//===系统偏移校准
	#define ADS1256_CMD_SYSOCAL							0xF3
	//===系统增益校准
	#define ADS1256_CMD_SYSGCAL							0xF4
	//===同步ADC转换
	#define ADS1256_CMD_SYNC							0xFC
	//===进入休眠模式
	#define ADS1256_CMD_STANDBY							0xFD
	//===软件复位设备
	#define ADS1256_CMD_REST							0xFE
	//===退出睡眠模式
	//#defineADS1256_CMD_WAKEUP							0xFF
	
	//===寄存器的映射地址
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
	
	//===差分输入的正端
	#define ADS1256_MUXP_AIN0							0x00
	#define ADS1256_MUXP_AIN1							0x10
	#define ADS1256_MUXP_AIN2							0x20
	#define ADS1256_MUXP_AIN3							0x30
	#define ADS1256_MUXP_AIN4							0x40
	#define ADS1256_MUXP_AIN5							0x50
	#define ADS1256_MUXP_AIN6							0x60
	#define ADS1256_MUXP_AIN7							0x70
	#define ADS1256_MUXP_AINCOM							0x80
	
	//===差分输入的负端
	#define ADS1256_MUXN_AIN0							0x00
	#define ADS1256_MUXN_AIN1							0x01
	#define ADS1256_MUXN_AIN2							0x02
	#define ADS1256_MUXN_AIN3							0x03
	#define ADS1256_MUXN_AIN4							0x04
	#define ADS1256_MUXN_AIN5							0x05
	#define ADS1256_MUXN_AIN6							0x06
	#define ADS1256_MUXN_AIN7							0x07
	#define ADS1256_MUXN_AINCOM							0x08
	
	//===系统时钟的输出配置
	#define ADS1256_ADCON_CLK_OFF						(0<<5)
	#define ADS1256_ADCON_CLK_1							(1<<5)
	#define ADS1256_ADCON_CLK_2							(2<<5)
	#define ADS1256_ADCON_CLK_4							(3<<5)

	//===传感器检测电流
	#define ADS1256_ADCON_SENSOR_OFF					(0<<3)
	#define ADS1256_ADCON_SENSOR_0P5UA					(1<<3)
	#define ADS1256_ADCON_SENSOR_2UA					(2<<3)
	#define ADS1256_ADCON_SENSOR_10UA					(3<<3)
	
	//===定义增益
	#define ADS1256_ADCON_GAIN_1						0x00
	#define ADS1256_ADCON_GAIN_2						0x01
	#define ADS1256_ADCON_GAIN_4						0x02
	#define ADS1256_ADCON_GAIN_8						0x03
	#define ADS1256_ADCON_GAIN_16						0x04
	#define ADS1256_ADCON_GAIN_32						0x05
	#define ADS1256_ADCON_GAIN_64						0x06
	//#define ADS1256_ADCON_GAIN_64						0x07
	
	//===定义数据速率
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

	//===GPIO3输入/输出的配置
	#define ADS1256_GPIO3_MODE_OUTPUT					0x7F
	#define ADS1256_GPIO3_MODE_INPUT					0x80
	//===GPIO2输入/输出的配置
	#define ADS1256_GPIO2_MODE_OUTPUT					0xBF
	#define ADS1256_GPIO2_MODE_INPUT					0x40
	//===GPIO1输入/输出的配置
	#define ADS1256_GPIO1_MODE_OUTPUT					0xDF
	#define ADS1256_GPIO1_MODE_INPUT					0x20
	//===GPIO0输入/输出的配置
	#define ADS1256_GPIO0_MODE_OUTPUT					0xEF
	#define ADS1256_GPIO0_MODE_INPUT					0x10

	//===参考电压
	#define ADS1256_REF_MV								2500
	//===Buffer关闭最大采集电压
	#define ADS1256_BUFFER_OFF_SAMPLE_RANGE_MV			( ADS1256_REF_MV<<1 )
	//===Buffer开启最大采集电压
	#define ADS1256_BUFFER_ON_SAMPLE_RANGE_MV			( ADS1256_BUFFER_OFF_SAMPLE_RANGE_MV-2000)
	//===最大采集电压
	#define	ADS1256_GAIN_1_FULL_RANGE_UV				5000000
	//===最大采集电压
	#define	ADS1256_GAIN_2_FULL_RANGE_UV				(2500000)
	//===最大采集电压
	#define	ADS1256_GAIN_4_FULL_RANGE_UV				(1250000)
	//===最大采集电压
	#define	ADS1256_GAIN_8_FULL_RANGE_UV				(625000)
	//===最大采集电压
	#define	ADS1256_GAIN_16_FULL_RANGE_UV				(321500)
	//===最大采集电压
	#define	ADS1256_GAIN_32_FULL_RANGE_UV				(156250)
	//===最大采集电压
	#define	ADS1256_GAIN_64_FULL_RANGE_UV				(78125)
	//===最大采集电压
	#define	ADS1256_BUFFER_ON_FULL_RANGE_UV				( ADS1256_BUFFER_ON_SAMPLE_RANGE_MV*1000 )
	//===ADS1256的通道数
	#define ADS1256_CHANNEL_MAX							8
	//===是否使能ADS1256的校准功能
	#define ADS1256_SELF_CALIBRATION_ENABLE				0
	#define ADS1256_SELF_CALIBRATION_SPAN_TIME_MS		1000

	//===定义结构体
	typedef struct _ADS1256_HandlerType					ADS1256_HandlerType;

	//===定义指针结构体
	typedef	struct _ADS1256_HandlerType					*pADS1256_HandlerType;

	//===定义结构体
	struct _ADS1256_HandlerType
	{
		UINT8_T msgReady;															//---设备是否准备就绪，0---工作正常；1---工作异常
		UINT8_T msgChipID;															//---设备的ID信息
		UINT8_T	msgGain;															//---增益设置
		UINT8_T msgCalcError[ADS1256_CHANNEL_MAX];									//---是否需要校准偏差 0===不叫准，1---校准
		UINT8_T msgNowChannel;														//---当前转换的通道
		UINT8_T msgOldChannel;														//---上一次转换的通道
		UINT8_T	msgIsPositive[ADS1256_CHANNEL_MAX];									//---0---无数据，1---是负数，2---是正值
		UINT8_T msgChannelMode[ADS1256_CHANNEL_MAX];								//---输入装换模式,0---无配置；1---单端模式；2---差分模式
		UINT16_T msgChannelPowerError[ADS1256_CHANNEL_MAX];							//---通道的基础误差
		UINT16_T msgChannelPowerX1Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX2Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX4Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX8Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX16Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX32Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT16_T msgChannelPowerX64Error[ADS1256_CHANNEL_MAX];						//---通道的基础误差
		UINT32_T msgChannelNowPowerResult[ADS1256_CHANNEL_MAX];						//---当前ADC计算的电压结果
		UINT32_T msgChannelOldPowerResult[ADS1256_CHANNEL_MAX];						//---上一次ADC计算的电压结果
		UINT32_T msgChannelADCResult[ADS1256_CHANNEL_MAX];							//---ADC转换结果
		UINT8_T msgDelayms;															//---等待时间,单位是ms
		UINT8_T msgSleepMode;														//---休眠模式，0---不休眠，1---休眠模式
		UINT8_T msgDRate;															//---数据转换的速率，默认是0xF0
		UINT8_T msgBufferON;														//---是否开启缓存区，0---不开启，1---开启
		UINT32_T msgNowTime;														//---当前的时间
		void(*msgFuncDelayms)(UINT32_T delay);										//---延时参数
		GPIO_HandlerType	msgDRDY;												//---准备好信号
		GPIO_HandlerType	msgHWRST;												//---硬件复位信号
		SPI_HandlerType		msgSPI;													//---使用的SPI模式
	};

	//===任务函数
	#define ADS1256_TASK_ONE		pADS1256Device0
	#define ADS1256_TASK_TWO		0
	#define ADS1256_TASK_THREE		0
	
	//===外部调用接口
	extern ADS1256_HandlerType  	g_ADS1256Device0;
	extern pADS1256_HandlerType 	pADS1256Device0;

	//===函数定义
	UINT8_T ADS1256_SPI_Init(ADS1256_HandlerType *ADS1256x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T ADS1256_SPI_WaitDRDY(ADS1256_HandlerType *ADS1256x);
	UINT8_T ADS1256_SPI_SW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal);
	UINT8_T ADS1256_SPI_HW_SendCmd(ADS1256_HandlerType *ADS1256x, UINT8_T cmd, UINT8_T *pRVal);
	UINT8_T ADS1256_SPI_WriteReg( ADS1256_HandlerType *ADS1256x, UINT8_T regID, UINT8_T regVal );
	UINT8_T ADS1256_SPI_ReadReg( ADS1256_HandlerType *ADS1256x, UINT8_T regAddr, UINT8_T *pRVal );
	UINT8_T ADS1256_SPI_WriteCmd( ADS1256_HandlerType *ADS1256x, UINT8_T cmd );
	UINT8_T ADS1256_SPI_HardReset( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_SoftReset( ADS1256_HandlerType *ADS1256x );
	UINT8_T ADS1256_SPI_Reset(ADS1256_HandlerType* ADS1256x);
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
	UINT8_T ADS1256_SPI_CalcChannelPowerResult(ADS1256_HandlerType* ADS1256x, UINT8_T ch);
	UINT8_T ADS1256_SPI_CalcBaseError(ADS1256_HandlerType* ADS1256x, UINT8_T ch);
	UINT8_T ADS1256_SPI_CheckDevice(ADS1256_HandlerType *ADS1256x);
	UINT8_T ADS1256_SPI_ConfigInit(ADS1256_HandlerType *ADS1256x);
	UINT8_T ADS1256_SPI_AutoSelfRecovery(ADS1256_HandlerType* ADS1256x);
	UINT8_T ADS1256_SPI_AutoReadChannelResult(ADS1256_HandlerType* ADS1256x, UINT8_T ch);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ADS1256_CFG_H_ */
