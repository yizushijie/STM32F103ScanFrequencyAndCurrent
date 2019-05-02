#ifndef LCD1602_CFG_H_
#define LCD1602_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "delay_task.h"
	/////////////////////////////////////////////////////////////////////////////////////
	//===lcd1602使用的端口定义
	/////////////////////////////////////////////////////////////////////////////////////
	//===RS端口
	#define LCD1602_RS_PORT				GPIOB
	#define LCD1602_RS_BIT				LL_GPIO_PIN_14
	#define LCD1602_RS_STATE			GPIO_GET_STATE( LCD1602_RS_PORT,LCD1602_RS_BIT )
	#define LCD1602_RS_WRITE			GPIO_SET_WRITE( LCD1602_RS_PORT,LCD1602_RS_BIT )
	#define LCD1602_RS_READ				GPIO_SET_READ(  LCD1602_RS_PORT,LCD1602_RS_BIT )
	#define LCD1602_RS_OUT_0			GPIO_OUT_0(     LCD1602_RS_PORT,LCD1602_RS_BIT )
	#define LCD1602_RS_OUT_1			GPIO_OUT_1(     LCD1602_RS_PORT,LCD1602_RS_BIT )
	#define LCD1602_RS_OUT_C			GPIO_OUT_C(     LCD1602_RS_PORT,LCD1602_RS_BIT )
	
	//===RW端口
	#define LCD1602_RW_PORT				GPIOB
	#define LCD1602_RW_BIT				LL_GPIO_PIN_14
	#define LCD1602_RW_STATE			GPIO_GET_STATE( LCD1602_RW_PORT,LCD1602_RW_BIT )
	#define LCD1602_RW_WRITE			GPIO_SET_WRITE( LCD1602_RW_PORT,LCD1602_RW_BIT )
	#define LCD1602_RW_READ				GPIO_SET_READ(  LCD1602_RW_PORT,LCD1602_RW_BIT )
	#define LCD1602_RW_OUT_0			GPIO_OUT_0(     LCD1602_RW_PORT,LCD1602_RW_BIT )
	#define LCD1602_RW_OUT_1			GPIO_OUT_1(     LCD1602_RW_PORT,LCD1602_RW_BIT )
	#define LCD1602_RW_OUT_C			GPIO_OUT_C(     LCD1602_RW_PORT,LCD1602_RW_BIT )
	
	//===EN端口
	#define LCD1602_EN_PORT				GPIOB
	#define LCD1602_EN_BIT				LL_GPIO_PIN_14
	#define LCD1602_EN_STATE			GPIO_GET_STATE( LCD1602_EN_PORT,LCD1602_EN_BIT )
	#define LCD1602_EN_WRITE			GPIO_SET_WRITE( LCD1602_EN_PORT,LCD1602_EN_BIT )
	#define LCD1602_EN_READ				GPIO_SET_READ(  LCD1602_EN_PORT,LCD1602_EN_BIT )
	#define LCD1602_EN_OUT_0			GPIO_OUT_0(     LCD1602_EN_PORT,LCD1602_EN_BIT )
	#define LCD1602_EN_OUT_1			GPIO_OUT_1(     LCD1602_EN_PORT,LCD1602_EN_BIT )
	#define LCD1602_EN_OUT_C			GPIO_OUT_C(     LCD1602_EN_PORT,LCD1602_EN_BIT )
	
	//===DATA端口,数据端口
	#define LCD1602_DATA_PORT			GPIOB
	#define LCD1602_DATA_BITS			LL_GPIO_PIN_14
	#define LCD1602_BIT7_MASK			0x80
	#define LCD1602_DATA_STATE			GPIO_GET_STATE(  LCD1602_DATA_PORT,LCD1602_DATA_BITS )
	#define LCD1602_DATA_WRITE			GPIO_SET_WRITE(  LCD1602_DATA_PORT,LCD1602_DATA_BITS )
	#define LCD1602_DATA_READ			GPIO_SET_READ(   LCD1602_DATA_PORT,LCD1602_DATA_BITS )
	#define LCD1602_DATA_VAL(dat)		GPIO_H8BITS_OUT( LCD1602_DATA_PORT,dat )
	
	//===函数定义
	UINT8_T LCD1602_Init(void);
	UINT8_T LCD1602_WriteCmd(UINT8_T cmd);
	UINT8_T LCD1602_WriteData(UINT8_T val);
	UINT8_T LCD1602_SetCursor(UINT8_T xPos, UINT8_T yPos);
	UINT8_T LCD1602_ShowChar(UINT8_T xPos, UINT8_T yPos, UINT8_T dat);
	UINT8_T LCD1602_ShowStr(UINT8_T xPos, UINT8_T yPos, UINT8_T *str);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*LCD1602_CFG_H_ */