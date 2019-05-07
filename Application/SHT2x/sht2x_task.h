#ifndef SHT2X_TASK_H_
#define SHT2X_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "sht2x_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T SHT2XTask_I2C_Init(SHT2X_HandlerType *SHT2x, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C);
	UINT8_T SHT2XTask_I2C_DeInit(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_SoftReset(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_GetSerialNumber(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_ReadUserReg(SHT2X_HandlerType *SHT2x, UINT8_T *pReg);
	UINT8_T SHT2XTask_I2C_WriteUserReg(SHT2X_HandlerType *SHT2x, UINT8_T reg);
	UINT8_T SHT2XTask_I2C_ReadTempPoll(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_ReadHumiPoll(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_ReadTempHM(SHT2X_HandlerType *SHT2x);
	UINT8_T SHT2XTask_I2C_ReadHumiHM(SHT2X_HandlerType *SHT2x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*SHT2X_TASK_H_ */