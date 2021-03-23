
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
