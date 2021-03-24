
TYPE
	PathPlanMoveEnum : 
		(
		PATH_MOVE_NONE := 0,
		PATH_DEC_ACC_PEAK := 1,
		PATH_DEC_ACC_SATURATED := 2,
		PATH_ACC_DEC_PEAK := 10,
		PATH_ACC_DEC_SATURATED := 20,
		PATH_DEC_DEC := 30,
		PATH_ACC_ACC := 40
		);
	PathPlanErrorCode : 
		( (*Return code for all functions*)
		PATH_ERROR_NONE, (*No error has occured*)
		PATH_ERROR_TIME_LIMIT, (*The time duration exceeds possible limits given input values*)
		PATH_ERROR_DIST_LIMIT, (*The distance exceeds possible limits given input values*)
		PATH_ERROR_VEL_LIMITS_INVALID, (*Invalid velocity limits*)
		PATH_ERROR_VEL_ENDPT_LIMIT, (*The velocity endpoint(s) exceeds the specified velocity limits*)
		PATH_ERROR_ACC_LIMIT, (*The given input parameters would require an infinite acceleration to complete the move*)
		PATH_ERROR_NONPOS_INPUT, (*Time duration, distance, and/or acceleration input value is non-positive*)
		PATH_ERROR_HIGHER_ORDER,
		PATH_ERROR_2ND_ORDER_NO_SOLN, (*The 2nd order equation's solution results in imaginary roots*)
		PATH_ERROR_1ST_ORDER_NO_SOLN, (*The 1st order equation has no solution (constant)*)
		PATH_ERROR_POINT_LIMIT,
		PATH_ERROR_SEQ_POINTS,
		PATH_ERROR_TIME_POINT (*The requested time value exceeds the provided velocity profile*)
		);
	PathPlanBaseSolutionType : 	STRUCT 
		t : ARRAY[0..3]OF REAL;
		dx : REAL;
		v : ARRAY[0..3]OF REAL;
		a : REAL;
		Move : PathPlanMoveEnum;
	END_STRUCT;
	PathPlanRootsSolutionType : 	STRUCT  (*Solution structure to a second order equation*)
		r1 : REAL; (*First real root of second order polynomial*)
		r2 : REAL; (*Second real root of second order polynomial*)
	END_STRUCT;
	PathPlanTimeDiffSolutionType : 	STRUCT 
		tA : ARRAY[0..3]OF REAL;
		vA : ARRAY[0..3]OF REAL;
		MoveA : PathPlanMoveEnum;
		tB : ARRAY[0..3]OF REAL;
		vB : ARRAY[0..3]OF REAL;
		MoveB : PathPlanMoveEnum;
		tdiff : REAL;
		a : REAL;
	END_STRUCT;
	PathPlanPointSolutionType : 	STRUCT 
		x : REAL;
		v : REAL;
		a : REAL;
	END_STRUCT;
END_TYPE
