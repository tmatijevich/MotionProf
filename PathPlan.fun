
FUNCTION PathPlanRoots : DINT (*Find real roots of second order polynomial (quadratic equation)*)
	VAR_INPUT
		p_2 : LREAL; (*2nd order coefficient*)
		p_1 : LREAL; (*1st order coefficient*)
		p_0 : LREAL; (*0th order coefficient*)
	END_VAR
	VAR_IN_OUT
		output : PathPlanRootsOutputType; (*Output*)
	END_VAR
END_FUNCTION
