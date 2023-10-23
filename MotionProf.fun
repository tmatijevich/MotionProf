
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
		Duration : LREAL; (*[s] Time duration*)
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

FUNCTION MotionProfVel : DINT (*Minimum velocity to move with acceleration in time over a distance*)
	VAR_INPUT
		Duration : LREAL; (*[s] Time duration*)
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

FUNCTION MotionProfTimeDiff : DINT (*Maximum difference in time duration between fastest and slowest profiles*)
	VAR_INPUT
		Distance : LREAL; (*[units] Distance*)
		InitialVelocity : LREAL; (*[units/s] Initial velocity*)
		FinalVelocity : LREAL; (*[units/s] Final velocity*)
		MinimumVelocity : LREAL; (*[units/s] Minimum velocity*)
		MaximumVelocity : LREAL; (*[units/s] Maximum velocity*)
		Acceleration : LREAL; (*[units/s/s] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		Output : MotionProfTimeDiffType; (*Output*)
	END_VAR
END_FUNCTION

FUNCTION MotionProfPoint : DINT (*Velocity profile point interpolation*)
	VAR_INPUT
		InitialPosition : LREAL; (*[units] Initial position*)
		TimePoints : ARRAY[0..MOTIONPROF_POINT_MAX] OF LREAL; (*[s] Time points*)
		VelocityPoints : ARRAY[0..MOTIONPROF_POINT_MAX] OF LREAL; (*[units/s] Velocity points*)
		NumberOfPoints : USINT; (*Number of points*)
		Time : LREAL; (*[s] Time interpolation*)
		JerkFactor : LREAL; (*Jerk factor 1.0 infinite jerk 2.0 minimum jerk*)
	END_VAR
	VAR_IN_OUT
		Point : MotionProfPointType; (*Point output*)
	END_VAR
END_FUNCTION
