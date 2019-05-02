#include "lcd1602_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_GPIOInit(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(LCD1602_RS_PORT, 1);
	GPIOTask_Clock(LCD1602_RW_PORT, 1);
	GPIOTask_Clock(LCD1602_EN_PORT, 1);
	GPIOTask_Clock(LCD1602_DATA_PORT, 1);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;				//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---LCD1602_RS_BIT的初始化
	GPIO_InitStruct.Pin = LCD1602_RS_BIT;
	LL_GPIO_Init(LCD1602_RS_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(LCD1602_RS_PORT, LCD1602_RS_BIT);

	//---LCD1602_RW_BIT的初始化
	GPIO_InitStruct.Pin = LCD1602_RW_BIT;
	LL_GPIO_Init(LCD1602_RW_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(LCD1602_RW_PORT, LCD1602_RW_BIT);

	//---LCD1602_EN_BIT的初始化
	GPIO_InitStruct.Pin = LCD1602_EN_BIT;
	LL_GPIO_Init(LCD1602_EN_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(LCD1602_EN_PORT, LCD1602_EN_BIT);

	//---LCD1602_DATA_BIT的初始化
	GPIO_InitStruct.Pin = LCD1602_DATA_BITS;
	LL_GPIO_Init(LCD1602_DATA_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(LCD1602_DATA_PORT, LCD1602_DATA_BITS);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明： 判断1602液晶忙，并等待
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_WaitReady(void)
{
	UINT32_T _return = 0;

	//---复位数据总线
	LCD1602_DATA_VAL(0xFF);

	//---RS拉低
	LCD1602_RS_OUT_0;

	//--RW拉高
	LCD1602_RW_OUT_1;
	while (1)
	{
		//---使能EN
		LCD1602_EN_OUT_1;
		DelayTask_ms(5);

		//---读取数据
		_return = LCD1602_DATA_STATE;

		//---拉低使能以便于下一次产生上升沿
		LCD1602_EN_OUT_0;

		//---判断状态字BIT7位是否为1，为1则表示忙，程序等待
		if ((_return&LCD1602_BIT7_MASK) == 0)
		{
			//---退出循环
			break;
		}
	}

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：写LCD1602命令
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_WriteCmd(UINT8_T cmd)
{
	//---判断忙，忙则等待
	LCD1602_WaitReady();
	LCD1602_RS_OUT_0;
	LCD1602_RW_OUT_0;

	//---写入命令
	LCD1602_DATA_VAL(cmd);

	//---拉高使能端 数据被传输到LCD1602内
	LCD1602_EN_OUT_1;

	//---拉低使能以便于下一次产生上升沿
	LCD1602_EN_OUT_0;

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：写LCD1602数据
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_WriteData(UINT8_T val)
{
	//---判断忙，忙则等待
	LCD1602_WaitReady();
	LCD1602_RS_OUT_1;
	LCD1602_RW_OUT_0;

	//---写入数据
	LCD1602_DATA_VAL(val);

	//---拉高使能端 数据被传输到LCD1602内
	LCD1602_EN_OUT_1;

	//---拉低使能以便于下一次产生上升沿
	LCD1602_EN_OUT_0;

	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：清屏操作
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_ClearScreen(void)
{
	return LCD1602_WriteCmd(0x01);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：设置光标位置
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_SetCursor(UINT8_T xPos, UINT8_T yPos)
{
	if (yPos != 0)
	{
		xPos += 0x40;
	}
	return LCD1602_WriteCmd(xPos | 0x80);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：LCD1602显示字符
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_ShowChar(UINT8_T xPos, UINT8_T yPos, UINT8_T dat)
{
	//---设置光标
	LCD1602_SetCursor(xPos, yPos);

	//---显示字符
	return LCD1602_WriteData(dat);
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：LCD1602显示字符串
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_ShowStr(UINT8_T xPos, UINT8_T yPos, UINT8_T *str)
{
	//---设置光标
	LCD1602_SetCursor(xPos, yPos);

	//---显示字符串
	while (*str != '\0')
	{
		LCD1602_WriteData(*str);
		str++;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：LCD1602初始化
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LCD1602_Init(void)
{
	//---GPIO的初始化
	LCD1602_GPIOInit();

	//16*2显示，5*7点阵，8位数据口
	LCD1602_WriteCmd(0x38);

	//开显示，光标关闭
	LCD1602_WriteCmd(0x0C);

	//文字不动，地址自动+1
	LCD1602_WriteCmd(0x06);

	//清屏
	LCD1602_WriteCmd(0x01);
	return OK_0;
}