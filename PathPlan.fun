
FUNCTION PathAcc : DINT (*Determine the minimum acceleration to change velocity in time over a distance*)
	VAR_INPUT
		dt : LREAL; (*Time duration*)
		dx : LREAL; (*Distance*)
		v_0 : LREAL; (*Initial velocity*)
		v_f : LREAL; (*Final velocity*)
		v_min : LREAL; (*Minimum velocity*)
		v_max : LREAL; (*Maximum velocity*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathTime : DINT (*Minimum time to move with acceleration over a distance.*)
	VAR_INPUT
		dx : LREAL; (*[Units] Distance*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathAccRiseInTime : DINT (*Minimum acceleration to achieve a move with rise in a window of time*)
	VAR_INPUT
		dt_tilde : LREAL; (*[s] Difference in time durations*)
		dx : LREAL; (*[Units] Distance*)
		v_1 : LREAL; (*[Units/s] Rise velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity limit*)
		v_max : LREAL; (*[Units/s] Maximum velocity limit*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanTimeDiffSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathRoots : DINT (*Return the real roots of a second order polynomial*)
	VAR_INPUT
		p_2 : LREAL; (*2nd order coefficient*)
		p_1 : LREAL; (*1st order coefficient*)
		p_0 : LREAL; (*0th order coefficient*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanRootsSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathPoint : DINT (*Position, velocity, and acceleration at a point along a velocity profile.*)
	VAR_INPUT
		x_0 : LREAL; (*[Units] Initial position*)
		t_ : ARRAY[0..PATH_POINTS_MAX_INDEX] OF LREAL; (*[s] Array of time point values*)
		v_ : ARRAY[0..PATH_POINTS_MAX_INDEX] OF LREAL; (*[Units/s] Array of velocity point values*)
		n : USINT; (*Number of points*)
		t : LREAL; (*[s] Input time*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanPointSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION
