
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

FUNCTION PathPoint : DINT (*Determine the point on a piecewise linear velocity profile*)
	VAR_INPUT
		x_0 : REAL; (*[Units] Initial position*)
		t_ : ARRAY[0..PATH_POINTS_MAX_INDEX] OF REAL; (*[s] Array of time point values*)
		v_ : ARRAY[0..PATH_POINTS_MAX_INDEX] OF REAL; (*[Units/s] Array of velocity point values*)
		n : USINT; (*Number of points*)
		t : REAL; (*[s] Input time*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanPointSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION
