#include "main.h"

//===频率电流扫描的小板版本
#define LNW_FT_ASK_FI_VERSION		2

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：系统时钟的配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SystemClock_Config(void)
{
	//---Flash延时等待设置
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	//---等待设置完成
	if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
	{
		Error_Handler();
	}

	//---使能外部晶振
	LL_RCC_HSE_Enable();

	//---等待时钟切换完成
	while (LL_RCC_HSE_IsReady() != 1);

	//---使能内部低频晶振---40KHz
	LL_RCC_LSI_Enable();

	//---等待时钟切换完成
	while (LL_RCC_LSI_IsReady() != 1);

	//---配置PLL时钟
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

	//---使能PLL
	LL_RCC_PLL_Enable();

	//---等待PLL稳定
	while (LL_RCC_PLL_IsReady() != 1);

	//---设置APHB的时钿--外设的总线时钟
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	//---设置APB1的时钿--外设的总线时钟
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

	//---设置APB2的时钿--外设的总线时钟
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	//---设置系统时钟为PLL之后的时钟
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	//--- 等待系统时钟稳定
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	//LL_Init1msTick( 72000000 );
	//---设置系统滴答定时器的时钟为系统高速时钿--使能滴答定时器的时钟
	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

	//---设置系统代码的时钟
	LL_SetSystemCoreClock(72000000);

	//---使能Flash的预读取功能
	LL_FLASH_EnablePrefetch();

	//---使能数据缓存功能
	LL_FLASH_EnableDataCache();

	//---使能指令高速缓存
	LL_FLASH_EnableInstCache();
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void NVIC_Init(void)
{
	//---设置NVIC的分组模式
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
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void Sys_Init(void)
{
	//---系统时钟的初始化
	SystemClock_Config();

	//---NVIC初始化
	NVIC_Init();

	//---自定义内存管理初始化
	MyMemInit();

	//---GPIO初始化
	GPIOTask_Init();

	//---滴答定时器初始化
	SysTickTask_Init();

	//---随机数初始化
	RandomTask_Init();

	//---CRC校验初始化
	CRCTask_Init();

	//---LED初始化
	LEDTask_Init();

	//---FLASH模拟EEPROM的初始化
	EEPROMTask_Init();

	//---初始化定时器
	TimerTask_Init();

	//---WM8510的初始化
	WM8510Task_I2C_Init(pWM8510Device0, DelayTask_us, 0);

	//---简单的增加WM8510外部时钟的自校准功能
	WM8510Task_I2C_CalibrateClock(pWM8510Device0);

	//---第二版本的频率电流小板才支持eeprom存储数据
	#if(LNW_FT_ASK_FI_VERSION==2)
		//---eeprom的驱动初始化，初始化要在进行eeprom参数读写的前面，否则会发生错误
		AT24CXXTask_I2C_Init(pAT24CXXDevice0,DelayTask_us, DelayTask_ms,0);
	#endif

	//---ADC初始化
	ADCTask_ADC_Init();

	//---电源控制初始化
	PowerTask_Init();

	//---串口的初始化
	USARTTask_Init(pUSART1, USART1_RX_MAX_SIZE, USART1_RX_BUFFER, USART_CRC_NONE, USART1_TX_MAX_SIZE, USART1_TX_BUFFER, USART_CRC_NONE, SysTickTask_GetTick);

	//---按键触发信号的初始化
	KeyTask_Init();

	//---解码器初始化
	DecodeTask_Init();

	//---ASK的初始化
	RFASKTask_Init(pRFASK);

	//---备份域初始化
	//BKPTask_Init();

	//---系统模拟RTC
	SysRTCTask_SoftBuildInit(pSysSoftRTC,3);
	
	//---查询解码的初始化，所有的SITE都进行解码
	DecodeTask_QueryInit(0x0F);

	////---时钟输出
	//CLKA_FREQ_ON;
	//CLKB_FREQ_ON;
	//CLKC_FREQ_ON;
	//CLKD_FREQ_ON;

	//---使能Flash读保护
	//FlashTask_LockRead();
	//---开启看门狗
	IWDGTask_Init();

	//---启动解码逻辑
	//DecodeTask_START();
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
int main(void)
{
	//---变量定义
	UINT8_T getSOT = 0;
	UINT8_T getRST = 0;
	//---系统初始化函数
	Sys_Init();
	//---主循环
	while (1)
	{
		//---获取SOT信号
		getSOT = KeyTask_ScanSOT();
		//---读取解码信号
		getRST = DecodeTask_ScanRST();
		//---判断解码信号
		if (getRST != 0)
		{
			//---查询解码的初始化
			DecodeTask_QueryInit(getRST);
		}
		//---查询解码
		DecodeTask_Query();
		//---执行频率电流扫描相关的任务---YSEL
		RFASKTask_KeyTask(pUSART1, pRFASK, pWM8510Device0, getSOT);
		//---在线调试命令
		RFASKTask_Task(pUSART1, pRFASK, pWM8510Device0);
		//---模拟RTC处理,并进行复位时间宽度的监控
		SysRTCTask_SoftBuildTask(pSysSoftRTC, SysTickTask_GetTick());
		//---喂狗
		WDT_RESET();
	}
}