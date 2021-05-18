
TYPE
	PathPlanMoveEnum : 
		( (*Velocity profile movement type*)
		PATH_MOVE_NONE := 0, (*Undefined*)
		PATH_DEC_ACC_PEAK := 1, (*Velocity profile decelerates then accelerates*)
		PATH_DEC_ACC_SATURATED := 2, (*Velocity profile decelerates, saturates, then accelerates*)
		PATH_ACC_DEC_PEAK := 10, (*Velocity profile accelerates then decelerates*)
		PATH_ACC_DEC_SATURATED := 20, (*Velocity profile accelerates, saturates, then decelerates*)
		PATH_DEC_DEC := 30, (*Velocity profile decelerates, holds velocity, then decelerates*)
		PATH_ACC_ACC := 40 (*Velocity profile accelerates, holds velocity, then accelerates*)
		);
	PathPlanErrorCode : 
		( (*Return code for all functions*)
		PATH_ERROR_NONE, (*No error has occured*)
		PATH_ERROR_TIME_LIMIT, (*The time duration exceeds possible limits*)
		PATH_ERROR_DIST_LIMIT, (*The distance exceeds possible limits*)
		PATH_ERROR_VEL_LIMITS_INVALID, (*Invalid velocity limits*)
		PATH_ERROR_VEL_ENDPT_LIMIT, (*The velocity endpoint(s) exceed the velocity limits*)
		PATH_ERROR_ACC_LIMIT, (*The input parameters require an infinite acceleration*)
		PATH_ERROR_NONPOS_INPUT, (*A input value is non-positive*)
		PATH_ERROR_HIGHER_ORDER,
		PATH_ERROR_2ND_ORDER_NO_SOLN, (*The 2nd order equation's solution results in imaginary roots*)
		PATH_ERROR_1ST_ORDER_NO_SOLN, (*The 1st order equation has no solution*)
		PATH_ERROR_POINT_LIMIT,
		PATH_ERROR_SEQ_POINTS,
		PATH_ERROR_TIME_POINT (*The requested time value exceeds the provided velocity profile*)
		);
	PathPlanBaseSolutionType : 	STRUCT  (*Solution structure for base functions*)
		t : ARRAY[0..3]OF REAL; (*[s] Time point array, always starting with 0.0 seconds*)
		dx : REAL; (*[Units] Distance*)
		v : ARRAY[0..3]OF REAL; (*[Units/s] Velocity point array, starting with v_0 and ending with v_f*)
		a : REAL; (*[Units/s^2] Acceleration*)
		move : PathPlanMoveEnum; (*Movement type*)
	END_STRUCT;
	PathPlanRootsSolutionType : 	STRUCT  (*Solution structure to a second order equation*)
		r_1 : REAL; (*First real root of second order polynomial*)
		r_2 : REAL; (*Second real root of second order polynomial*)
	END_STRUCT;
	PathPlanTimeDiffSolutionType : 	STRUCT  (*Solution structure for time difference functions*)
		tA : ARRAY[0..3]OF REAL; (*Time points of time minimizing profile*)
		vA : ARRAY[0..3]OF REAL; (*Velocity points of time minimizing profile*)
		MoveA : PathPlanMoveEnum; (*Move type of time minimizing profile*)
		tB : ARRAY[0..3]OF REAL; (*Time points of time maximizing profile*)
		vB : ARRAY[0..3]OF REAL; (*Velocity points of time maximizing profile*)
		MoveB : PathPlanMoveEnum; (*Move type of time maximizing profile*)
		tdiff : REAL; (*Difference in time durations of extrema profiles*)
		dx : USINT; (*Distance*)
		a : REAL; (*Acceleration (symmetric)*)
	END_STRUCT;
	PathPlanPointSolutionType : 	STRUCT  (*Linear interpolation solution of a piecewise velocity profile*)
		x : REAL; (*[Units] Position*)
		v : REAL; (*[Units/s] Velocity*)
		a : REAL; (*[Units/s^2] Acceleration*)
	END_STRUCT;
END_TYPE
