#ifndef FLASH_CFG_H_
#define FLASH_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===FLASH的起始地址 */
	#define STM32_FLASH_BASE			FLASH_BASE
	
	//===所选STM32的FLASH 容量大小(单位为K)
	#if defined(FLASH_BANK2_END)
		#define STM32_FLASH_SIZE		( (FLASH_BANK2_END-STM32_FLASH_BASE+1)/1024 )
	#elif defined(FLASH_BANK1_END)
		#define STM32_FLASH_SIZE		( (FLASH_BANK1_END-STM32_FLASH_BASE+1)/1024  )
	#else
		#define STM32_FLASH_SIZE		( (FLASH_END-STM32_FLASH_BASE+1)/1024  )
	#endif
	
	//===计算扇区
	#if STM32_FLASH_SIZE<256
		#define STM32_SECTOR_SIZE		1024
		#define STM32_SECTOR_NUM		STM32_FLASH_SIZE
	#else
		#define STM32_SECTOR_SIZE		2048
		#define STM32_SECTOR_NUM		( STM32_FLASH_SIZE/2 )
	#endif

	//===函数定义
	void FLASH_ReadHalfWord(UINT32_T readAddr, UINT16_T *pVal, UINT16_T length);
	void FLASH_WriteHalfWord(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length);
	void FLASH_WriteHalfWordWithErase(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length);
	void Flash_LockRead(void);
	void Flash_UnLockRead(void);
	UINT32_T Flash_GetSize(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*FLASH_CFG_H_ */