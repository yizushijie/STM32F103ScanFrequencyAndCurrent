#ifndef RTC_TASK_H_
#define RTC_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "rtc_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===º¯Êý¶¨Òå
	void SysRTCTask_SoftBuildInit(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays, UINT8_T watchaMode);
	void SysRTCTask_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond);
	UINT8_T SysRTCTask_CalcWeekDay(RTC_HandlerType*RTCx);
	UINT8_T SysRTCTask_RealTimeSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCTask_RefreshSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCTask_SetSoftWatch(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays);
	UINT8_T SysRTCTask_ClearSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCTask_SoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCTask_SoftBuildTask(Soft_RTC_HandlerType* RTCx, UINT32_T rtcSecond);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*RTC_TASK_H_ */