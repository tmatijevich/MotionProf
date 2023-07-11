
FUNCTION MotionProfAcc : DINT (*Minimum acceleration to move in time over a distance*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		dx : LREAL; (*[units] Distance*)
		v_0 : LREAL; (*[units/s] Initial velocity*)
		v_f : LREAL; (*[units/s] Final velocity*)
		v_min : LREAL; (*[units/s] Minimum velocity*)
		v_max : LREAL; (*[units/s] Maximum velocity*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfBaseOutputType; (*Output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfRoots : DINT (*Find real roots of second order polynomial (quadratic equation)*)
	VAR_INPUT
		p_2 : LREAL; (*2nd order coefficient*)
		p_1 : LREAL; (*1st order coefficient*)
		p_0 : LREAL; (*0th order coefficient*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfRootsOutputType; (*Output*)
	END_VAR
END_FUNCTION
