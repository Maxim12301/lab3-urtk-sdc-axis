#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MotorContr.h"
#ifdef __cplusplus
	};
#endif

/* Интегратор (метод Эйлера) */
void FB_Integrator(struct FB_Integrator* inst)
{
	inst->out = inst->in * inst->dt + inst->out;
}
