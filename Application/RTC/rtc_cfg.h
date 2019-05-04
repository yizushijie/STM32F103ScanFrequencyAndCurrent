#ifndef RTC_CFG_H_
#define RTC_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===�ṹ�嶨��
	typedef struct _Soft_RTC_HandlerType					Soft_RTC_HandlerType;

	//===����ָ��ṹ��
	typedef struct _Soft_RTC_HandlerType					* pSoft_RTC_HandlerType;

	//===�������RTCʱ��
	struct _Soft_RTC_HandlerType
	{
		RTC_HandlerType msgSoftRTC;						//---ʹ�õ�RTC
		VLTUINT8_T      msgWatchSpanDays;				//---��ض����췢���仯
		VLTUINT8_T		msgNowDay;						//---��ǰ������

	};

	//===�ⲿ���ýӿ�
	extern  Soft_RTC_HandlerType g_SysSoftRTC;
	extern  pSoft_RTC_HandlerType pSysSoftRTC;

	//===��������
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