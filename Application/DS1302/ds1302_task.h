#ifndef DS1302_TASK_H_
#define DS1302_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "ds1302_lib.h"
//////////////////////////////////////////////////////////////////////////////////////
	UINT8_T DS1302Task_Init(DS1302_HandlerType *DS1302x, void(*pFuncDelayus)(UINT32_T delay));
	void DS1302Task_WriteReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat);
	void DS1302Task_ReadReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal);
	void DS1302Task_WriteTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat);
	void DS1302Task_ReadTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal);
	void DS1302Task_WriteRTC(DS1302_HandlerType *DS1302x, RTC_HandlerType rtcTime);
	void DS1302Task_ReadRTC(DS1302_HandlerType *DS1302x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*DS1302_TASK_H_ */
