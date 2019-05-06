#include "rtc_cfg.h"

//===全局变量定义
Soft_RTC_HandlerType g_SysSoftRTC = { 0 };
pSoft_RTC_HandlerType pSysSoftRTC = &g_SysSoftRTC;

//===每个月的天数
const UINT8_T g_MonthDaysTab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：根据软件编译时间计算当前系统时钟,并设置系统复位的时间
//////输入参数: watchaMode---0-不监控，1-实时监控，2-刷新监控
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildInit(Soft_RTC_HandlerType* RTCx,UINT16_T spanDays, UINT8_T watchaMode)
{
	//---取编译日期---年月日---__DATE__===Jul 03 2018

	//---一月
	if (__DATE__[2] == 'n')
	{
		RTCx->msgSoftRTC.month = 1;
	}

	//--二月
	else if (__DATE__[2] == 'b')
	{
		RTCx->msgSoftRTC.month = 2;
	}
	else if (__DATE__[2] == 'r')
	{
		if (__DATE__[0] == 'M')
		{
			//---三月
			RTCx->msgSoftRTC.month = 3;
		}
		else
		{
			//---四月
			RTCx->msgSoftRTC.month = 4;
		}
	}
	else if (__DATE__[2] == 'y')
	{
		//---五月
		RTCx->msgSoftRTC.month = 5;
	}
	else if (__DATE__[2] == 'n')
	{
		//---六月
		RTCx->msgSoftRTC.month = 6;
	}
	else if (__DATE__[2] == 'l')
	{
		//---七月
		RTCx->msgSoftRTC.month = 7;
	}
	else if (__DATE__[2] == 'g')
	{
		//---八月
		RTCx->msgSoftRTC.month = 8;
	}
	else if (__DATE__[2] == 'p')
	{
		//---九月
		RTCx->msgSoftRTC.month = 9;
	}
	else if (__DATE__[2] == 't')
	{
		//---十月
		RTCx->msgSoftRTC.month = 10;
	}
	else if (__DATE__[2] == 'v')
	{
		//---十一月
		RTCx->msgSoftRTC.month = 11;
	}
	else if (__DATE__[2] == 'c')
	{
		//---十二月
		RTCx->msgSoftRTC.month = 12;
	}
	else
	{
		//---默认是一月
		RTCx->msgSoftRTC.month = 1;
	}

	//---年
	if (__DATE__[9]==0x20)
	{
		RTCx->msgSoftRTC.year = (UINT8_T)((__DATE__[10] - '0'));
	}
	else
	{
		RTCx->msgSoftRTC.year = (UINT8_T)((__DATE__[9] - '0') * 10 + (__DATE__[10] - '0'));
	}
	
	//---日
	if (__DATE__[4] == 0x20)
	{
		RTCx->msgSoftRTC.day = (UINT8_T)(__DATE__[5] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.day = (UINT8_T)((__DATE__[4] - '0') * 10 + (__DATE__[5] - '0'));
	}

	//---世纪
	if (__DATE__[7] == 0x20)
	{
		RTCx->msgSoftRTC.century = (UINT8_T)((__DATE__[8] - '0')+ 1);
	}
	else
	{
		RTCx->msgSoftRTC.century = (UINT8_T)((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0') + 1);
	}
	
	//---计算星期
	RTCx->msgSoftRTC.week = SysRTC_CalcWeekDay(&(RTCx->msgSoftRTC));

	//---取编译日期---时分秒---__TIME__=06:17:05

	//---时
	if (__TIME__[0]==0x20)
	{
		RTCx->msgSoftRTC.hour = (UINT8_T)(__TIME__[1] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.hour = (UINT8_T)((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'));
	}

	//---分
	if (__TIME__[3] == 0x20)
	{
		RTCx->msgSoftRTC.minute = (UINT8_T)(__TIME__[4] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.minute = (UINT8_T)((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'));
	}

	//---秒
	if (__TIME__[6] == 0x20)
	{
		RTCx->msgSoftRTC.second = (UINT8_T)(__TIME__[7] - '0');
	}
	else
	{
		RTCx->msgSoftRTC.second = (UINT8_T)((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'));
	}
	
	//---时间格式是24H
	RTCx->msgSoftRTC.time24H = 1;

	//---设定的监控天数
	RTCx->msgWatchSpanDays = spanDays;
	//---时间监控类型
	RTCx->msgWatchMode = watchaMode;
	//---当前时间天数
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	//---当前的月数
	RTCx->msgMonth = RTCx->msgSoftRTC.month;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：RTC时间处理，更新年月日时分秒
//////输入参数: 系统滴答定时器的计数值
//////输出参数: 无
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SysRTC_SoftBuildHandle(Soft_RTC_HandlerType*RTCx, UINT32_T rtcSecond)
{
	UINT32_T cnt = 0;

	//---判断滴答定时是否发生溢出操作
	if (rtcSecond < (RTCx->msgSoftRTC.secondTick))
	{
		cnt = (0xFFFFFFFF - RTCx->msgSoftRTC.secondTick + rtcSecond);
	}
	else
	{
		cnt = rtcSecond - RTCx->msgSoftRTC.secondTick;
	}

	//---计算当前年份==(世纪-1)*100+年
	int iY = RTCx->msgSoftRTC.century - 1;
	iY = (iY * 100) + RTCx->msgSoftRTC.year;

	//---是否发生秒变化
	if (cnt >= 1000)
	{
		//---更新秒滴答计数值
		RTCx->msgSoftRTC.secondTick = rtcSecond;

		//---默认时间基数是1ms，基数1000次，发生秒变化
		RTCx->msgSoftRTC.second += 1;

		//---秒变化
		if (RTCx->msgSoftRTC.second > 60)
		{
			RTCx->msgSoftRTC.minute += 1;
			RTCx->msgSoftRTC.second = 0;

			//---分变化
			if (RTCx->msgSoftRTC.minute > 60)
			{
				RTCx->msgSoftRTC.hour += 1;
				RTCx->msgSoftRTC.minute = 0;

				//---时变化
				if (RTCx->msgSoftRTC.hour > 24)
				{
					//---天变化
					RTCx->msgSoftRTC.day += 1;

					//---星期变化
					RTCx->msgSoftRTC.week += 1;

					//---一星期只有七天
					if (RTCx->msgSoftRTC.week > 7)
					{
						RTCx->msgSoftRTC.week = 1;
					}

					//---清零时变化
					RTCx->msgSoftRTC.hour = 0;

					//---判断是不是二月
					if (RTCx->msgSoftRTC.month == 2)
					{
						//---年和世纪是分开的
						if (YEAR_TYPE(iY) != 0)
						{
							//---天变化
							if (RTCx->msgSoftRTC.day > 29)
							{
								RTCx->msgSoftRTC.day = 0;
								RTCx->msgSoftRTC.month += 1;
							}
						}
						else
						{
							//---天变化
							if (RTCx->msgSoftRTC.day > 28)
							{
								RTCx->msgSoftRTC.day = 0;
								RTCx->msgSoftRTC.month += 1;
							}
						}
					}
					else
					{
						//---天变化
						if (RTCx->msgSoftRTC.day > g_MonthDaysTab[RTCx->msgSoftRTC.month])
						{
							RTCx->msgSoftRTC.day = 0;
							RTCx->msgSoftRTC.month += 1;

							//---年变化
							if (RTCx->msgSoftRTC.month > 12)
							{
								RTCx->msgSoftRTC.month = 1;
								RTCx->msgSoftRTC.year += 1;

								//---世纪变化
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
//////函		数：
//////功		能：根据日期判断星期几（使用基姆拉尔森计算公式）
//////输入参数:
//////输出参数: 1---星期一；2---星期二；3---星期三；4---星期四；5---星期五；6---星期六；7---星期天；0---错误
//////说		明：
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

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
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
		//---计算时间脉冲
		_return = (_return * 30) + RTCx->msgSoftRTC.day - RTCx->msgDay;
		//---判断是否到达监控时间的阈值，如果到达，执行复位操作
		if ((_return > RTCx->msgWatchSpanDays) || ((_return == RTCx->msgWatchSpanDays)))
		{
			//---发生复位了，给出标志位，用于其他检查，对于没有eeprom的芯片，需要增加增加eeprom的写入操作
			RTCx->msgWatchMode = 1;
			//---软件复位
			SOFT_RESET();
			////---硬件复位,等待看门狗启动
			//while (1);
		}
		_return = OK_0;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 刷新监控
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_RefreshSoftWatch(Soft_RTC_HandlerType* RTCx)
{
	RTCx->msgMonth=RTCx->msgSoftRTC.month;
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 设置监控
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_SetSoftWatch(Soft_RTC_HandlerType* RTCx,UINT16_T spanDays)
{
	//---设定的监控天数
	RTCx->msgWatchSpanDays = spanDays;
	RTCx->msgWatchMode = 0;
	//---当前时间天数
	RTCx->msgDay = RTCx->msgSoftRTC.day;
	//---当前的月数
	RTCx->msgMonth = RTCx->msgSoftRTC.month;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除监控
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_ClearSoftWatch(Soft_RTC_HandlerType* RTCx)
{
	RTCx->msgWatchMode = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T SysRTC_SoftWatch(Soft_RTC_HandlerType* RTCx)
{
	if (RTCx->msgWatchMode==1)
	{
		//---实时监控
		SysRTC_RealTimeSoftWatch(RTCx);
	}
	else if (RTCx->msgWatchMode==2)
	{
		//---刷新监控
		SysRTC_RefreshSoftWatch(RTCx);
	}
	return OK_0;
}