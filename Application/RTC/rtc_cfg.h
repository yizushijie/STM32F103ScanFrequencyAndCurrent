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
		VLTUINT16_T     msgWatchSpanDays;				//---��ض����췢���仯
		VLTUINT8_T		msgDay;							//---��ǰ�µ�����
		VLTUINT8_T		msgMonth;						//---��ǰ��
		VLTUINT8_T      msgWatchMode;					//---�Ƿ�ȡ����أ�0---ȡ����أ�1---ʵʱ��أ�2---ˢ�¼��
	};

	//===�ⲿ���ýӿ�
	extern  Soft_RTC_HandlerType g_SysSoftRTC;
	extern  pSoft_RTC_HandlerType pSysSoftRTC;

	//===��������
	void SysRTC_SoftBuildInit(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays, UINT8_T watchaMode);
	void SysRTC_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond);
	UINT8_T SysRTC_CalcWeekDay(RTC_HandlerType*RTCx);
	UINT8_T SysRTC_RealTimeSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTC_RefreshSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTC_SetSoftWatch(Soft_RTC_HandlerType* RTCx, UINT16_T spanDays);
	UINT8_T SysRTC_ClearSoftWatch(Soft_RTC_HandlerType* RTCx);
	UINT8_T SysRTC_SoftWatch(Soft_RTC_HandlerType* RTCx);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*RTC_CFG_H_ */