
FUNCTION Kin_GetAcc : BOOL
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
