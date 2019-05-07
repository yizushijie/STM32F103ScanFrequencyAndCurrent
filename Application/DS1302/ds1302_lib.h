#ifndef DS1302_LIB_H_
#define DS1302_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "ds1302_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T DS1302Lib_Init(DS1302_HandlerType *DS1302x, void(*pFuncDelayus)(UINT32_T delay));
	void DS1302Lib_WriteReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat);
	void DS1302Lib_ReadReg(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal);
	void DS1302Lib_WriteTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T dat);
	void DS1302Lib_ReadTime(DS1302_HandlerType *DS1302x, UINT8_T addr, UINT8_T *pVal);
	void DS1302Lib_WriteRTC(DS1302_HandlerType *DS1302x, RTC_HandlerType rtcTime);
	void DS1302Lib_ReadRTC(DS1302_HandlerType *DS1302x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*DS1302_LIB_H_ */
