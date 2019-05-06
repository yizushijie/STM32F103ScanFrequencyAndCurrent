#ifndef RTC_LIB_H_
#define RTC_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "rtc_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===º¯Êý¶¨Òå
	void SysRTCLib_SoftBuildInit(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays, UINT8_T watchaMode);
	void SysRTCLib_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond);
	UINT8_T SysRTCLib_CalcWeekDay(RTC_HandlerType*RTCx);
	UINT8_T SysRTCLib_RealTimeSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCLib_RefreshSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCLib_SetSoftWatch(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays);
	UINT8_T SysRTCLib_ClearSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTCLib_SoftWatch(Soft_RTC_HandlerType* RTCx);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*RTC_LIB_H_ */