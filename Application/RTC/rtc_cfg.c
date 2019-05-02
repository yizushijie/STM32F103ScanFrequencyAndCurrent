#include "rtc_cfg.h"

//===全局变量定义
RTC_HandlerType g_SysSoftRTC = { 0 };
pRTC_HandlerType pSysSoftRTC = &g_SysSoftRTC;

//===每个月的天数
const UINT8_T g_MonthDaysTab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：根据软件编译时间计算当前系统时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildInit(RTC_HandlerType *RTCx)
{
	//---取编译日期---年月日---__DATE__===Jul 03 2018

	//---一月
	if (__DATE__[2] == 'n')
	{
		RTCx->month = 1;
	}

	//--二月
	else if (__DATE__[2] == 'b')
	{
		RTCx->month = 2;
	}
	else if (__DATE__[2] == 'r')
	{
		if (__DATE__[0] == 'M')
		{
			//---三月
			RTCx->month = 3;
		}
		else
		{
			//---四月
			RTCx->month = 4;
		}
	}
	else if (__DATE__[2] == 'y')
	{
		//---五月
		RTCx->month = 5;
	}
	else if (__DATE__[2] == 'n')
	{
		//---六月
		RTCx->month = 6;
	}
	else if (__DATE__[2] == 'l')
	{
		//---七月
		RTCx->month = 7;
	}
	else if (__DATE__[2] == 'g')
	{
		//---八月
		RTCx->month = 8;
	}
	else if (__DATE__[2] == 'p')
	{
		//---九月
		RTCx->month = 9;
	}
	else if (__DATE__[2] == 't')
	{
		//---十月
		RTCx->month = 10;
	}
	else if (__DATE__[2] == 'v')
	{
		//---十一月
		RTCx->month = 11;
	}
	else if (__DATE__[2] == 'c')
	{
		//---十二月
		RTCx->month = 12;
	}
	else
	{
		//---默认是一月
		RTCx->month = 1;
	}

	//---年
	RTCx->year = (UINT8_T)((__DATE__[9] - '0') * 10 + (__DATE__[10] - '0'));

	//---日
	RTCx->day = (UINT8_T)((__DATE__[4] - '0') * 10 + (__DATE__[5] - '0'));

	//---世纪
	RTCx->century = (UINT8_T)((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0') + 1);

	//---计算星期
	RTCx->week = SysRTC_CalcWeekDay(RTCx);

	//---取编译日期---时分秒---__TIME__=06:17:05

	//---时
	RTCx->hour = (UINT8_T)((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'));

	//---分
	RTCx->minute = (UINT8_T)((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'));

	//---秒
	RTCx->second = (UINT8_T)((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'));

	//---时间格式是24H
	RTCx->time24H = 1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：RTC时间处理，更新年月日时分秒
//////输入参数: 系统滴答定时器的计数值
//////输出参数: 无
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildHandle(RTC_HandlerType *RTCx, UINT32_T rtcSecond)
{
	UINT32_T cnt = 0;

	//---判断滴答定时是否发生溢出操作
	if (rtcSecond < (RTCx->secondTick))
	{
		cnt = (0xFFFFFFFF - RTCx->secondTick + rtcSecond);
	}
	else
	{
		cnt = rtcSecond - RTCx->secondTick;
	}

	//---计算当前年份==(世纪-1)*100+年
	int iY = RTCx->century - 1;
	iY = (iY * 100) + RTCx->year;

	//---是否发生秒变化
	if (cnt >= 1000)
	{
		//---更新秒滴答计数值
		RTCx->secondTick = rtcSecond;

		//---默认时间基数是1ms，基数1000次，发生秒变化
		RTCx->second += 1;

		//---秒变化
		if (RTCx->second > 60)
		{
			RTCx->minute += 1;
			RTCx->second = 0;

			//---分变化
			if (RTCx->minute > 60)
			{
				RTCx->hour += 1;
				RTCx->minute = 0;

				//---时变化
				if (RTCx->hour > 24)
				{
					//---天变化
					RTCx->day += 1;

					//---星期变化
					RTCx->week += 1;

					//---一星期只有七天
					if (RTCx->week > 7)
					{
						RTCx->week = 1;
					}

					//---清零时变化
					RTCx->hour = 0;

					//---判断是不是二月
					if (RTCx->month == 2)
					{
						//---年和世纪是分开的
						if (YEAR_TYPE(iY) != 0)
						{
							//---天变化
							if (RTCx->day > 29)
							{
								RTCx->day = 0;
								RTCx->month += 1;
							}
						}
						else
						{
							//---天变化
							if (RTCx->day > 28)
							{
								RTCx->day = 0;
								RTCx->month += 1;
							}
						}
					}
					else
					{
						//---天变化
						if (RTCx->day > g_MonthDaysTab[RTCx->month])
						{
							RTCx->day = 0;
							RTCx->month += 1;

							//---年变化
							if (RTCx->month > 12)
							{
								RTCx->month = 1;
								RTCx->year += 1;

								//---世纪变化
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
//////函		数：
//////功		能：根据日期判断星期几（使用基姆拉尔森计算公式）
//////输入参数:
//////输出参数: 1---星期一；2---星期二；3---星期三；4---星期四；5---星期五；6---星期六；7---星期天；0---错误
//////说		明：
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
		基姆拉尔森计算公式：
		W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
		在公式中d表示日期中的日数，m表示月份数，y表示年数。
		注意：在公式中有个与其他公式不同的地方：
		把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算
	*/
	if (1 == iM || 2 == iM)
	{
		iM += 12;
		iY--;
	}
	iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100 + iY / 400) % 7;

	//---解析星期几
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