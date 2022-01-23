(*********************************************************************************
 * File: PathPlan.typ
 * Author: Tyler Matijevich
 * Created: 2020-04-06
 *********************************************************************************)

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
		PATH_ERROR_NONE := 0, (*Successful completion*)
		PATH_ERROR_VELOCITYLIMIT := 10, (*Implausible velocity limits*)
		PATH_ERROR_VELOCITYENDPT := 11, (*Velocity endpoints exceed limits*)
		PATH_ERROR_NONPOSITIVE := 12, (*Non-positive time, distance, or acceleration input*)
		PATH_ERROR_MOVELIMIT := 13, (*Input values require impossible or infinite acceleration magnitude*)
		PATH_ERROR_NOSOLUTION2ND := 20, (*2nd order polynomial has imaginary roots*)
		PATH_ERROR_NOSOLUTION1ST := 21, (*1st order polynomianl has no roots*)
		PATH_ERROR_POINTLIMIT := 30, (*Number of points exceeds range [2, PATH_POINTS_INDEX + 1]*)
		PATH_ERROR_TIMESEQUENCE := 31, (*Time points are not sequential*)
		PATH_ERROR_TIMELIMIT := 32, (*Request time exceeds time points*)
		PATH_ERROR_JERKLIMIT := 33 (*Jerk factor exceeds range [1.0, 2.0]*)
		);
	PathPlanBaseSolutionType : 	STRUCT  (*Solution structure for base functions*)
		t_ : ARRAY[0..3]OF LREAL; (*[s] Time points, t_[0] always 0.0*)
		dx : LREAL; (*[Units] Distance*)
		v_ : ARRAY[0..3]OF LREAL; (*[Units/s] Velocity points, v_[0] = v_0 and v_[3] = v_f*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		move : PathPlanMoveEnum; (*Movement type*)
	END_STRUCT;
	PathPlanRootsSolutionType : 	STRUCT  (*2nd order (quadratic) roots solution *)
		r_1 : LREAL; (*First real root*)
		r_2 : LREAL; (*Second real root*)
	END_STRUCT;
	PathPlanInTimeSolutionType : 	STRUCT  (*Solution structure for time difference functions*)
		accDec : PathPlanInTimePathType; (*Acc/Dec profile*)
		decAcc : PathPlanInTimePathType; (*Dec/Acc profile*)
		dt_tilde : LREAL; (*[s] Difference in time durations*)
	END_STRUCT;
	PathPlanInTimePathType : 	STRUCT  (*Path structure for time difference functions*)
		t_ : ARRAY[0..4]OF LREAL; (*[s] Time points*)
		dx : LREAL; (*[Units] Distance*)
		v_ : ARRAY[0..4]OF LREAL; (*[Units/s] Velocity points*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		move : PathPlanMoveEnum; (*Move type*)
	END_STRUCT;
	PathPlanPointSolutionType : 	STRUCT  (*Interpolated point solution of linear segments with parabolic blends*)
		x : LREAL; (*[Units] Position*)
		v : LREAL; (*[Units/s] Velocity*)
		a : LREAL; (*[Units/s^2] Acceleration*)
		j : LREAL; (*[Units/s^3] Jerk*)
	END_STRUCT;
END_TYPE
