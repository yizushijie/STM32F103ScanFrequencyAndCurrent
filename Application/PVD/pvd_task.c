#include "pvd_task.h"
void PVDTask_Init(UINT32_T pwrLevel)
{
	PVDLib_Init(pwrLevel);
}

void PVDTask_DeInit(void)
{
	PVDLib_DeInit();
}

void PVDTask_IRQTask(void)
{
	PVDLib_IRQTask();
}