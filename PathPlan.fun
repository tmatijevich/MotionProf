
FUNCTION PathAcc : DINT (*Determine the minimum acceleration to change velocity in time over a distance*)
	VAR_INPUT
		dt : REAL; (*Time duration*)
		dx : REAL; (*Distance*)
		v_0 : REAL; (*Initial velocity*)
		v_f : REAL; (*Final velocity*)
		v_min : REAL; (*Minimum velocity*)
		v_max : REAL; (*Maximum velocity*)
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

FUNCTION PathAccInTimeDiffWithRise : DINT (*Determine the minimum acceleration of two paths with a difference in time durations. The paths include a rise from standstill.*)
	VAR_INPUT
		dt_tilde : REAL; (*[s] Difference in time durations*)
		dx : REAL; (*[Units] Distance*)
		v_1 : REAL; (*[Units/s] Rise velocity*)
		v_f : REAL; (*[Units/s] Final velocity*)
		v_min : REAL; (*[Units/s] Minimum velocity limit*)
		v_max : REAL; (*[Units/s] Maximum velocity limit*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanTimeDiffSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathRoots : DINT (*Return the real roots of a second order polynomial*)
	VAR_INPUT
		p_2 : REAL; (*2nd order coefficient*)
		p_1 : REAL; (*1st order coefficient*)
		p_0 : REAL; (*0th order coefficient*)
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
