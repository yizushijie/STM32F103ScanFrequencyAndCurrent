#ifndef DS18B20_CFG_H_
#define DS18B20_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "complier_lib.h"
#ifdef USE_MCU_STM32
#include "delay_task.h"
#endif
#include "one_wire_task.h"
//////////////////////////////////////////////////////////////////////////////////////
//===定义结构体
	typedef struct _DS18B20_HandlerType				DS18B20_HandlerType;

	//===定义指针结构
	typedef struct _DS18B20_HandlerType				*pDS18B20_HandlerType;

	//===结构定义
	struct _DS18B20_HandlerType
	{
		UINT8_T					msgIsPositive;						//0---无数据，1---是负数，2---是正值
		UINT16_T				msgTempX100;						//---温度的值，放大100倍的值
		UINT32_T				msgTempX10000;						//---温度的值，放大1000倍的值
		OneWire_HandlerType		msgOneWire;							//---一线通信
		void(*msgFuncDelayms)(UINT32_T delay);						//---ms延时函数
	};

	//===定义的任务函数
#define DS18B20_TASK_ONE	pDS18B20Device0
#define DS18B20_TASK_TWO	0
#define DS18B20_TASK_THREE	0

//===外部调用接口
	extern DS18B20_HandlerType g_DS18B20Device0;
	extern pDS18B20_HandlerType pDS18B20Device0;

	//===函数的定义
	UINT8_T  DS18B20_OneWire_Init(DS18B20_HandlerType *DS18B20x, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay));
	UINT8_T  DS18B20_OneWire_Device0_Init(DS18B20_HandlerType *DS18B20x);
	UINT8_T  DS18B20_OneWire_Device1_Init(DS18B20_HandlerType *DS18B20x);
	UINT8_T  DS18B20_OneWire_Device2_Init(DS18B20_HandlerType *DS18B20x);
	UINT8_T  DS18B20_OneWire_DeInit(DS18B20_HandlerType *DS18B20x);
	UINT8_T  DS18B20_OneWire_ReadDeviceID(DS18B20_HandlerType *DS18B20x, UINT8_T *_id);
	UINT16_T DS18B20_OneWire_ReadTemp(DS18B20_HandlerType *DS18B20x);
	UINT16_T DS18B20_OneWire_ReadTempByID(DS18B20_HandlerType *DS18B20x, UINT8_T *_id);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*DS18B20_CFG_H_ */
