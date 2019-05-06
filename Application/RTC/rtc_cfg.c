#include "rtc_cfg.h"

//===ȫ�ֱ�������
Soft_RTC_HandlerType g_SysSoftRTC = { 0 };
pSoft_RTC_HandlerType pSysSoftRTC = &g_SysSoftRTC;

//===ÿ���µ�����
const UINT8_T g_MonthDaysTab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������������ʱ����㵱ǰϵͳʱ��,������ϵͳ��λ��ʱ��
//////�������: watchaMode---0-����أ�1-ʵʱ��أ�2-ˢ�¼��
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildInit(Soft_RTC_HandlerType* RTCx,UINT16_T spanDays, UINT8_T watchaMode)
{
	//---ȡ��������---������---__DATE__===Jul 03 2018

	//---һ��
	if (__DATE__[2] == 'n')
	{
		RTCx->msgSoftRTC.month = 1;
	}

	//--����
	else if (__DATE__[2] == 'b')
	{
		RTCx->msgSoftRTC.month = 2;
	}
	else if (__DATE__[2] == 'r')
	{
		if (__DATE__[0] == 'M')
		{
			//---����
			RTCx->msgSoftRTC.month = 3;
		}
		else
		{
			//---����
			RTCx->msgSoftRTC.month = 4;
		}
	}
	else if (__DATE__[2] == 'y')
	{
		//---����
		RTCx->msgSoftRTC.month = 5;
	}
	else if (__DATE__[2] == 'n')
	{
		//---����
		RTCx->msgSoftRTC.month = 6;
	}
	else if (__DATE__[2] == 'l')
	{
		//---����
		RTCx->msgSoftRTC.month = 7;
	}
	else if (__DATE__[2] == 'g')
	{
		//---����
		RTCx->msgSoftRTC.month = 8;
	}
	else if (__DATE__[2] == 'p')
	{
		//---����
		RTCx->msgSoftRTC.month = 9;
	}
	else if (__DATE__[2] == 't')
	{
		//---ʮ��
		RTCx->msgSoftRTC.month = 10;
	}
	else if (__DATE__[2] == 'v')
	{
		//---ʮһ��
		RTCx->msgSoftRTC.month = 11;
	}
	else if (__DATE__[2] == 'c')
	{
		//---ʮ����
		RTCx->msgSoftRTC.month = 12;
	}
	else
	{
		//---Ĭ����һ��
		RTCx->msgSoftRTC.month = 1;
	}

	//---��
	if (__DATE__[9]==0x20)
	{
		RTCx->msgSoftRTC.year = (UINT8_T)((__DATE__[10] - '0'));
	}
	else
	{
		RTCx->msgSoftRTC.year = (UINT8_T)((__DATE__[9] - '0') * 10 + (__DATE__[10] - '0'));
	}
	
	//---��
	if (__DATE__[4] == 0x20)
	{
		RTCx->msgSoftRTC.day = (UINT8_T)(__DATE__[5] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.day = (UINT8_T)((__DATE__[4] - '0') * 10 + (__DATE__[5] - '0'));
	}

	//---����
	if (__DATE__[7] == 0x20)
	{
		RTCx->msgSoftRTC.century = (UINT8_T)((__DATE__[8] - '0')+ 1);
	}
	else
	{
		RTCx->msgSoftRTC.century = (UINT8_T)((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0') + 1);
	}
	
	//---��������
	RTCx->msgSoftRTC.week = SysRTC_CalcWeekDay(&(RTCx->msgSoftRTC));

	//---ȡ��������---ʱ����---__TIME__=06:17:05

	//---ʱ
	if (__TIME__[0]==0x20)
	{
		RTCx->msgSoftRTC.hour = (UINT8_T)(__TIME__[1] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.hour = (UINT8_T)((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'));
	}

	//---��
	if (__TIME__[3] == 0x20)
	{
		RTCx->msgSoftRTC.minute = (UINT8_T)(__TIME__[4] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.minute = (UINT8_T)((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'));
	}

	//---��
	if (__TIME__[6] == 0x20)
	{
		RTCx->msgSoftRTC.second = (UINT8_T)(__TIME__[7] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.second = (UINT8_T)((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'));
	}
	
	//---ʱ���ʽ��24H
	RTCx->msgSoftRTC.time24H = 1;

	//---�趨�ļ������
	RTCx->msgWatchSpanDays = spanDays;
	//---ʱ��������
	RTCx->msgWatchMode = watchaMode;
	//---��ǰʱ������
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	//---��ǰ������
	RTCx->msgMonth = RTCx->msgSoftRTC.month;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�RTCʱ�䴦������������ʱ����
//////�������: ϵͳ�δ�ʱ���ļ���ֵ
//////�������: ��
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond)
{
	UINT32_T cnt = 0;

	//---�жϵδ�ʱ�Ƿ����������
	if (rtcSecond < (RTCx->msgSoftRTC.secondTick))
	{
		cnt = (0xFFFFFFFF - RTCx->msgSoftRTC.secondTick + rtcSecond);
	}
	else
	{
		cnt = rtcSecond - RTCx->msgSoftRTC.secondTick;
	}

	//---���㵱ǰ���==(����-1)*100+��
	int iY = RTCx->msgSoftRTC.century - 1;
	iY = (iY * 100) + RTCx->msgSoftRTC.year;

	//---�Ƿ�����仯
	if (cnt >= 1000)
	{
		//---������δ����ֵ
		RTCx->msgSoftRTC.secondTick = rtcSecond;

		//---Ĭ��ʱ�������1ms������1000�Σ�������仯
		RTCx->msgSoftRTC.second += 1;

		//---��仯
		if (RTCx->msgSoftRTC.second > 60)
		{
			RTCx->msgSoftRTC.minute += 1;
			RTCx->msgSoftRTC.second = 0;

			//---�ֱ仯
			if (RTCx->msgSoftRTC.minute > 60)
			{
				RTCx->msgSoftRTC.hour += 1;
				RTCx->msgSoftRTC.minute = 0;

				//---ʱ�仯
				if (RTCx->msgSoftRTC.hour > 24)
				{
					//---��仯
					RTCx->msgSoftRTC.day += 1;

					//---���ڱ仯
					RTCx->msgSoftRTC.week += 1;

					//---һ����ֻ������
					if (RTCx->msgSoftRTC.week > 7)
					{
						RTCx->msgSoftRTC.week = 1;
					}

					//---����ʱ�仯
					RTCx->msgSoftRTC.hour = 0;

					//---�ж��ǲ��Ƕ���
					if (RTCx->msgSoftRTC.month == 2)
					{
						//---��������Ƿֿ���
						if (YEAR_TYPE(iY) != 0)
						{
							//---��仯
							if (RTCx->msgSoftRTC.day > 29)
							{
								RTCx->msgSoftRTC.day = 0;
								RTCx->msgSoftRTC.month += 1;
							}
						}
						else
						{
							//---��仯
							if (RTCx->msgSoftRTC.day > 28)
							{
								RTCx->msgSoftRTC.day = 0;
								RTCx->msgSoftRTC.month += 1;
							}
						}
					}
					else
					{
						//---��仯
						if (RTCx->msgSoftRTC.day > g_MonthDaysTab[RTCx->msgSoftRTC.month])
						{
							RTCx->msgSoftRTC.day = 0;
							RTCx->msgSoftRTC.month += 1;

							//---��仯
							if (RTCx->msgSoftRTC.month > 12)
							{
								RTCx->msgSoftRTC.month = 1;
								RTCx->msgSoftRTC.year += 1;

								//---���ͱ仯
								if (RTCx->msgSoftRTC.year > 100)
								{
									RTCx->msgSoftRTC.century += 1;
									RTCx->msgSoftRTC.year = 0;
								}
							}
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ����������ж����ڼ���ʹ�û�ķ����ɭ���㹫ʽ��
//////�������:
//////�������: 1---����һ��2---���ڶ���3---��������4---�����ģ�5---�����壻6---��������7---�����죻0---����
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_CalcWeekDay(RTC_HandlerType*RTCx)
{
	int iM = RTCx->month;

	//int iY = RTCx->year;
	int iY = RTCx->century - 1;
	iY = (iY * 100) + RTCx->year;
	int iD = RTCx->day;
	int iWeekDay = -1;
	UINT8_T _return = 0;
	/*
		��ķ����ɭ���㹫ʽ��
		W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
		�ڹ�ʽ��d��ʾ�����е�������m��ʾ�·�����y��ʾ������
		ע�⣺�ڹ�ʽ���и���������ʽ��ͬ�ĵط���
		��һ�ºͶ��¿�������һ���ʮ���º�ʮ���£����������2004-1-10����ɣ�2003-13-10�����빫ʽ����
	*/
	if (1 == iM || 2 == iM)
	{
		iM += 12;
		iY--;
	}
	iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100 + iY / 400) % 7;

	//---�������ڼ�
	switch (iWeekDay)
	{
		case 0:
			_return = 7;
			break;
		case 1:
			_return = 1;
			break;
		case 2:
			_return = 2;
			break;
		case 3:
			_return = 3;
			break;
		case 4:
			_return = 4;
			break;
		case 5:
			_return = 5;
			break;
		case 6:
			_return = 6;
			break;
		default:
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_RealTimeSoftWatch(Soft_RTC_HandlerType* RTCx)
{
	UINT16_T _return = OK_0;
	if (RTCx->msgWatchSpanDays!=0)
	{
		if (RTCx->msgSoftRTC.month < RTCx->msgMonth)
		{
			_return = RTCx->msgSoftRTC.month + 12 - RTCx->msgMonth;
		}
		else
		{
			_return = RTCx->msgMonth - RTCx->msgSoftRTC.month;
		}
		//---����ʱ������
		_return = (_return * 30) + RTCx->msgSoftRTC.day - RTCx->msgDay;
		//---�ж��Ƿ񵽴���ʱ�����ֵ��������ִ�и�λ����
		if ((_return > RTCx->msgWatchSpanDays) || ((_return == RTCx->msgWatchSpanDays)))
		{
			//---������λ�ˣ�������־λ������������飬����û��eeprom��оƬ����Ҫ��������eeprom��д�����
			RTCx->msgWatchMode = 1;
			//---�����λ
			SOFT_RESET();
			////---Ӳ����λ,�ȴ����Ź�����
			//while (1);
		}
		_return = OK_0;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////��		���� 
//////��		�ܣ� ˢ�¼��
//////�������: 
//////�������: 
//////˵		���� 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_RefreshSoftWatch(Soft_RTC_HandlerType* RTCx)
{
	RTCx->msgMonth=RTCx->msgSoftRTC.month;
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		���� 
//////��		�ܣ� ���ü��
//////�������: 
//////�������: 
//////˵		���� 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_SetSoftWatch(Soft_RTC_HandlerType* RTCx,UINT16_T spanDays)
{
	//---�趨�ļ������
	RTCx->msgWatchSpanDays = spanDays;
	RTCx->msgWatchMode = 0;
	//---��ǰʱ������
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	//---��ǰ������
	RTCx->msgMonth = RTCx->msgSoftRTC.month;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		���� 
//////��		�ܣ� ������
//////�������: 
//////�������: 
//////˵		���� 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_ClearSoftWatch(Soft_RTC_HandlerType* RTCx)
{
	RTCx->msgWatchMode = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////��		���� 
//////��		�ܣ� 
//////�������: 
//////�������: 
//////˵		���� 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_SoftWatch(Soft_RTC_HandlerType* RTCx)
{
	if (RTCx->msgWatchMode==1)
	{
		//---ʵʱ���
		SysRTC_RealTimeSoftWatch(RTCx);
	}
	else if (RTCx->msgWatchMode==2)
	{
		//---ˢ�¼��
		SysRTC_RefreshSoftWatch(RTCx);
	}
	return OK_0;
}