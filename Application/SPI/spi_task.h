#ifndef SPI_TASK_H_
#define SPI_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "spi_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===º¯Êý¶¨Òå
	UINT8_T SPITask_MHW_PollMode_Init(SPI_HandlerType *SPIx, LL_SPI_InitTypeDef SPI_InitStruct);
	UINT8_T SPITask_MHW_GPIO_Init(SPI_HandlerType *SPIx);
	UINT8_T SPITask_MSW_GPIO_Init(SPI_HandlerType *SPIx);
	UINT8_T SPITask_GPIO_DeInit(SPI_HandlerType *SPIx);
	UINT8_T SPITask_DeInit(SPI_HandlerType *SPIx);
	UINT8_T SPITask_MHW_PollMode_WriteByte(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPITask_MHW_PollMode_WriteData(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	UINT8_T SPITask_MSW_WriteBitMSB(SPI_HandlerType *SPIx, UINT8_T wVal);
	UINT8_T SPITask_MSW_ReadBitMSB(SPI_HandlerType *SPIx, UINT8_T *pRVal);
	UINT8_T SPITask_MSW_WriteBitLSB(SPI_HandlerType *SPIx, UINT8_T wVal);
	UINT8_T SPITask_MSW_ReadBitLSB(SPI_HandlerType *SPIx, UINT8_T *pRVal);
	UINT8_T SPITask_MSW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock);
	UINT8_T SPITask_MHW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock);
	UINT8_T SPITask_MSW_SendByteMSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPITask_MSW_SendByteLSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal);
	UINT8_T SPITask_MSW_SendDataMSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	UINT8_T SPITask_MSW_SendDataLSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*SPI_TASK_H_ */
