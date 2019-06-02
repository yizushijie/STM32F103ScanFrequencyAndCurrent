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
	
	//===命令寄存器
	//===继续在以前的操作模式
	#define ADS8688_CMD_REG_NO_OP				0x0000
	//===设备被置于待机模式
	#define ADS8688_CMD_REG_STDBY				0x8200
	//===设备已关闭供电
	#define ADS8688_CMD_REG_PWR_DN				0x8300
	//===寄存器重置为默认
	#define ADS8688_CMD_REG_RST					0x8500
	//===启用自动模式下复位
	#define ADS8688_CMD_REG_AUTO_RST			0xA000
	//===通道0
	#define ADS8688_CMD_REG_MAN_CH_0			0xC000
	//===通道1
	#define ADS8688_CMD_REG_MAN_CH_1			0xC400
	//===通道2
	#define ADS8688_CMD_REG_MAN_CH_2			0xC800
	//===通道3
	#define ADS8688_CMD_REG_MAN_CH_3			0xCC00
	//===通道4
	#define ADS8688_CMD_REG_MAN_CH_4			0xD000
	//===通道5
	#define ADS8688_CMD_REG_MAN_CH_5			0xD400
	//===通道6
	#define ADS8688_CMD_REG_MAN_CH_6			0xD800
	//===通道7
	#define ADS8688_CMD_REG_MAN_CH_7			0xDC00
	//===AUX通道
	#define ADS8688_CMD_REG_MAN_AUX				0xE000
	
	//===编程寄存器
	//===自动Squencing启用：默认值为0xFF - BITX使CHX
	#define ADS8688_PROG_REG_AUTO_SEQ_EN		0x01		//[FF]
	//===通道掉电；默认为0x00 - BITX到CHX断电
	#define ADS8688_PROG_REG_CH_PWR_DN			0x02		//[00]
	//功能选择：默认为0x00;//菊花链ID为bit 7-6，ALARM功能为bit 4，SDO数据格式为bit 2-0位
	#define ADS8688_PROG_REG_FEATURE_SELECT		0x03		//[00]

	//===设置通道0输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH0_SET_RANGE		0x05		//[00]
	//===设置通道1输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH1_SET_RANGE		0x06		//[00]
	//===设置通道2输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH2_SET_RANGE		0x07		//[00]
	//===设置通道3输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH3_SET_RANGE		0x08		//[00]
	//===设置通道4输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH4_SET_RANGE		0x09		//[00]
	//===设置通道5输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH5_SET_RANGE		0x0A		//[00]
	//===设置通道6输入范围，默认为0x00 -位3-0选择范围
	#define ADS8688_PROG_REG_CH6_SET_RANGE		0x0B		//[00]
	//===设置通道7输入范围，默认为0x00 -位3-0选择范围
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
	
	//===命令读取
	#define ADS8688_CMD_READ_BACK				0x3F		//[00]
	
	//===输入范围-2.5 / + 2.5 Vref的+/- 10.24V
	#define ADS8688_RANGE_10240MV_10240MV		0x00
	//===输入范围-1.25 / + 1.25 Vref的+/- 5.12V
	#define ADS8688_RANGE_5120MV_5120MV			0x01
	//===输入范围到-0.625 / 0.625 + Vref的+/- 2.56V
	#define ADS8688_RANGE_2560MV_2560MV			0x02
	//===输入范围以2.5 Vref的10.24V
	#define ADS8688_RANGE_0MV_10240MV			0x05
	//===输入范围以1.25 Vref的5.12V
	#define ADS8688_RANGE_0MV_5120MV			0x06
	//===ADC的采样通道数
	#define	ADS8688_CHANNEL_MAX					8	
	
	//===定义结构体
	typedef struct _ADS8688_HandlerType		ADS8688_HandlerType;

	//===定义指针结构体
	typedef	struct _ADS8688_HandlerType		*pADS8688_HandlerType;

	//===定义结构体
	struct _ADS8688_HandlerType
	{
		UINT8_T  msgDelayms;												//---等待时间,单位是ms
		UINT8_T  msgChipID;													//---设备的ID信息
		UINT8_T  msgAutoSeqEn;												//---通过AUTO_SEQ_EN的设置探测设备的存在
		UINT8_T  msgChannelPWRDN;											//---通道掉电模式
		UINT8_T  msgFeature;												//---特性设置参数
		UINT8_T  msgChannelMode[ADS8688_CHANNEL_MAX];						//---输入装换模式,0---未选中使能；1---选中使能
		UINT8_T	 msgIsPositive[ADS8688_CHANNEL_MAX];						//---0---无数据，1---是负数，2---是正值
		UINT8_T  msgChannelRangeIsPositive[ADS8688_CHANNEL_MAX];			//---0---是正负量程，1---是正量程
		UINT8_T  msgChannelRange[ADS8688_CHANNEL_MAX];						//---AD通道的量程参考
		UINT32_T msgChannelRangeBaseUVX1000[ADS8688_CHANNEL_MAX];			//---AD通道的每个BIT代表的电压值，单位是uv的十倍
		UINT64_T msgChannelRangeFullUVX1000[ADS8688_CHANNEL_MAX];			//---AD通道的满量程的电压值，单位是uv的十倍
		UINT32_T msgChannelADCResult[ADS8688_CHANNEL_MAX];					//---AD通道的采样结果
		UINT32_T msgChannelPowerResult[ADS8688_CHANNEL_MAX];				//---AD通道的采样的电压结果
		void(*msgFuncDelayms)(UINT32_T delay);								//---延时参数
		SPI_HandlerType		msgSPI;											//---使用的SPI模式
		GPIO_HandlerType	msgHWRST;										//---硬件复位信号
	};

	//===任务函数
	#define ADS8688_TASK_ONE		pADS8688Device0
	#define ADS8688_TASK_TWO		0
	#define ADS8688_TASK_THREE		0
	
	//===外部调用接口
	extern ADS8688_HandlerType  g_ADS8688Device0;
	extern pADS8688_HandlerType  pADS8688Device0;

	//===函数定义
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
