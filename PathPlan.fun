(*********************************************************************************
 * File: PathPlan.fun
 * Author: Tyler Matijevich
 * Created: 2020-04-06
 *********************************************************************************)

FUNCTION PathAcc : DINT (*Minimum acceleration to move in time over a distance*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		dx : LREAL; (*[Units] Distance*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathTime : DINT (*Minimum time to move with acceleration over a distance*)
	VAR_INPUT
		dx : LREAL; (*[Units] Distance*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathDist : DINT (*Maximum distance from move with acceleration in time*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathVel : DINT (*Minimum velocity to move with acceleration in time over a distance*)
	VAR_INPUT
		dt : LREAL; (*[s] Time duration*)
		dx : LREAL; (*[Units] Distance*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		solution : PathPlanBaseSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathTimeDiff : DINT (*Difference in time durations between fastest and slowest moves*)
	VAR_INPUT
		dx : LREAL; (*[Units] Distance*)
		v_0 : LREAL; (*[Units/s] Initial velocity*)
		v_f : LREAL; (*[Units/s] Final velocity*)
		v_min : LREAL; (*[Units/s] Minimum velocity*)
		v_max : LREAL; (*[Units/s] Maximum velocity*)
		a : LREAL; (*[Units/s^2] Acceleration magnitude*)
		solution : PathPlanInTimeSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathRoots : DINT (*Real roots of a second order polynomial*)
	VAR_INPUT
		p_2 : LREAL; (*2nd order coefficient*)
		p_1 : LREAL; (*1st order coefficient*)
		p_0 : LREAL; (*0th order coefficient*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanRootsSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION

FUNCTION PathPoint : DINT (*Position, velocity, and acceleration at a point in time along linear segments with parabolic blends*)
	VAR_INPUT
		x_0 : LREAL; (*[Units] Initial position*)
		t_ : ARRAY[0..PATH_POINT_INDEX] OF LREAL; (*[s] Time points*)
		v_ : ARRAY[0..PATH_POINT_INDEX] OF LREAL; (*[Units/s] Velocity points*)
		n : USINT; (*Number of points*)
		t : LREAL; (*[s] Request time*)
		k : LREAL; (*Jerk factor (peak acceleration gain) from 1.0 (infinite jerk) to 2.0 (minimum jerk)*)
	END_VAR
	VAR_IN_OUT
		solution : PathPlanPointSolutionType; (*Solution reference*)
	END_VAR
END_FUNCTION
