#ifndef FLASH_TASK_H_
#define FLASH_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "flash_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===定义EEPROM使用的位置
	#if STM32_FLASH_SIZE<256
		#define	FLASH_BASE_ADDR	( STM32_FLASH_BASE+(STM32_SECTOR_NUM-2)*STM32_SECTOR_SIZE )
	#else
		#define	FLASH_BASE_ADDR	( STM32_FLASH_BASE+(STM32_SECTOR_NUM-1)*STM32_SECTOR_SIZE )
	#endif

	//===函数定义
	void FLASHTask_ReadHalfWord(UINT32_T readAddr, UINT16_T *pVal, UINT16_T length);
	void FLASHTask_WriteHalfWord(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length);
	void FLASHTask_WriteHalfWordWithErase(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length);
	void FlashTask_LockRead(void);
	void FlashTask_UnLockRead(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*FLASH_TASK_H_ */