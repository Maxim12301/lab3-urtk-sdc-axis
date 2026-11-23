#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void init_regulators(void);
void axis_X_init(void);
void axis_X_cyclic(void);

void _INIT ProgramInit(void)
{
	axis_X_init();
	init_regulators();

	axis_X.reset_error = 1;
	pwm_period = 200;
}

void _CYCLIC ProgramCyclic(void)
{
	BOOL ctrl_on;
	BOOL hand_pos;
	BOOL hand_neg;

	/* питание обмотки возбуждения по тумблеру */
	coil_pwm_value = coil_powered ? 32767 : 0;

	/* LifeCnt, время цикла, счётчик и концевики для SDC-оси */
	axis_X_cyclic();

	/* ручное управление кнопками разрешено только при выключенном контроллере */
	ctrl_on  = (Axis_X.controller.status == ncON);
	hand_pos = pos_hand_btn && !ctrl_on;
	hand_neg = neg_hand_btn && !ctrl_on;

	/* после успешного реферирования ось переводится в состояние «отключено» */
	if (Axis_X.move.mode == ncHOMING)
		homing_active = 1;
	else if (homing_active && Axis_X.move.mode == ncOFF)
	{
		homing_active = 0;
		if (Axis_X.move.homing.status.ok && ctrl_on)
			ncaction((UDINT)&Axis_X, ncCONTROLLER, ncSWITCH_OFF);
	}

	/* регулятор скорости: задание SDC-оси (unit/s) минус скорость оси */
	fb_regulator_X.e = Axis_X_DrvIf.oSetPos * 6500.0f / 32767.0f - axis_X.speed;
	FB_Regulator(&fb_regulator_X);
	axis_X.u = fb_regulator_X.u;

	/* ручной режим и блокировка по концевикам */
	if (!coil_powered)
		axis_X.u = 0.0f;
	else if (hand_pos && !axis_X.endswitch_a_reached)
		axis_X.u = 12.0f;
	else if (hand_neg && !axis_X.endswitch_b_reached)
		axis_X.u = -12.0f;

	FB_Axis(&axis_X);
	if (axis_X.u > -4.0f && axis_X.u < 4.0f) /* слишком малое напряжение не проворачивает двигатель */
		axis_X.pwm_value = 0;

	/* в ручном режиме и без питания регулятор сбрасывается */
	if (hand_pos || hand_neg || !coil_powered)
	{
		axis_X.speed = 0.0f;
		fb_regulator_X.integrator.out = 0.0f;
		fb_regulator_X.iyOld = 0.0f;
	}
}

void _EXIT ProgramExit(void)
{
}

void init_regulators(void)
{
	fb_regulator_X.dt = 0.002f;
	fb_regulator_X.max_abs_value = 24;
	fb_regulator_X.k_i = 0.16f;
	fb_regulator_X.k_p = 0.0f;
	fb_regulator_X.iyOld = 0.0f;
}

void axis_X_init(void)
{
	axis_X.dt = 0.002f;

	/* типы SDC-модулей */
	Axis_X_HW.EncIf1_Typ = ncSDC_ENC16;
	Axis_X_HW.DiDoIf_Typ = ncSDC_DIDO;
	Axis_X_HW.DrvIf_Typ  = ncSDC_DRVSERVO16;

	/* имена переменных интерфейсов */
	strcpy(Axis_X_HW.EncIf1_Name, "Axis_X_EncIf");
	strcpy(Axis_X_HW.DrvIf_Name,  "Axis_X_DrvIf");
	strcpy(Axis_X_HW.DiDoIf_Name, "Axis_X_DiDoIf");

	/* входы готовности и нормальной работы */
	Axis_X_EncIf.iEncOK = 1;
	Axis_X_DrvIf.iDrvOK = 1;
	Axis_X_DrvIf.iStatusEnable = 1;
	Axis_X_DiDoIf.iDriveReady = 1;
}

void axis_X_cyclic(void)
{
	Axis_X_EncIf.iLifeCnt++;
	Axis_X_DiDoIf.iLifeCntDriveEnable++;
	Axis_X_DiDoIf.iLifeCntDriveReady++;
	Axis_X_DiDoIf.iLifeCntNegHwEnd++;
	Axis_X_DiDoIf.iLifeCntPosHwEnd++;
	Axis_X_DiDoIf.iLifeCntReference++;
	Axis_X_DrvIf.iLifeCnt++;
	Axis_X_EncIf.iActTime = (INT)AsIOTimeCyclicStart();

	Axis_X_EncIf.iActPos = axis_X.counter;
	Axis_X_DiDoIf.iPosHwEnd = axis_X.endswitch_a_reached;
	Axis_X_DiDoIf.iNegHwEnd = axis_X.endswitch_b_reached;
	Axis_X_DiDoIf.iDriveReady = coil_powered;
}
