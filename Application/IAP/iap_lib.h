#ifndef IAP_LIB_H_
#define IAP_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////

#include"iap_cfg.h"

//===��������
	void IAPLib_ToAPP(UINT32_T appAddr);
	void IAPLib_ToIAP(UINT32_T iapAddr);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*IAP_LIB_H_ */