#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MotorContr.h"
#ifdef __cplusplus
	};
#endif

/* Модель двигателя постоянного тока (из ЛР1) */
void FB_Motor(struct FB_Motor* inst)
{
	inst->w = (inst->u / inst->ke - inst->w) * (inst->dt / inst->Tm) + inst->w;
	inst->integrator.in = inst->w * 3.1415f / 30.0f; /* перевод в рад/с */
	inst->integrator.dt = inst->dt;
	FB_Integrator(&inst->integrator);
	inst->phi = inst->integrator.out;
}
