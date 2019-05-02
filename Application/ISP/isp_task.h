#ifndef ISP_TASK_H_
#define ISP_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "isp_lib.h"
//////////////////////////////////////////////////////////////////////////////////////
//===º¯Êý¶¨Òå
	UINT8_T ISPTask_Init(ISP_HandlerType *ISPx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));
	UINT8_T ISPTask_DeInit(ISP_HandlerType *ISPx);
	UINT8_T ISPTask_EnterProg(ISP_HandlerType *ISPx);
	UINT8_T ISPTask_ExitProg(ISP_HandlerType *ISPx);
	UINT8_T ISPTask_ReadReady(ISP_HandlerType *ISPx);
	UINT8_T ISPTask_EraseChip(ISP_HandlerType *ISPx);
	UINT8_T ISPTask_ReadChipID(ISP_HandlerType *ISPx, UINT8_T *pVal);
	UINT8_T ISPTask_ReadChipCalibration(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T length);
	UINT8_T ISPTask_ReadChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse);
	UINT8_T ISPTask_ReadChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal);
	UINT8_T ISPTask_ReadChipRom(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T addr, UINT16_T length);
	UINT8_T ISPTask_WriteChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse);
	UINT8_T ISPTask_WriteChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal);
	UINT8_T ISPTask_ReadChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPTask_ReadChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPTask_WriteChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPTask_WriteChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPTask_WriteChipEepromAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPTask_WriteChipEepromLongAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPTask_UpdateExternAddr(ISP_HandlerType *ISPx, UINT8_T addr);
	UINT8_T ISPTask_UpdateExternLongAddr(ISP_HandlerType *ISPx, UINT32_T addr);
	UINT8_T ISPTask_ReadChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPTask_ReadChipFlashLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT32_T addr, UINT16_T length);
	UINT8_T ISPTask_UpdateChipFlashBuffer(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T index, UINT16_T length);
	UINT8_T ISPTask_UpdateChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr);
	UINT8_T ISPTask_UpdateChipFlashLongAddr(ISP_HandlerType *ISPx, UINT32_T addr);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ISP_TASK_H_ */
