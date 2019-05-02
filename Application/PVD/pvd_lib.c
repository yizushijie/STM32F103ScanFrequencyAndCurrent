#include "pvd_lib.h"

void PVDLib_Init(UINT32_T pwrLevel)
{
	PVD_Init(pwrLevel);
}

void PVDLib_DeInit(void)
{
	PVD_DeInit();
}

void PVDLib_IRQTask(void)
{
	PVD_IRQTask();
}