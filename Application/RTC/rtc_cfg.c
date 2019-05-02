#include "rtc_cfg.h"

//===ȫ�ֱ�������
RTC_HandlerType g_SysSoftRTC = { 0 };
pRTC_HandlerType pSysSoftRTC = &g_SysSoftRTC;

//===ÿ���µ�����
const UINT8_T g_MonthDaysTab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ������������ʱ����㵱ǰϵͳʱ��
//////�������:
//////�������:
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildInit(RTC_HandlerType *RTCx)
{
	//---ȡ��������---������---__DATE__===Jul 03 2018

	//---һ��
	if (__DATE__[2] == 'n')
	{
		RTCx->month = 1;
	}

	//--����
	else if (__DATE__[2] == 'b')
	{
		RTCx->month = 2;
	}
	else if (__DATE__[2] == 'r')
	{
		if (__DATE__[0] == 'M')
		{
			//---����
			RTCx->month = 3;
		}
		else
		{
			//---����
			RTCx->month = 4;
		}
	}
	else if (__DATE__[2] == 'y')
	{
		//---����
		RTCx->month = 5;
	}
	else if (__DATE__[2] == 'n')
	{
		//---����
		RTCx->month = 6;
	}
	else if (__DATE__[2] == 'l')
	{
		//---����
		RTCx->month = 7;
	}
	else if (__DATE__[2] == 'g')
	{
		//---����
		RTCx->month = 8;
	}
	else if (__DATE__[2] == 'p')
	{
		//---����
		RTCx->month = 9;
	}
	else if (__DATE__[2] == 't')
	{
		//---ʮ��
		RTCx->month = 10;
	}
	else if (__DATE__[2] == 'v')
	{
		//---ʮһ��
		RTCx->month = 11;
	}
	else if (__DATE__[2] == 'c')
	{
		//---ʮ����
		RTCx->month = 12;
	}
	else
	{
		//---Ĭ����һ��
		RTCx->month = 1;
	}

	//---��
	RTCx->year = (UINT8_T)((__DATE__[9] - '0') * 10 + (__DATE__[10] - '0'));

	//---��
	RTCx->day = (UINT8_T)((__DATE__[4] - '0') * 10 + (__DATE__[5] - '0'));

	//---����
	RTCx->century = (UINT8_T)((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0') + 1);

	//---��������
	RTCx->week = SysRTC_CalcWeekDay(RTCx);

	//---ȡ��������---ʱ����---__TIME__=06:17:05

	//---ʱ
	RTCx->hour = (UINT8_T)((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'));

	//---��
	RTCx->minute = (UINT8_T)((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'));

	//---��
	RTCx->second = (UINT8_T)((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'));

	//---ʱ���ʽ��24H
	RTCx->time24H = 1;
}

///////////////////////////////////////////////////////////////////////////////
//////��		����
//////��		�ܣ�RTCʱ�䴦������������ʱ����
//////�������: ϵͳ�δ�ʱ���ļ���ֵ
//////�������: ��
//////˵		����
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildHandle(RTC_HandlerType *RTCx, UINT32_T rtcSecond)
{
	UINT32_T cnt = 0;

	//---�жϵδ�ʱ�Ƿ����������
	if (rtcSecond < (RTCx->secondTick))
	{
		cnt = (0xFFFFFFFF - RTCx->secondTick + rtcSecond);
	}
	else
	{
		cnt = rtcSecond - RTCx->secondTick;
	}

	//---���㵱ǰ���==(����-1)*100+��
	int iY = RTCx->century - 1;
	iY = (iY * 100) + RTCx->year;

	//---�Ƿ�����仯
	if (cnt >= 1000)
	{
		//---������δ����ֵ
		RTCx->secondTick = rtcSecond;

		//---Ĭ��ʱ�������1ms������1000�Σ�������仯
		RTCx->second += 1;

		//---��仯
		if (RTCx->second > 60)
		{
			RTCx->minute += 1;
			RTCx->second = 0;

			//---�ֱ仯
			if (RTCx->minute > 60)
			{
				RTCx->hour += 1;
				RTCx->minute = 0;

				//---ʱ�仯
				if (RTCx->hour > 24)
				{
					//---��仯
					RTCx->day += 1;

					//---���ڱ仯
					RTCx->week += 1;

					//---һ����ֻ������
					if (RTCx->week > 7)
					{
						RTCx->week = 1;
					}

					//---����ʱ�仯
					RTCx->hour = 0;

					//---�ж��ǲ��Ƕ���
					if (RTCx->month == 2)
					{
						//---��������Ƿֿ���
						if (YEAR_TYPE(iY) != 0)
						{
							//---��仯
							if (RTCx->day > 29)
							{
								RTCx->day = 0;
								RTCx->month += 1;
							}
						}
						else
						{
							//---��仯
							if (RTCx->day > 28)
							{
								RTCx->day = 0;
								RTCx->month += 1;
							}
						}
					}
					else
					{
						//---��仯
						if (RTCx->day > g_MonthDaysTab[RTCx->month])
						{
							RTCx->day = 0;
							RTCx->month += 1;

							//---��仯
							if (RTCx->month > 12)
							{
								RTCx->month = 1;
								RTCx->year += 1;

								//---���ͱ仯
								if (RTCx->year > 100)
								{
									RTCx->century += 1;
									RTCx->year = 0;
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
UINT8_T SysRTC_CalcWeekDay(RTC_HandlerType *RTCx)
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