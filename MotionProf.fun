
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
