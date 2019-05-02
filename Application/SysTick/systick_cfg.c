#include "systick_cfg.h"

//---��������
SysTick_HandlerType		g_SysTick = { 0 };
pSysTick_HandlerType	pSysTick = &g_SysTick;

#if defined (USE_HAL_DRIVER)

//---�ⲿ���õ�Hal����ʱ����
extern HAL_TickFreqTypeDef uwTickFreq;
#endif

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_Suspend(void)
{
	//---��ʹ��ϵͳ�δ�ʱ�����ж�
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_Resume(void)
{
	//---ʹ��ϵͳ�δ�ʱ�����ж�
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ� SysTickʹ�õĽṹ��ĳ�ʼ��
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_StructInit(SysTick_HandlerType *sysTickx)
{
	sysTickx->msgIncTick = 0;
	sysTickx->msgDecTick = 0;
	sysTickx->msgIncTickOVF = 0;
	sysTickx->msgHalIncTick = NULL;
	sysTickx->msgFuncTick = NULL;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		���� ���1ms�ĵδ�ʱ��
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_Init(void)
{
	//---��ʼ���ṹ��
	SysTick_StructInit(pSysTick);

	//---����δ�ʱ��
	SysTick_Suspend();

	//---����1ms��ʱ��δ���---�ó�ʼ������û��ע���ж�
	LL_InitTick(SystemCoreClock, 1000);

	//---�����ж���ռ����
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//---���õδ�ʱ���ж�---����Ϊ������ȼ�
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

	//---ʹ�ܵδ�ʱ���ж�---Ĭ����1ms
	SysTick_Resume();
#ifdef USE_HAL_DRIVER

	//---Hal��������ʱ����
	uwTickFreq = HAL_TICK_FREQ_DEFAULT;

	//---ע��Hal�������
	pSysTick->msgHalIncTick = HAL_IncTick;
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_DeInit(void)
{
	SysTick_Suspend();
	SysTick_StructInit(pSysTick);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_FuncTick(void(*pFuncTick)(void))
{
	if (pFuncTick != NULL)
	{
		//---ע��������
		pSysTick->msgFuncTick = pFuncTick;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��ȴ���ʱ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_IRQDecTick(UINT32_T waitDecTick)
{
	//---�жϵ�ǰ��ֵ
	UINT32_T load = SysTick->LOAD + 1;
	UINT32_T val = SysTick->VAL;
	load >>= 1;

	//---����������ʱ
	if (val < load)
	{
		waitDecTick += 1;
	}

	//---��ʱ����
	pSysTick->msgDecTick = waitDecTick;

	//---�ȴ�ʱ�䵽��
	while (pSysTick->msgDecTick != 0)
	{
		WDT_RESET();
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��ȴ���ʱ����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_DecTick(UINT32_T waitDecTick)
{
	//---����Ҫ�δ��ʱ��
	UINT32_T ticks = SysTick->VAL;

	//---����Ҫ��ʱ��usʱ��
	UINT32_T usDelayTicks = (ticks / SYS_CLOCK_MHZ);

	//---���ʱ����1us������ڣ�����
	if (usDelayTicks > 1)
	{
		//---��ǰ����������ֵ�ļ���
		SysTick_WaitusTick(usDelayTicks);

		//---ms�жϼ�����С
		waitDecTick -= 1;

		//---ʣ�µ���ʱʱ�䣬ǿ��װ��������Ϊ����
		ticks = (UINT32_T)(1000 - usDelayTicks - 20);
	}

	//---Ҫ��ʱ��ʱ��
	pSysTick->msgDecTick = waitDecTick;

	//---�ȴ�ʱ�䵽��
	while (pSysTick->msgDecTick != 0)
	{
		WDT_RESET();
	}

	//---���ʱ����1us������ڣ�����
	if ((ticks > 1)&&(ticks<1000))
	{
		//---��ɲ��1ms֮��Ҫ��ɵļ���ֵ
		SysTick_WaitusTick(ticks);
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT32_T SysTick_GetTick(void)
{
	return pSysTick->msgIncTick;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_IRQTick(void)
{
	//---�ݼӼ���
	pSysTick->msgIncTick++;

	//---�ж��Ƿ����
	if (pSysTick->msgIncTick == 0)
	{
		pSysTick->msgIncTickOVF++;
	}

	//---������
	if (pSysTick->msgFuncTick != NULL)
	{
		pSysTick->msgFuncTick();
	}

	//---hal��ʱ����
	if (pSysTick->msgHalIncTick != NULL)
	{
		pSysTick->msgHalIncTick();
	}

	//---�ݼ�����
	if (pSysTick->msgDecTick != 0)
	{
		pSysTick->msgDecTick--;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitTick(UINT32_T usTime)
{
	UINT32_T load = SysTick->LOAD;
	UINT32_T ctrl = SysTick->CTRL;
	UINT32_T val = SysTick->VAL;

	//---�رն�ʱ����ʱ��ѡ��Ϊϵͳʱ�ӣ�������8��Ƶ
	SysTick->CTRL = 0x00000004;

	//---����װ��ֵ
	SysTick->LOAD = usTime * SYS_CLOCK_MHZ - SYS_CLOCK_ADD;//150;//usTimer*SYS_CLOCK_MHZ - 1;

	//---���������
	SysTick->VAL = 0x00;

	//---ʹ�ܼ���������
	SysTick->CTRL |= 0x01;

	//---�ȴ�������0
	while (!(SysTick->CTRL & 0x00010000))
	{
		WDT_RESET();
	}

	//---�رն�ʱ����ʱ��ѡ��Ϊϵͳʱ�ӣ�������8��Ƶ
	SysTick->CTRL = 0x00000004;

	//---�ָ�װ��ֵ
	SysTick->LOAD = load;

	//---��������������
	SysTick->VAL = val;

	//---�ָ�����������
	SysTick->CTRL = ctrl;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ��ȴ�us��ʱ�ĵ���
//////�������:
//////�������:
//////˵		�������������������ֵ��1000�����ֵ����1000�����������Զ�����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitusTick(UINT32_T usTime)
{
	//---�������ֵ
	UINT32_T ticks = ((usTime * SYS_CLOCK_MHZ));
	//---����ֵ
	UINT32_T told = SysTick->VAL;
	UINT32_T tnow = 0;
	UINT32_T tcnt = 0;

	//---װ��ֵ
	UINT32_T tload = SysTick->LOAD;

	//---�ж��ǲ��ǳ�������
	if (usTime < 1000)
	{
		while (1)
		{
			tnow = SysTick->VAL;
			if (told != tnow)
			{
				//---������Ϊ�ݼ�������
				if (tnow < told)
				{
					tcnt += (told - tnow);
				}
				else
				{
					tcnt += (tload - tnow + told);
				}
				told = tnow;
				if (tcnt >= ticks)
				{
					break;
				}
			}
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_WaitSysTick(UINT32_T sysTickTime)
{
	//---�������ֵ
	UINT32_T ticks = sysTickTime;

	//---����ֵ
	UINT32_T told = SysTick->VAL;
	UINT32_T tnow = 0;
	UINT32_T tcnt = 0;

	//---װ��ֵ
	UINT32_T tload = SysTick->LOAD;
	while (1)
	{
		tnow = SysTick->VAL;
		if (told != tnow)
		{
			//---������Ϊ�ݼ�������
			if (tnow < told)
			{
				tcnt += (told - tnow);
			}
			else
			{
				tcnt += (tload - tnow + told);
			}
			told = tnow;
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		�����ж�ϵͳ�δ�ʱ���ǲ���1ms�Ķ�ʱ
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysTick_1msIncTick(void)
{
	//---�ж�ʱ��Դ�Ƿ����8��Ƶ
	if ((SysTick->CTRL&SysTick_CTRL_CLKSOURCE_Msk) != 0)
	{
		//===ʱ��Դû�н���8��Ƶ
		if (SysTick->LOAD != (SYS_CLOCK_KHZ - 1))
		{
			return ERROR_1;
		}
	}
	else
	{
		//===ʱ��Դ����8��Ƶ
		if (SysTick->LOAD != ((SYS_CLOCK_KHZ >> 3) - 1))
		{
			return ERROR_1;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ� ��ȡ��������Ĵ�С
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT32_T SysTick_GetTickOVF(void)
{
	return pSysTick->msgIncTickOVF;
}