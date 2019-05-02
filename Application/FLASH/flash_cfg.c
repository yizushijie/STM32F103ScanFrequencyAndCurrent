#include "flash_cfg.h"

//===Flash缓存空间
UINT16_T g_FlashVal[STM32_SECTOR_SIZE / 2];

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void FLASH_ReadHalfWord(UINT32_T readAddr, UINT16_T *pVal, UINT16_T length)
{
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		pVal[i] = LL_FLASH_ReadHalfWord(readAddr);
		readAddr += 2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void FLASH_WriteHalfWord(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length)
{
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		if (pVal[i] != 0xFFFF)
		{
			LL_FLASH_WriteHalfWord(writeAddr, pVal[i]);
		}
		writeAddr += 2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void FLASH_WriteHalfWordWithErase(UINT32_T writeAddr, UINT16_T *pVal, UINT16_T length)
{
	//---扇区地址
	UINT32_T secpos = 0;

	//---扇区内偏移地址(16位字计算)
	UINT16_T secoff = 0;

	//---扇区内剩余地址(16位字计算)
	UINT16_T secremain = 0;
	UINT16_T i = 0;

	//---去掉0X08000000 后的地址
	UINT32_T offaddr = 0;

	//---地址的判断
	if (writeAddr < STM32_FLASH_BASE || (writeAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
	{
		//---非法地址
		return;
	}

	//---解锁
	LL_FLASH_Unlock();

	//---清理标志位
	LL_FLASH_ClearFlag();

	//---实际偏移地址
	offaddr = writeAddr - STM32_FLASH_BASE;

	//---扇区地址
	secpos = offaddr / STM32_SECTOR_SIZE;

	//---在扇区内的偏移(2个字节为基本单位)
	secoff = (offaddr%STM32_SECTOR_SIZE) / 2;

	//---扇区剩余空间大小
	secremain = STM32_SECTOR_SIZE / 2 - secoff;

	//---数据是否超出扇区
	if (length <= secremain)
	{
		//---待写入的数据总数不大于该扇区范围
		secremain = length;
	}
	while (1)
	{
		//---读出整个扇区的内容
		FLASH_ReadHalfWord((secpos*STM32_SECTOR_SIZE + STM32_FLASH_BASE), g_FlashVal, (STM32_SECTOR_SIZE / 2));

		//---数据校验
		for (i = 0; i < secremain; i++)
		{
			//---校验数据
			if (g_FlashVal[secoff + i] != 0xFFFF)
			{
				//---需要擦除
				break;
			}
		}

		//---需要擦除
		if (i < secremain)
		{
			//---擦除整个扇区,扇区本来的数据已保存在STMFLASH_BUF
			LL_FLASH_ErasePage((secpos*STM32_SECTOR_SIZE + STM32_FLASH_BASE));

			//---将待写入的数据复制到STMFLASH_BUF
			for (i = 0; i < secremain; i++)
			{
				g_FlashVal[i + secoff] = pVal[i];
			}

			//---写入整个扇区
			FLASH_WriteHalfWord((secpos*STM32_SECTOR_SIZE + STM32_FLASH_BASE), g_FlashVal, (STM32_SECTOR_SIZE / 2));
		}
		else
		{
			//---不需要擦除,直接写入扇区剩余区间
			FLASH_WriteHalfWord(writeAddr, pVal, secremain);
		}

		//---判断写入状态
		if (length == secremain)
		{
			//---写入结束了
			break;
		}

		//---写入未结束
		else
		{
			//---扇区地址增1
			secpos++;

			//---偏移位置为0
			secoff = 0;

			//---指针偏移
			pVal += secremain;

			//---写地址偏移(16位数据地址,需要*2)
			writeAddr += (secremain * 2);

			//---字节(16位)数递减
			length -= secremain;

			//---下一个扇区还是写不完
			if (length > (STM32_SECTOR_SIZE / 2))
			{
				secremain = STM32_SECTOR_SIZE / 2;
			}

			//---下一个扇区可以写完了
			else
			{
				secremain = length;
			}
		}

		//---喂狗
		WDT_RESET();
	};

	//---上锁
	LL_FLASH_Lock();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 锁定读取，使能读保护
//////////////////////////////////////////////////////////////////////////////
void Flash_LockRead(void)
{
	//---关闭预使能
	LL_FLASH_DisablePrefetch();

	//---获取写保护
	UINT32_T flashWRP = LL_FLASH_OB_GetWRP();

	//---获取读保护
	UINT32_T flashRDP = LL_FLASH_OB_GetRDP();

	//---用户配置
	UINT8_T  flashUser = LL_FLASH_OB_GetUser();

	//---配置读保护
	if (flashRDP == LL_OB_RDP_LEVEL_0)
	{
		LL_FLASH_Unlock();
		LL_FLASH_OB_Unlock();
		LL_FLASH_OB_RDP_LevelConfig(LL_OB_RDP_LEVEL_1);
		LL_FLASH_OB_Lock();
		LL_FLASH_Lock();
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 解锁读取，不使能读保护
//////////////////////////////////////////////////////////////////////////////
void Flash_UnLockRead(void)
{
	//---关闭预使能
	LL_FLASH_DisablePrefetch();

	//---获取写保护
	UINT32_T flashWRP = LL_FLASH_OB_GetWRP();

	//---获取读保护
	UINT32_T flashRDP = LL_FLASH_OB_GetRDP();

	//---用户配置
	UINT8_T  flashUser = LL_FLASH_OB_GetUser();

	//---解除读保护
	if (flashWRP != LL_OB_RDP_LEVEL_0)
	{
		LL_FLASH_Unlock();
		LL_FLASH_OB_Unlock();
		LL_FLASH_OB_RDP_LevelConfig(LL_OB_RDP_LEVEL_0);
		LL_FLASH_OB_Lock();
		LL_FLASH_Lock();
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：读取设备的Flash的大小
//////////////////////////////////////////////////////////////////////////////
UINT32_T Flash_GetSize(void)
{
	return  LL_GetFlashSize();
}