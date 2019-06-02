#ifndef W25QXX_LIB_H_
#define W25QXX_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "w25qxx_cfg.h"
//////////////////////////////////////////////////////////////////////////////////////
//===º¯Êý¶¨Òå
	UINT8_T W25QXXLib_SPI_Init(W25QXX_HandlerType *W25QXXx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T W25QXXLib_SPI_ReadRegSR1(W25QXX_HandlerType *W25QXXx);
	UINT8_T W25QXXLib_SPI_ReadRegSR2(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_WriteRegSR1(W25QXX_HandlerType *W25QXXx, UINT8_T cmd);
	void W25QXXLib_SPI_WriteRegSR2(W25QXX_HandlerType *W25QXXx, UINT8_T cmd);
	void W25QXXLib_SPI_EnableWrite(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_DisableWrite(W25QXX_HandlerType *W25QXXx);
	UINT16_T W25QXXLib_SPI_ReadID(W25QXX_HandlerType *W25QXXx);
	UINT8_T W25QXXLib_SPI_WaitBusy(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_ReadData(W25QXX_HandlerType *W25QXXx, UINT32_T addr, UINT8_T *pVal, UINT16_T length);
	void W25QXXLib_SPI_ReadFast(W25QXX_HandlerType *W25QXXx, UINT32_T addr, UINT8_T *pVal, UINT16_T length);
	void W25QXXLib_SPI_WritePage(W25QXX_HandlerType *W25QXXx, UINT32_T addr, UINT8_T *pVal, UINT16_T length);
	void W25QXXLib_SPI_EraseChip(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_EraseSector(W25QXX_HandlerType *W25QXXx, UINT32_T sectorAddr);
	void W25QXXLib_SPI_EraseBlock32KB(W25QXX_HandlerType *W25QXXx, UINT32_T blockAddr);
	void W25QXXLib_SPI_EraseBlock64KB(W25QXX_HandlerType *W25QXXx, UINT32_T blockAddr);
	void W25QXXLib_SPI_PowerDown(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_WakeUp(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_ReadUniqueIDNumber(W25QXX_HandlerType *W25QXXx, UINT8_T *pVal);
	void W25QXXLib_SPI_ReadJEDECID(W25QXX_HandlerType *W25QXXx, UINT8_T *pVal);
	void W25QXXLib_SPI_EraseSecurityReg(W25QXX_HandlerType *W25QXXx, UINT32_T regAddr);
	void W25QXXLib_SPI_ProgramSecurityReg(W25QXX_HandlerType *W25QXXx, UINT32_T regAddr, UINT8_T *pVal, UINT16_T length);
	void W25QXXLib_SPI_ReadSecurityReg(W25QXX_HandlerType *W25QXXx, UINT32_T regAddr, UINT8_T *pVal, UINT16_T length);
	void W25QXXLib_SPI_EnableReset(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_Reset(W25QXX_HandlerType *W25QXXx);
	void W25QXXLib_SPI_OnlyWrite(W25QXX_HandlerType *W25QXXx, UINT32_T addr, UINT8_T* pVal, UINT16_T length);
	void W25QXXLib_SPI_CheckWrite(W25QXX_HandlerType *W25QXXx, UINT32_T addr, UINT8_T* pVal, UINT16_T length);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*W25QXX_LIB_H_ */
