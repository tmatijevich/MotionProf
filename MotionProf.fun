
FUNCTION MotionProfAcc : DINT (*Minimum acceleration to move in time over a distance*)
	VAR_INPUT
		Duration : LREAL; (*[s] Time duration*)
		Distance : LREAL; (*[units] Distance*)
		InitialVelocity : LREAL; (*[units/s] Initial velocity*)
		FinalVelocity : LREAL; (*[units/s] Final velocity*)
		MinimumVelocity : LREAL; (*[units/s] Minimum velocity*)
		MaximumVelocity : LREAL; (*[units/s] Maximum velocity*)
	END_VAR
	VAR_IN_OUT
		Profile : MotionProfProfileType; (*Profile output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfTime : DINT (*Minimum time duration to move with acceleration over a distance*)
	VAR_INPUT
		Distance : LREAL; (*[units] Distance*)
		InitialVelocity : LREAL; (*[units/s] Initial velocity*)
		FinalVelocity : LREAL; (*[units/s] Final velocity*)
		MinimumVelocity : LREAL; (*[units/s] Minimum velocity*)
		MaximumVelocity : LREAL; (*[units/s] Maximum velocity*)
		Acceleration : LREAL; (*[units/s/s] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		Profile : MotionProfProfileType; (*Profile output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfDist : DINT (*Maximum distance from move with acceleration in time*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		v_0 : LREAL; (*[units/s] Initial velocity*)
		v_f : LREAL; (*[units/s] Final velocity*)
		v_min : LREAL; (*[units/s] Minimum velocity*)
		v_max : LREAL; (*[units/s] Maximum velocity*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfBaseOutputType; (*Output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfVel : DINT (*Minimum velocity to move with acceleration in time over a distance*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		dx : LREAL; (*[units] Distance*)
		v_0 : LREAL; (*[units/s] Initial velocity*)
		v_f : LREAL; (*[units/s] Final velocity*)
		v_min : LREAL; (*[units/s] Minimum velocity*)
		v_max : LREAL; (*[units/s] Maximum velocity*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfBaseOutputType; (*Output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfTimeDiff : DINT (*Maximum difference in time duration between fastest and slowest profiles*)
	VAR_INPUT
		dx : LREAL; (*[units] Distance*)
		v_0 : LREAL; (*[units/s] Initial velocity*)
		v_f : LREAL; (*[units/s] Final velocity*)
		v_min : LREAL; (*[units/s] Minimum velocity*)
		v_max : LREAL; (*[units/s] Maximum velocity*)
		a : LREAL; (*[units/s/s] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfTimeDiffOutputType; (*Output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfPoint : DINT (*Velocity profile point interpolation*)
	VAR_INPUT
		x_0 : LREAL; (*[units] Initial position*)
		t_ : ARRAY[0..MOTIONPROF_POINT_MAX] OF LREAL; (*[s] Time points*)
		v_ : ARRAY[0..MOTIONPROF_POINT_MAX] OF LREAL; (*[units/s] Velocity points*)
		n : USINT; (*Number of points*)
		t : LREAL; (*[s] Time interpolation*)
		k : LREAL; (*Jerk factor 1.0 infinite jerk 2.0 minimum jerk*)
	END_VAR
	VAR_IN_OUT
		output : MotionProfPointOutputType; (*Output*)
	END_VAR
END_FUNCTION
