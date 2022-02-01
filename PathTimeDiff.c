/*******************************************************************************
 * File:      PathTimeDiff.c
 * Author:    Tyler Matijevich
 * Created:   2020-03-25
*******************************************************************************/

#include "PathPlanMain.h"

/* Difference in time durations between fastest and slowest moves */
long PathTimeDiff(double dx, double v_0, double v_f, double v_min, double v_max, double a, PathPlanInTimeSolutionType *solution) {
	
	/* Declare local variables */
	double dx_l, dt_12;
	long timeResult;
	PathPlanBaseSolutionType timeSolution;
	
	/* Reset the solution */
	memset(solution, 0, sizeof(*solution));
	
	/* Call PathTime for fastest path, checks inputs */
	timeResult = PathTime(dx, v_0, v_f, v_min, v_max, a, &timeSolution);
	if(timeResult != PATH_ERROR_NONE)
		return timeResult;
		
	/* Copy time-minimizing profile */
	solution->accDec.move = timeSolution.move;
	solution->accDec.v_[1] = timeSolution.v_[1];
	solution->accDec.v_[2] = timeSolution.v_[2];
	solution->accDec.t_[1] = timeSolution.t_[1];
	solution->accDec.t_[2] = timeSolution.t_[2];
	solution->accDec.t_[3] = timeSolution.t_[3];
	
	/* Time-maximizing profile */
	dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a);
	/* Saturated? */
	if(dx < dx_l) { /* Peak (dip) */
		solution->decAcc.move = PATH_MOVE_DECACCPEAK;
		
		/* Compute v_12 */
		solution->decAcc.v_[1] = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
		solution->decAcc.v_[2] = solution->decAcc.v_[1];
		solution->decAcc.t_[1] = (v_0 - solution->decAcc.v_[1]) / a;
		solution->decAcc.t_[2] = solution->decAcc.t_[1];
		solution->decAcc.t_[3] = solution->decAcc.t_[2] + (v_f - solution->decAcc.v_[2]) / a;
	}
	else { /* Saturated */
		solution->decAcc.move = PATH_MOVE_DECACCSATURATED;
		
		/* Compute time at v_min */
		dt_12 = (dx - dx_l) / v_min;
		solution->decAcc.v_[1] = v_min;
		solution->decAcc.v_[2] = v_min;
		solution->decAcc.t_[1] = (v_0 - v_min) / a;
		solution->decAcc.t_[2] = solution->decAcc.t_[1] + dt_12;
		solution->decAcc.t_[3] = solution->decAcc.t_[2] + (v_f - v_min) / a;
	}
	
	/* Set remaining solution */
	solution->accDec.dx	 	= dx;
	solution->accDec.v_[0] 	= v_0;
	solution->accDec.v_[3] 	= v_f;
	solution->accDec.a 		= a;
	solution->decAcc.dx	 	= dx;
	solution->decAcc.v_[0] 	= v_0;
	solution->decAcc.v_[3] 	= v_f;
	solution->decAcc.a 		= a;
	solution->dt_tilde 		= solution->decAcc.t_[3] - solution->accDec.t_[3];
	
	return PATH_ERROR_NONE;
	
} /* Function defintion */
