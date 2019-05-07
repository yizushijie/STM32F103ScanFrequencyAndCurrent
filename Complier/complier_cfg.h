#ifndef COMPLIER_CFG_H_
#define COMPLIER_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===定义使用的头文件
	#include "stdbool.h"
	#include "stddef.h"
	#include "stdlib.h"
	#include "stdio.h"
	#include "string.h"
	#include "math.h"
	#include "stdarg.h"
	#include "stdint.h"
	
	//===无符号的8位数据
	typedef	unsigned char					UINT8_T;
	typedef	unsigned char					U8;
	typedef	unsigned char					u8;
	typedef	unsigned char					uchar;

	//===volatile无符号的8位数据
	typedef	volatile unsigned char			VLTUINT8_T;
	typedef	volatile unsigned char			vltuint8_t;
	typedef	volatile unsigned char			VLTU8;
	typedef	volatile unsigned char			vltu8;
	typedef	volatile unsigned char			vltuchar;

	//===有符号的8位数据
	typedef	 char							INT8_T;
	typedef	 char							S8;
	typedef	 char							s8;

	//===volatile无符号的8位数据
	typedef	volatile  char					VLTINT8_T;
	typedef	volatile  char					vltint8_t;

	//===兼容不同位数的单片机
	#if (SIZE_MAX==0xFFFF)
	
	//===无符号的16位数据
		typedef	unsigned  int					UINT16_T;
		typedef	unsigned  int					U16;
		typedef	unsigned  int					u16;
	
		typedef	volatile unsigned  int			VLTUINT16_T;
		typedef	volatile unsigned  int			vltuint16_t;
		typedef	volatile unsigned  int			VLTU16;
		typedef	volatile unsigned  int			vltu16;
	
		//===有符号的16位数据
		typedef	signed  int						INT16_T;
		typedef	signed  int						S16;
		typedef	signed  int						s16;
	
		typedef	volatile signed  int			VLTINT16_T;
		typedef	volatile signed  int			vltint16_t;
		typedef	volatile signed  int			VLTS16;
		typedef	volatile signed  int			vlts16;
	
	#else
	
		typedef	unsigned short int				UINT16_T;
		typedef	unsigned short int				U16;
		typedef	unsigned short int				u16;
	
		typedef	volatile unsigned short int		VLTUINT16_T;
		typedef	volatile unsigned short int		vltuint16_t;
		typedef	volatile unsigned short int		VLTU16;
		typedef	volatile unsigned short int		vltu16;
	
		//===有符号的16位数据
		typedef	signed short int				INT16_T;
		typedef	signed short int				S16;
		typedef	signed short int				s16;
	
		typedef	volatile signed short int		VLTINT16_T;
		typedef	volatile signed short int		vltint16_t;
		typedef	volatile signed short int		VLTS16;
		typedef	volatile signed short int		vlts16;
	
	#endif
	
		//===兼容不同位数的单片机
	#if (SIZE_MAX==0xFFFF)
	
	//---无符号的32位数据
		typedef	unsigned long int				UINT32_T;
		typedef	unsigned long int				U32;
		typedef	unsigned long int				u32;
	
		typedef	volatile unsigned long int		VLTUINT32_T;
		typedef	volatile unsigned long int		vltuint32_t;
		typedef	volatile unsigned long int		VLTU32;
		typedef	volatile unsigned long	int		vltu32;
	
		//===有符号的32位数据
		typedef	signed	long int				INT32_T;
		typedef	signed	long int				S32;
		typedef	signed	long int				s32;
	
		typedef	volatile signed	long int		VLTINT32_T;
		typedef	volatile signed	long int		vltint32_t;
		typedef	volatile signed	long int		VLTS32;
		typedef	volatile signed	long int		vlts32;
	
	#else
	
	//===无符号的32位数据
		typedef	unsigned  int					UINT32_T;
		typedef	unsigned  int					U32;
		typedef	unsigned  int					u32;
	
		typedef	volatile unsigned  int			VLTUINT32_T;
		typedef	volatile unsigned  int			vltuint32_t;
		typedef	volatile unsigned  int			VLTU32;
		typedef	volatile unsigned  int			vltu32;
	
		//===有符号的32位数据
		typedef	signed int						INT32_T;
		typedef	signed int						S32;
		typedef	signed int						s32;
	
		typedef	volatile signed	int				VLTINT32_T;
		typedef	volatile signed	int				vltint32_t;
		typedef	volatile signed	int				VLTS32;
		typedef	volatile signed	int				vlts32;
	
	#endif

	//===无符号的64位数据
	typedef	unsigned long long int				UINT64_T;
	typedef	unsigned long long int				U64;
	typedef	unsigned long long int				u64;

	typedef	volatile unsigned long long int		VLTUINT64_T;
	typedef	volatile unsigned long long int		vltuint64_t;
	typedef	volatile unsigned long long int		VLTU64;
	typedef	volatile unsigned long long int		vltu64;

	//===有符号的64位数据
	typedef	signed long long int				INT64_T;
	typedef	signed long long int				S64;
	typedef signed long long int				s64;

	typedef	volatile signed long long int		VLTINT64_T;
	typedef	volatile signed long long int		vltint64_t;
	typedef	volatile signed long long int		VLTS64;
	typedef	volatile signed long long int		vlts64;

	//===8bit无符号的联合体
	typedef union
	{
		struct
		{
			UINT8_T b0 : 1;
			UINT8_T b1 : 1;
			UINT8_T b2 : 1;
			UINT8_T b3 : 1;
			UINT8_T b4 : 1;
			UINT8_T b5 : 1;
			UINT8_T b6 : 1;
			UINT8_T b7 : 1;
		};
		UINT8_T bit;
	}UNION_UINT8_T;

	//===8bit有符号的联合体
	typedef union
	{
		struct
		{
			INT8_T b0 : 1;
			INT8_T b1 : 1;
			INT8_T b2 : 1;
			INT8_T b3 : 1;
			INT8_T b4 : 1;
			INT8_T b5 : 1;
			INT8_T b6 : 1;
			INT8_T b7 : 1;
		};
		INT8_T bit;
	}UNION_INT8_T;

	//===16bit无符号的联合体
	typedef union
	{
		struct
		{
			UINT8_T b0 : 1;
			UINT8_T b1 : 1;
			UINT8_T b2 : 1;
			UINT8_T b3 : 1;
			UINT8_T b4 : 1;
			UINT8_T b5 : 1;
			UINT8_T b6 : 1;
			UINT8_T b7 : 1;
			UINT8_T b8 : 1;
			UINT8_T b9 : 1;
			UINT8_T b10 : 1;
			UINT8_T b11 : 1;
			UINT8_T b12 : 1;
			UINT8_T b13 : 1;
			UINT8_T b14 : 1;
			UINT8_T b15 : 1;
		};
		UINT8_T  bit[2];
		UINT16_T wBit;
	}UNION_UINT16_T;

	//===16bit有符号的联合体
	typedef union
	{
		struct
		{
			INT8_T b0 : 1;
			INT8_T b1 : 1;
			INT8_T b2 : 1;
			INT8_T b3 : 1;
			INT8_T b4 : 1;
			INT8_T b5 : 1;
			INT8_T b6 : 1;
			INT8_T b7 : 1;
			INT8_T b8 : 1;
			INT8_T b9 : 1;
			INT8_T b10 : 1;
			INT8_T b11 : 1;
			INT8_T b12 : 1;
			INT8_T b13 : 1;
			INT8_T b14 : 1;
			INT8_T b15 : 1;
		};
		INT8_T  bit[2];
		INT16_T wBit;
	}UNION_INT16_T;

	//===32bit无符号的联合体
	typedef union
	{
		struct
		{
			UINT8_T b0 : 1;
			UINT8_T b1 : 1;
			UINT8_T b2 : 1;
			UINT8_T b3 : 1;
			UINT8_T b4 : 1;
			UINT8_T b5 : 1;
			UINT8_T b6 : 1;
			UINT8_T b7 : 1;
			UINT8_T b8 : 1;
			UINT8_T b9 : 1;
			UINT8_T b10 : 1;
			UINT8_T b11 : 1;
			UINT8_T b12 : 1;
			UINT8_T b13 : 1;
			UINT8_T b14 : 1;
			UINT8_T b15 : 1;
			UINT8_T b16 : 1;
			UINT8_T b17 : 1;
			UINT8_T b18 : 1;
			UINT8_T b19 : 1;
			UINT8_T b20 : 1;
			UINT8_T b21 : 1;
			UINT8_T b22 : 1;
			UINT8_T b23 : 1;
			UINT8_T b24 : 1;
			UINT8_T b25 : 1;
			UINT8_T b26 : 1;
			UINT8_T b27 : 1;
			UINT8_T b28 : 1;
			UINT8_T b29 : 1;
			UINT8_T b30 : 1;
			UINT8_T b31 : 1;
		};
		UINT8_T  bit[4];
		UINT16_T wBit[2];
		UINT32_T dWBit;
	}UNION_UINT32_T;

	//===32bit有符号的联合体
	typedef union
	{
		struct
		{
			INT8_T b0 : 1;
			INT8_T b1 : 1;
			INT8_T b2 : 1;
			INT8_T b3 : 1;
			INT8_T b4 : 1;
			INT8_T b5 : 1;
			INT8_T b6 : 1;
			INT8_T b7 : 1;
			INT8_T b8 : 1;
			INT8_T b9 : 1;
			INT8_T b10 : 1;
			INT8_T b11 : 1;
			INT8_T b12 : 1;
			INT8_T b13 : 1;
			INT8_T b14 : 1;
			INT8_T b15 : 1;
			INT8_T b16 : 1;
			INT8_T b17 : 1;
			INT8_T b18 : 1;
			INT8_T b19 : 1;
			INT8_T b20 : 1;
			INT8_T b21 : 1;
			INT8_T b22 : 1;
			INT8_T b23 : 1;
			INT8_T b24 : 1;
			INT8_T b25 : 1;
			INT8_T b26 : 1;
			INT8_T b27 : 1;
			INT8_T b28 : 1;
			INT8_T b29 : 1;
			INT8_T b30 : 1;
			INT8_T b31 : 1;
		};
		INT8_T  bit[4];
		INT16_T wBit[2];
		INT32_T dWBit;
	}UNION_INT32_T;

	//===常用错误枚举
	typedef enum
	{
		OK_0 = 0,
		ERROR_1 = 1,
		ERROR_2 = 2,
		ERROR_3 = 3,
		ERROR_4 = 4,
		ERROR_5 = 5,
		ERROR_6 = 6,
		ERROR_7 = 7,
		ERROR_8 = 8,
		ERROR_9 = 9,
		ERROR_10 = 10,
		ERROR_11 = 11,
		ERROR_12 = 12,
		ERROR_13 = 13,
		ERROR_14 = 14,
		ERROR_15 = 15,
		ERROR_16 = 16,
		ERROR_17 = 17,
		ERROR_18 = 18,
		ERROR_19 = 19,
		ERROR_20 = 20,
		ERROR_21 = 21,
		ERROR_22 = 22,
		ERROR_23 = 23,
		ERROR_24 = 24,
		ERROR_25 = 25,
		ERROR_26 = 26,
		ERROR_27 = 27,
		ERROR_28 = 28,
		ERROR_29 = 29,
		ERROR_30 = 30,
		ERROR_31 = 31,
		ERROR_32 = 32,
		ERROR_33 = 33,
		ERROR_34 = 34,
		ERROR_35 = 35,
		ERROR_36 = 36,
		ERROR_37 = 37,
		ERROR_38 = 38,
		ERROR_39 = 39,
		ERROR_40 = 40,
		ERROR_41 = 41,
		ERROR_42 = 42,
		ERROR_43 = 43,
		ERROR_44 = 44,
		ERROR_45 = 45,
		ERROR_46 = 46,
		ERROR_47 = 47,
		ERROR_48 = 48,
		ERROR_49 = 49,
		ERROR_50 = 50,
	}FuncSTATE;

	//===结构体定义
	typedef struct _RTC_HandlerType					RTC_HandlerType;

	//===定义指针结构体
	typedef struct _RTC_HandlerType					*pRTC_HandlerType;

	//===时钟结构体的定义
	struct _RTC_HandlerType
	{
		UINT8_T second;								//---秒
		UINT8_T minute;								//---分
		UINT8_T hour;								//---时
		UINT8_T day;								//---天
		UINT8_T week;								//---星期
		UINT8_T month;								//---月
		UINT8_T year;								//---年
		UINT8_T century;							//---世纪
		UINT8_T time24H;                            //---24H格式。0---12小时；1---24小时
		UINT32_T secondTick;						//---秒变化的记录
	};

	//===常用宏定义
	typedef enum
	{
		IDLE = 0,
		BUSY = 1,
	}WorkSTATE;

	//===定义bool变量
	#if !defined(bool)
		#define false	0
		#define true	1
			typedef enum
			{
				false,
				true,
			}bool;
	#endif
	
		//===两个数据比较大小
	#define MAX(a,b)								( (a) > (b) ? (a) : (b) )
	#define MIN(a,b)								( (a) < (b) ? (a) : (b) )
	
	//===数据的绝对值
	#define ABS(a)									( (a)>=0?(a):-(a) )
	
	//===范围内的最小值
	#define RANGE(x, a, b)							( MIN( MAX( x, a ), b ) )
	
	//===数据的绝对差
	#define ABS_SUB(a,b)							( (a)>(b)?((a)-(b)):((b)-(a)) )
	
	//===利用__DATE__获取当前日期，占用12个字节，包括字符串结束符
	#define	VERSION_DATE_SIZE						12
	
	//===利用__TIME__获取当前时间，占用9个字节，包括字符串结束符
	#define VERSION_TIME_SIZE						9
	
	//===判断是否是闰年
	#define YEAR_TYPE(a)							( ((a%100!=0)&&(a%4==0))||(a%400==0) )
	
	//===使用的单片机的型号
	//#define USE_MCU_AVR
	#define USE_MCU_STM32
	
	#ifdef USE_MCU_STM32
		#include "stm32_cfg.h"
	#endif // USE_MCU_STM32
	
	#ifdef USE_MCU_AVR
		#include "avr_cfg.h"
	#endif
	
	//===外部调用接口
	extern const UINT8_T  g_VersionDate[VERSION_DATE_SIZE];
	extern const UINT8_T  g_VersionTime[VERSION_TIME_SIZE];

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* COMPLIER_CFG_H */