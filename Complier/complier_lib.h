#ifndef COMPLIER_LIB_H_
#define COMPLIER_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===头文件定义
#include "complier_cfg.h"

//===函数定义
	UINT8_T  HexToBcd(UINT8_T hexVal);
	UINT8_T  HexToASCII(UINT8_T hexVal);
	UINT8_T  ASCIIToHex(UINT8_T hexVal);
	UINT8_T  BcdToDec(UINT8_T bcdVal);
	UINT8_T  DecToBcd(UINT8_T decVal);
	UINT8_T  AscSortFun1(UINT8_T *pVal, UINT16_T length);
	UINT8_T  AscSortFun2(UINT16_T *pVal, UINT16_T length);
	UINT8_T  DescSortFun1(UINT8_T *pVal, UINT16_T length);
	UINT8_T  DescSortFun2(UINT16_T *pVal, UINT16_T length);
	UINT16_T CalcAvgFun1(UINT16_T *pVal, UINT16_T length, UINT16_T index);
	UINT16_T CalcAvgFun2(UINT16_T *pVal, UINT16_T length);
	UINT8_T  CalcAvgFun3(UINT8_T *pVal, UINT16_T length, UINT16_T index);
	UINT16_T CalcAvgFun4(UINT8_T *pVal, UINT16_T length);
	UINT8_T  CompareFun1(UINT8_T *p1, UINT8_T *p2, UINT16_T length);
	UINT8_T  CompareFun2(UINT8_T *p1, UINT8_T *p2, UINT16_T index, UINT16_T length);
	UINT8_T  CompareFun3(UINT16_T *p1, UINT16_T *p2, UINT16_T length);
	UINT8_T  CompareFun4(UINT16_T *p1, UINT16_T *p2, UINT16_T index, UINT16_T length);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*COMPLIER_LIB_H */