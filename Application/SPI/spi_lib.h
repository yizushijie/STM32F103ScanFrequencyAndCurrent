#ifndef SPI_LIB_H_
#define SPI_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "spi_cfg.h"

//===º¯Êý¶¨Òå
	UINT8_T SPILib_MHW_PollMode_Init(SPI_HandlerType *SPIx, LL_SPI_InitTypeDef SPI_InitStruct);
	void SPILib_MHW_SetTransferBitOrder(SPI_HandlerType *SPIx, UINT32_T BitOrder);
	UINT8_T SPILib_MHW_GPIO_Init(SPI_HandlerType *SPIx);
	UINT8_T SPILib_MSW_GPIO_Init(SPI_HandlerType *SPIx);
	UINT8_T SPILib_GPIO_DeInit(SPI_HandlerType *SPIx);
	UINT8_T SPILib_DeInit(SPI_HandlerType *SPIx);
	UINT8_T SPILib_MHW_PollMode_WriteByte(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPILib_MHW_PollMode_WriteData(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	UINT8_T SPILib_MSW_WriteBitMSB(SPI_HandlerType *SPIx, UINT8_T wVal);
	UINT8_T SPILib_MSW_ReadBitMSB(SPI_HandlerType *SPIx, UINT8_T *pRVal);
	UINT8_T SPILib_MSW_WriteBitLSB(SPI_HandlerType *SPIx, UINT8_T wVal);
	UINT8_T SPILib_MSW_ReadBitLSB(SPI_HandlerType *SPIx, UINT8_T *pRVal);
	UINT8_T SPILib_MSW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock);
	UINT8_T SPILib_MHW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock);
	UINT8_T SPILib_MSW_SendByteMSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPILib_MSW_SendByteLSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPILib_MSW_SendDataMSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	UINT8_T SPILib_MSW_SendDataLSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*SPI_LIB_H_ */
