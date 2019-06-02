#ifndef USART_CFG_H_
#define USART_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "crc_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	typedef struct _USART_HandlerType						USART_HandlerType;
	typedef struct _USART_HandlerType						* pUSART_HandlerType;

	//===数据结构体
	typedef struct _USART_HandlerDef						USART_HandlerDef;
	typedef struct _USART_HandlerDef						*pUSART_HandlerDef;

	//===接收数据结构体
	typedef struct _USART_HandlerDef						USART_RxHandlerType;
	typedef struct _USART_HandlerDef						*pUSART_RxHandlerType;

	//===发送数据结构体
	typedef struct _USART_HandlerDef						USART_TxHandlerType;
	typedef struct _USART_HandlerDef						*pUSART_TxHandlerType;

	//===串口数据结构体定义
	struct _USART_HandlerDef
	{
		VLTUINT8_T		msgCheckSum;							//---校验和
		VLTUINT8_T		msgCRCFlag;								//---校验标志位  0 ---无校验，1 ---校验和，2---CRC8校验，3---CRC16校验，4---CRC32校验
		VLTUINT8_T		msgTaskState;							//---判断任务是否结束 0---未结束，1---结束
		VLTUINT8_T		msgStep;								//---步序
		VLTUINT8_T		msgAddNewLine;							//---是否需要换行符(\r\n)
		VLTUINT16_T		msgIndexW;								//---写序号
		VLTUINT16_T		msgIndexR;								//---读序号
		VLTUINT16_T		msgCount;								//---读写数据的长度
		VLTUINT16_T		msgSize;								//---数据缓存区的大小
		VLTUINT8_T		msgOverFlow;							//---超时标志位
		VLTUINT32_T		msgNowTime;								//---超时时间开始至
		VLTUINT32_T		msgEndTime;								//---超时时间结束值
		VLTUINT32_T		msgMaxTime;								//---超时时间
		UINT8_T			*pMsgVal;								//---缓存区(需要提前定义好数据的缓存区)
	};

	//===传输过程中判断判断是否需要初始化GPIO
	#define  USART_INIT_GPIO

	//===串口数据定义
	struct _USART_HandlerType
	{
		UINT8_T								msgIndex;			//---USART端口的索引号
		UINT8_T								msgRxID;			//---接收报头
		UINT8_T								msgTxID;			//---发送报头
		UINT8_T								msgDeviceID;		//---设备ID
		UINT8_T								msgIDIndex;			//---设备ID在数组中的位置
		UINT8_T								msgCmdIndex;		//---命令在数组中的位置
		UINT8_T								msgDataOneIndex;	//---数据1在数组中的位置
		UINT8_T								msgDataTwoIndex;	//---数据2在数组中的位置
		UINT32_T							msgTxBit;			//---发送GPIO端口
		GPIO_TypeDef						*msgTxPort;			//---USART的GPIO端口号
		UINT32_T							msg485Bit;			//---485的使能GPIO端口
		GPIO_TypeDef						*msg485Port;		//---485的使能GPIO端口
		USART_TypeDef						*msgUSART;			//---USART端口
		USART_TxHandlerType					msgTxHandler;		//---发送函数
		USART_RxHandlerType					msgRxHandler;		//---接收函数
		UINT32_T(*msgFuncTimeTick)(void);						//---用于超时计数
	};

	//===是否重映射printf函数
	#define USE_USART_PRINTF
	
	//===重映射printf之后的数据缓存区
	#ifdef USE_USART_PRINTF
	#define USART_PRINTF_SIZE					1024
	#endif
	
	//===使用的校验方式
	#define USART_CRC_NONE						0
	#define USART_CRC_CHECKSUM					1
	
	//===自适应CRC校验等级
	#ifdef USE_CRC8
	#define USART_CRC_CRC8						2
	#elif defined(USART_CRC_CHECKSUM)
	#define USART_CRC_CRC8						USART_CRC_CHECKSUM
	#else
	#define USART_CRC_CRC8						USART_CRC_NONE
	#endif
	
	#ifdef USE_CRC16
	#define USART_CRC_CRC16						3
	#elif defined(USART_CRC_CHECKSUM)
	#define USART_CRC_CRC16						USART_CRC_CHECKSUM
	#else
	#define USART_CRC_CRC16						USART_CRC_NONE
	#endif
	
	#ifdef USE_CRC32
	#define USART_CRC_CRC32						4
	#elif defined(USART_CRC_CHECKSUM)
	#define USART_CRC_CRC32						USART_CRC_CHECKSUM
	#else
	#define USART_CRC_CRC32						USART_CRC_NONE
	#endif
	
	//===发送端口的配置
	#define USART_TXGPIO_SET_INPUT					0
	#define USART_TXGPIO_SET_OUTPUT					1
	
	//===485数据端口的控制使能
	#define USART_485_RX_ENABLE						0
	#define USART_485_TX_ENABLE						1
	
	//===串口的状态
	#define USART_BUSY								0
	#define USART_OK								1
	#define USART_ERROR								2
	
	//===定义的任务函数
	#define USART_TASK_ONE							pUSART1
	#define USART_TASK_TWO							0
	#define USART_TASK_THREE						0
	
	//===外部调用接口
	extern USART_HandlerType						g_USART1;
	extern pUSART_HandlerType						pUSART1;

	//===函数定义
	UINT8_T  USART_StructInit(USART_HandlerType*  USARTx);
	UINT8_T  USART_Init(USART_HandlerType*  USARTx, UINT16_T rxSize, UINT8_T* pRxVal, UINT8_T rxCRCFlag, UINT16_T txSize, UINT8_T* pTxVal, UINT8_T txCRCFlag, UINT32_T(*pTimerTick)(void));
	UINT8_T  USART_GPIOInit(USART_HandlerType*  USARTx, UINT8_T isInput);
	UINT8_T  USART_485GPIOInit(USART_HandlerType*  USARTx, UINT8_T isEnable);
	UINT8_T  USART_EnableCRC_RX(USART_HandlerType*USARTx, UINT8_T crcFlag);
	UINT8_T  USART_GetCRC_RX(USART_HandlerType*USARTx);
	UINT8_T  USART_EnableCRC_TX(USART_HandlerType*USARTx, UINT8_T crcFlag);
	UINT8_T  USART_GetCRC_TX(USART_HandlerType*USARTx);
	UINT8_T  USART_TimeTick_Init(USART_HandlerType*USARTx, UINT8_T isRx);
	UINT8_T  USART_TimeTick_DeInit(USART_HandlerType*USARTx, UINT8_T isRx);
	UINT8_T  USART_TimeTick_OVF(USART_HandlerType*USARTx, UINT32_T timeOut, UINT8_T isRx);
	UINT8_T  USART_TimeOVFTask(USART_HandlerType*USARTx, UINT8_T isRx);
	UINT8_T  USART_ITRead_8BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USART_ITRead_16BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USART_ITRead_Task(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USART_GetOVF(USART_HandlerType* USARTx, UINT8_T isRx);
	UINT8_T  USART_ClearOVF(USART_HandlerType* USARTx, UINT8_T isRx);
	UINT8_T  USART_PollMode_WriteByte(USART_HandlerType*USARTx, UINT8_T  val);
	UINT8_T  USART_PollMode_WriteData(USART_HandlerType*USARTx, char *pVal);
	UINT8_T  USART_PollMode_ReadByte(USART_HandlerType*USARTx);
	UINT8_T  USART_PollMode_ReadData(USART_HandlerType*USARTx, char *pVal);
	UINT8_T  USART_ITWrite_TXETask(USART_HandlerType*USARTx);
	UINT8_T  USART_ITWrite_TCTask(USART_HandlerType*USARTx);
	UINT8_T  USART_RealTime_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USART_RealTime_AddByteSize(USART_HandlerType*USARTx, UINT16_T val);
	UINT8_T  USART_RealTime_AddByteCRC(USART_HandlerType*USARTx);
	UINT8_T  USART_FillMode_Init( USART_HandlerType*USARTx );
	UINT8_T  USART_FillMode_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USART_FillMode_AddData(USART_HandlerType*USARTx, UINT8_T *pVal, UINT16_T length);
	UINT8_T  USART_CRCTask_Read(USART_HandlerType*USARTx);
	UINT8_T  USART_CRCTask_Write(USART_HandlerType*USARTx);
	UINT8_T  USART_FillMode_WriteSTART(USART_HandlerType*USARTx, UINT8_T isNeedID);
	UINT8_T  USART_GetReadState(USART_HandlerType* USARTx);
	UINT8_T  USART_ClearReadState(USART_HandlerType* USARTx);
	UINT8_T  USART_GetWriteState(USART_HandlerType* USARTx);
	UINT8_T  USART_ClearWriteState(USART_HandlerType* USARTx);
	UINT8_T  USART_ReadInit(USART_HandlerType*  USARTx);
	UINT8_T  USART_WriteInit(USART_HandlerType*  USARTx);
	UINT8_T  USART_DeviceID(USART_HandlerType*USARTx);
	void     USART_Printf(USART_HandlerType*USARTx, char*fmt, ...);
	void	 USART_PrintfClockFreq(USART_HandlerType*USARTx);
	UINT8_T  USART_Clock(USART_TypeDef* USARTx, UINT8_T isEnable);
	UINT8_T  USART_DeInit(USART_HandlerType*USARTx);
	UINT8_T  USART_DeviceInit(USART_HandlerType *USARTx, UINT8_T id, UINT8_T idIndex, UINT8_T cmdIndex, UINT8_T d1Index, UINT8_T d2Index);
	UINT8_T  USART1_Init(USART_HandlerType*USARTx);
	UINT8_T  USART2_Init(USART_HandlerType*USARTx);
	UINT8_T  USART3_Init(USART_HandlerType*USARTx);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*USART_CFG_H_ */
