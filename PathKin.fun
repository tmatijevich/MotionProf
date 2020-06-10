
FUNCTION Kin_GetAcc : USINT
	VAR_INPUT
		dt : REAL;
		dx : REAL;
		v0 : REAL;
		vf : REAL;
		vmin : REAL;
		vmax : REAL;
	END_VAR
	VAR_IN_OUT
		soln : KinGetAccSoln_typ;
	END_VAR
END_FUNCTION

FUNCTION Kin_GetAccInTimespanPlus : BOOL
	VAR_INPUT
		tdiff : REAL;
		dx : REAL;
		v1 : REAL;
		vf : REAL;
		vmin : REAL;
		vmax : REAL;
	END_VAR
	VAR_IN_OUT
		soln : KinGetAccInTimespanSoln_typ;
	END_VAR
END_FUNCTION

FUNCTION Kin_GetTimespan : BOOL
	VAR_INPUT
		dx : REAL;
		v0 : REAL;
		vf : REAL;
		vmin : REAL;
		vmax : REAL;
		a : REAL;
	END_VAR
	VAR_IN_OUT
		soln : KinGetTimespanSoln_typ;
	END_VAR
END_FUNCTION

FUNCTION Kin_GetVelProfPoint : USINT
	VAR_INPUT
		x0 : REAL;
		timePoints : ARRAY[0..KIN_MAX_PROF_POINTS] OF REAL;
		velPoints : ARRAY[0..KIN_MAX_PROF_POINTS] OF REAL;
		n : USINT;
		t : REAL;
	END_VAR
	VAR_IN_OUT
		soln : KinGetVelProfPointSoln_typ;
	END_VAR
END_FUNCTION
