/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1794913840_2_
#define _BUR_1794913840_2_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
#else
#endif


/* Variables */
_BUR_LOCAL plcbit pos_hand_btn;
_BUR_LOCAL plcbit neg_hand_btn;
_BUR_LOCAL struct FB_Integrator fb_integrator;
_BUR_LOCAL unsigned short pwm_period;
_BUR_LOCAL struct FB_Regulator fb_regulator;
_BUR_LOCAL struct FB_Motor fb_motor;
_BUR_LOCAL signed short coil_pwm_value;
_BUR_LOCAL plcbit coil_powered;
_BUR_LOCAL struct FB_Axis axis_X;
_BUR_LOCAL plcbit homing_active;





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/SDCAxisCtrIX/Variables.var\\\" scope \\\"local\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/MotorContr/MotorContr.fun\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1794913840_2_ */

