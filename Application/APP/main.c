#include "main.h"

//===Ƶ�ʵ���ɨ���С��汾
#define LNW_FT_ASK_FI_VERSION		2

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�ϵͳʱ�ӵ�����
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void SystemClock_Config(void)
{
	//---Flash��ʱ�ȴ�����
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	//---�ȴ��������
	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
	{
		Error_Handler();
	}

	//---ʹ���ⲿ����
	LL_RCC_HSE_Enable();

	//---�ȴ�ʱ���л����
	while (LL_RCC_HSE_IsReady() != 1);

	//---ʹ���ڲ���Ƶ����---40KHz
	LL_RCC_LSI_Enable();

	//---�ȴ�ʱ���л����
	while (LL_RCC_LSI_IsReady() != 1);

	//---����PLLʱ��
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

	//---ʹ��PLL
	LL_RCC_PLL_Enable();

	//---�ȴ�PLL�ȶ�
	while (LL_RCC_PLL_IsReady() != 1);

	//---����APHB��ʱ��--���������ʱ��
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	//---����APB1��ʱ��--���������ʱ��
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

	//---����APB2��ʱ��--���������ʱ��
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	//---����ϵͳʱ��ΪPLL֮���ʱ��
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	//--- �ȴ�ϵͳʱ���ȶ�
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	//LL_Init1msTick( 72000000 );
	//---����ϵͳ�δ�ʱ����ʱ��Ϊϵͳ����ʱ��--ʹ�ܵδ�ʱ����ʱ��
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	//---����ϵͳ�����ʱ��
	LL_SetSystemCoreClock(72000000);

	//---ʹ��Flash��Ԥ��ȡ����
	LL_FLASH_EnablePrefetch();

	//---ʹ�����ݻ��湦��
	LL_FLASH_EnableDataCache();

	//---ʹ��ָ����ٻ���
	LL_FLASH_EnableInstCache();
}

///////////////////////////////////////////////////////////////////////////////
//////��	   ����
//////��	   �ܣ�
//////�������:
//////�������:
//////˵	   ����
//////////////////////////////////////////////////////////////////////////////
void NVIC_Init(void)
{
	//---����NVIC�ķ���ģʽ
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	/* System interrupt init*/
	/* HardFault_IRQn interrupt configuration */
#ifdef USE_MCU_STM32F1
	NVIC_SetPriority(HardFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
#endif
	/* MemoryManagement_IRQn interrupt configuration */
	NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* BusFault_IRQn interrupt configuration */
	NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* UsageFault_IRQn interrupt configuration */
	NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* SVCall_IRQn interrupt configuration */
	NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* DebugMonitor_IRQn interrupt configuration */
	NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void Sys_Init(void)
{
	//---ϵͳʱ�ӵĳ�ʼ��
	SystemClock_Config();

	//---NVIC��ʼ��
	NVIC_Init();

	//---�Զ����ڴ�����ʼ��
	MyMemInit();

	//---GPIO��ʼ��
	GPIOTask_Init();

	//---�δ�ʱ����ʼ��
	SysTickTask_Init();

	//---�������ʼ��
	RandomTask_Init();

	//---CRCУ���ʼ��
	CRCTask_Init();

	//---LED��ʼ��
	LEDTask_Init();

	//---FLASHģ��EEPROM�ĳ�ʼ��
	EEPROMTask_Init();

	//---��ʼ����ʱ��
	TimerTask_Init();

	//---WM8510�ĳ�ʼ��
	WM8510Task_I2C_Init(pWM8510Device0, DelayTask_us, 0);

	//---�򵥵�����WM8510�ⲿʱ�ӵ���У׼����
	WM8510Task_I2C_CalibrateClock(pWM8510Device0);

	//---�ڶ��汾��Ƶ�ʵ���С���֧��eeprom�洢����
	#if(LNW_FT_ASK_FI_VERSION==2)
		//---eeprom��������ʼ������ʼ��Ҫ�ڽ���eeprom������д��ǰ�棬����ᷢ������
		AT24CXXTask_I2C_Init(pAT24CXXDevice0,DelayTask_us, DelayTask_ms,0);
	#endif

	//---ADC��ʼ��
	ADCTask_ADC_Init();

	//---��Դ���Ƴ�ʼ��
	PowerTask_Init();

	//---���ڵĳ�ʼ��
	USARTTask_Init(pUSART1, USART1_RX_MAX_SIZE, USART1_RX_BUFFER, USART_CRC_NONE, USART1_TX_MAX_SIZE, USART1_TX_BUFFER, USART_CRC_NONE, SysTickTask_GetTick);

	//---���������źŵĳ�ʼ��
	KeyTask_Init();

	//---��������ʼ��
	DecodeTask_Init();

	//---ASK�ĳ�ʼ��
	RFASKTask_Init(pRFASK);

	//---�������ʼ��
	//BKPTask_Init();

	//---ϵͳģ��RTC
	SysRTCTask_SoftBuildInit(pSysSoftRTC,3);
	
	//---��ѯ����ĳ�ʼ�������е�SITE�����н���
	DecodeTask_QueryInit(0x0F);

	////---ʱ�����
	//CLKA_FREQ_ON;
	//CLKB_FREQ_ON;
	//CLKC_FREQ_ON;
	//CLKD_FREQ_ON;

	//---ʹ��Flash������
	//FlashTask_LockRead();
	//---�������Ź�
	IWDGTask_Init();

	//---���������߼�
	//DecodeTask_START();
}

///////////////////////////////////////////////////////////////////////////////
//////��	   ����
//////��	   �ܣ�
//////�������:
//////�������:
//////˵	   ����
//////////////////////////////////////////////////////////////////////////////
int main(void)
{
	//---��������
	UINT8_T getSOT = 0;
	UINT8_T getRST = 0;
	//---ϵͳ��ʼ������
	Sys_Init();
	//---��ѭ��
	while (1)
	{
		//---��ȡSOT�ź�
		getSOT = KeyTask_ScanSOT();
		//---��ȡ�����ź�
		getRST = DecodeTask_ScanRST();
		//---�жϽ����ź�
		if (getRST != 0)
		{
			//---��ѯ����ĳ�ʼ��
			DecodeTask_QueryInit(getRST);
		}
		//---��ѯ����
		DecodeTask_Query();
		//---ִ��Ƶ�ʵ���ɨ����ص�����---YSEL
		RFASKTask_KeyTask(pUSART1, pRFASK, pWM8510Device0, getSOT);
		//---���ߵ�������
		RFASKTask_Task(pUSART1, pRFASK, pWM8510Device0);
		//---ģ��RTC����,�����и�λʱ���ȵļ��
		SysRTCTask_SoftBuildTask(pSysSoftRTC, SysTickTask_GetTick());
		//---ι��
		WDT_RESET();
	}
}