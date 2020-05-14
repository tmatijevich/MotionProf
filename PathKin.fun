
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
