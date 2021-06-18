
TYPE
	PathPlanMoveEnum : 
		( (*Profile movement type*)
		PATH_MOVE_NONE := 0, (*Undefined movement*)
		PATH_MOVE_DECACCPEAK := 1, (*Profile decelerates then accelerates*)
		PATH_MOVE_DECACCSATURATED := 2, (*Profile decelerates, saturates, then accelerates*)
		PATH_MOVE_ACCDECPEAK := 10, (*Profile accelerates then decelerates*)
		PATH_MOVE_ACCDECSATURATED := 20, (*Profile accelerates, saturates, then decelerates*)
		PATH_MOVE_DECDEC := 30, (*Profile decelerates, holds velocity, then decelerates*)
		PATH_MOVE_ACCACC := 40 (*Profile accelerates, holds velocity, then accelerates*)
		);
	PathPlanErrorEnum : 
		( (*Return code for all functions*)
		PATH_ERROR_NONE, (*Successful function completion*)
		PATH_ERROR_VELOCITYLIMIT := 10, (*Implausible velocity limits*)
		PATH_ERROR_VELOCITYENDPT := 11, (*Velocity endpoints exceed limits*)
		PATH_ERROR_NONPOSITIVE := 12, (*Time, distance, or acceleration inputs are non-positive or a computed value is non-positive*)
		PATH_ERROR_MOVELIMIT := 13, (*Function inputs suggest infinite acceleration or acceleration beyond what is specified*)
		PATH_ERROR_NOSOLUTION2ND := 20, (*2nd order polynomial has imaginary roots*)
		PATH_ERROR_NOSOLUTION1ST := 21, (*1st order polynomianl has no roots*)
		PATH_ERROR_POINTLIMIT := 30, (*Number of points exceeds range [2, PATH_POINTS_MAX_INDEX]*)
		PATH_ERROR_TIMESEQUENCE := 31, (*Timepoints array is not sequential*)
		PATH_ERROR_TIMELIMIT := 32 (*The requested time extends beyond the timepoints array*)
		);
	PathPlanBaseSolutionType : 	STRUCT  (*Solution structure for base functions*)
		t_ : ARRAY[0..3]OF LREAL; (*[s] Time point array, always starting with 0.0 seconds*)
		dx : LREAL; (*[Units] Distance*)
		v_ : ARRAY[0..3]OF LREAL; (*[Units/s] Velocity point array, starting with v_0 and ending with v_f*)
		a : LREAL; (*[Units/s^2] Acceleration*)
		move : PathPlanMoveEnum; (*Movement type*)
	END_STRUCT;
	PathPlanRootsSolutionType : 	STRUCT  (*Solution structure for quardratic equation*)
		r_1 : LREAL; (*First real root of second order polynomial*)
		r_2 : LREAL; (*Second real root of second order polynomial*)
	END_STRUCT;
	PathPlanInTimeSolutionType : 	STRUCT  (*Solution structure for time difference functions*)
		accDec : PathPlanInTimePathType; (*AccDec profile*)
		decAcc : PathPlanInTimePathType; (*DecAcc profile*)
	END_STRUCT;
	PathPlanInTimePathType : 	STRUCT  (*Path structure for time difference functions*)
		t_ : ARRAY[0..4]OF LREAL; (*[s] Time point array*)
		dx : LREAL; (*[Units] Distance*)
		v_ : ARRAY[0..4]OF LREAL; (*[Units/s] Velocity point array*)
		a : LREAL; (*[Units/s^2] Acceleration*)
		move : PathPlanMoveEnum; (*Move type*)
	END_STRUCT;
	PathPlanPointSolutionType : 	STRUCT  (*Linear interpolation solution of a piecewise velocity profile*)
		x : LREAL; (*[Units] Position*)
		v : LREAL; (*[Units/s] Velocity*)
		a : LREAL; (*[Units/s^2] Acceleration*)
	END_STRUCT;
END_TYPE
