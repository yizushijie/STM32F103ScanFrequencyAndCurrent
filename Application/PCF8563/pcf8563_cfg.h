#ifndef PCF8563_CFG_H_
#define PCF8563_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===使用包含的头文件
	#include "complier_lib.h"
	#include "i2c_task.h"
	#include "delay_task.h"
	
	//===寄存器定义
	#define PCF8563_REG_STATUS1						0x00
	#define PCF8563_REG_STATUS2						0x01
	#define PCF8563_REG_SECOND						0x02
	#define PCF8563_REG_MINUTE						0x03
	#define PCF8563_REG_HOURS						0x04
	#define PCF8563_REG_DAY							0x05
	#define PCF8563_REG_WEEK						0x06
	#define PCF8563_REG_MONTH						0x07
	#define PCF8563_REG_YEAR						0x08
	#define PCF8563_REG_MINUTE_ALARM				0x09
	#define PCF8563_REG_HOURS_ALARM					0x0A
	#define PCF8563_REG_DAY_ALARM					0x0B
	#define PCF8563_REG_WEEK_ALARM					0x0C
	#define PCF8563_REG_CLKOUT						0x0D
	#define PCF8563_REG_TIMER_CTR					0x0E
	#define PCF8563_REG_TIMER 						0x0F
	
	//===设备地址
	#define PCF8563_WRITE_ADDR						0xA2
	#define PCF8563_READ_ADDR						0xA3
	
	//===CLKOUT的输出
	#define PCF8563_CLKOUT_32768HZ					0
	#define PCF8563_CLKOUT_1024HZ					1
	#define PCF8563_CLKOUT_32HZ						2
	#define PCF8563_CLKOUT_1HZ						3
	
	//===结构体定义
	typedef struct _PCF8563_HandlerType				PCF8563_HandlerType;

	//===指针结构体定义
	typedef struct _PCF8563_HandlerType				*pPCF8563_HandlerType;

	//===PCF853的数据结构体
	struct _PCF8563_HandlerType
	{
		RTC_HandlerType	msgRTC;						//---实时时钟
		I2C_HandlerType msgI2C;						//---使用的I2C
	};

	//===定义的任务函数
	#define PCF8563_TASK_ONE	pPCF8563Device0
	#define PCF8563_TASK_TWO	0
	#define PCF8563_TASK_THREE	0
	
	//===外部调用接口
	extern PCF8563_HandlerType g_PCF8563Device0;
	extern pPCF8563_HandlerType pPCF8563Device0;

	//===函数定义
	UINT8_T PCF8563_I2C_Init(PCF8563_HandlerType *PCF8563x, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C);
	UINT8_T PCF8563_I2C_DeInit(PCF8563_HandlerType *PCF8563x);
	UINT8_T PCF8563_SWI2C_WriteReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T val);
	UINT8_T PCF8563_HWI2C_WriteReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T val);
	UINT8_T PCF8563_SWI2C_ReadReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T *pVal, UINT16_T length);
	UINT8_T PCF8563_HWI2C_ReadReg(PCF8563_HandlerType *PCF8563x, UINT8_T reg, UINT8_T *pVal, UINT16_T length);
	UINT8_T PCF8563_I2C_Device0_Init(PCF8563_HandlerType *PCF8563HandlerType);
	UINT8_T PCF8563_I2C_Device1_Init(PCF8563_HandlerType *PCF8563HandlerType);
	UINT8_T PCF8563_I2C_Device2_Init(PCF8563_HandlerType *PCF8563HandlerType);
	UINT8_T PCF8563_I2C_ClockOut(PCF8563_HandlerType *PCF8563HandlerType, UINT8_T preVal);
	UINT8_T PCF8563_I2C_ReadRTC(PCF8563_HandlerType *PCF8563HandlerType);
	UINT8_T PCF8563_I2C_WriteRTC(PCF8563_HandlerType *PCF8563HandlerType, RTC_HandlerType rtc);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* PCF8563_CFG_H_ */