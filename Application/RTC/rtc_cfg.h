#ifndef RTC_CFG_H_
#define RTC_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===结构体定义
	typedef struct _Soft_RTC_HandlerType					Soft_RTC_HandlerType;

	//===定义指针结构体
	typedef struct _Soft_RTC_HandlerType					* pSoft_RTC_HandlerType;

	//===定义软件RTC时钟
	struct _Soft_RTC_HandlerType
	{
		RTC_HandlerType msgSoftRTC;						//---使用的RTC
		VLTUINT8_T      msgWatchSpanDays;				//---监控多少天发生变化
		VLTUINT8_T		msgNowDay;						//---当前的天数

	};

	//===外部调用接口
	extern  Soft_RTC_HandlerType g_SysSoftRTC;
	extern  pSoft_RTC_HandlerType pSysSoftRTC;

	//===函数定义
	void SysRTC_SoftBuildInit(Soft_RTC_HandlerType*RTCx,UINT8_T spanDays);
	void SysRTC_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond);
	UINT8_T SysRTC_CalcWeekDay(Soft_RTC_HandlerType*RTCx);
	UINT8_T SysRTC_WatchSpanDays(Soft_RTC_HandlerType* RTCx);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*RTC_CFG_H_ */