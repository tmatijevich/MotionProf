
FUNCTION GetAcc : DINT (*Determine the minimum acceleration to change velocity in time over a distance*)
	VAR_INPUT
		dt : REAL; (*Time duration*)
		dx : REAL; (*Distance*)
		v0 : REAL; (*Initial velocity*)
		vf : REAL; (*Final velocity*)
		vmin : REAL; (*Minimum velocity*)
		vmax : REAL; (*Maximum velocity*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION GetTimeDur : DINT (*Determine the minimum time duration to change velocity with acceleration over a distance*)
	VAR_INPUT
		dx : REAL; (*Distance*)
		v0 : REAL; (*Initial velocity*)
		vf : REAL; (*Final velocity*)
		vmin : REAL; (*Minimum velocity*)
		vmax : REAL; (*Maximum velocity*)
		a : REAL; (*Acceleration*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION GetDist : DINT (*Determine the maximum distance when changing velocity with acceleration in time*)
	VAR_INPUT
		dt : REAL; (*Time duration*)
		v0 : REAL; (*Initial velocity*)
		vf : REAL; (*Final velocity*)
		vmin : REAL; (*Minimum velocity*)
		vmax : REAL; (*Maximum velocity*)
		a : REAL; (*Acceleration*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION GetVel : DINT (*Determine the minimum intermediate velocity to change velocity with acceleration*)
	VAR_INPUT
		dt : REAL; (*Time duration*)
		dx : REAL; (*Distance*)
		v0 : REAL; (*Initial velocity*)
		vf : REAL; (*Final velocity*)
		vmin : REAL; (*Minimum velocity*)
		vmax : REAL; (*Maximum velocity*)
		a : REAL; (*Acceleration*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION GetTimeDiff : DINT (*Determine the difference between the time minimizing and time maximizing velocity profiles*)
	VAR_INPUT
		dx : REAL; (*Distance*)
		v0 : REAL; (*Initial velocity*)
		vf : REAL; (*Final velocity*)
		vmin : REAL; (*Minimum velocity*)
		vmax : REAL; (*Maximum velocity*)
		a : REAL; (*Acceleration*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanTimeDiffSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION SecondOrderRoots : DINT (*Return the real roots of a second order polynomial*)
	VAR_INPUT
		p2 : REAL; (*2nd order coefficient*)
		p1 : REAL; (*1st order coefficient*)
		p0 : REAL; (*0th order coefficient*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanRootsSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION GetPoint : DINT (*Determine the point on a piecewise linear velocity profile*)
	VAR_INPUT
		InitialPosition : REAL; (*Initial position of the velocity profile*)
		TimePoints : ARRAY[0..PATH_MAX_INDEX_POINTS] OF REAL; (*Array of time point values*)
		VelocityPoints : ARRAY[0..PATH_MAX_INDEX_POINTS] OF REAL; (*Array of velocity point values*)
		NumberOfPoints : USINT; (*Number of points set in the arrays defining the velocity profile*)
		Time : REAL; (*Time input to obtain profile point*)
	END_VAR
	VAR_IN_OUT
		Solution : PathPlanPointSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION
