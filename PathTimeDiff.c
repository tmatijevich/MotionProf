/*********************************************************************************
 * File:      PathTimeDiff.c
 * Author:    Tyler Matijevich 
 * Created:   2020-03-25 
 *********************************************************************************/ 

#include <PathPlan.h>
#include "PathPlanMain.h"

/* Difference in durations between fastest and slowest possible profiles */
DINT PathTimeDiff(LREAL dx, LREAL v_0, LREAL v_f, LREAL v_min, LREAL v_max, LREAL a, struct PathPlanInTimeSolutionType* solution) {

	/* Reset the solution */
	USINT i;
	for(i = 0; i <= 4; i++) {
		solution->accDec.t_[i] = 0.0;
		solution->accDec.v_[i] = 0.0;
		solution->decAcc.t_[i] = 0.0;
		solution->decAcc.v_[i] = 0.0;
	}
	solution->accDec.dx 	= 0.0;
	solution->accDec.a 		= 0.0;
	solution->accDec.move 	= PATH_MOVE_NONE;
	solution->decAcc.dx 	= 0.0;
	solution->decAcc.a 		= 0.0;
	solution->decAcc.move 	= PATH_MOVE_NONE;
	
	/* Input requirements */
	// #1 Plausible velocity limits
	if ((v_min <= 0.0) || (v_max <= v_min)) { // Require a strictly positive v_min
		return PATH_ERROR_VELOCITYLIMIT;
	}
	
	// #2 Valid endpoint velocities
	else if ((v_0 < v_min) || (v_0 > v_max) || (v_f < v_min) || (v_f > v_max)) {
		return PATH_ERROR_VELOCITYENDPT;
	}
	
	// #3 Positive inputs
	else if ((dx <= 0.0) || (a <= 0.0)) {
		return PATH_ERROR_NONPOSITIVE;
	}
	
	// #4 Plausible move
	else if (dx < fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a)) {
		return PATH_ERROR_MOVELIMIT;
	}
	
	// Time minimizing profile
	LREAL dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
	LREAL v_p, dt_12;
	
	if (dx < dx_u) {
		solution->accDec.move = PATH_MOVE_ACCDECPEAK;
		
		// Peak velocity
		v_p = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
		
		solution->accDec.t_[1] = (v_p - v_0) / a;
		solution->accDec.t_[2] = solution->accDec.t_[1];
		solution->accDec.t_[3] = solution->accDec.t_[2] + (v_p - v_f) / a;
		solution->accDec.v_[1] = v_p;
		solution->accDec.v_[2] = v_p;
		solution->accDec.v_[3] = v_f;
	}
	else {
		solution->accDec.move = PATH_MOVE_ACCDECSATURATED;
		
		// Time at velocity limit
		dt_12 = (dx - dx_u) / v_max;
		
		solution->accDec.t_[1] = (v_max - v_0) / a;
		solution->accDec.t_[2] = solution->accDec.t_[1] + dt_12;
		solution->accDec.t_[3] = solution->accDec.t_[2] + (v_max - v_f) / a;
		solution->accDec.v_[1] = v_max;
		solution->accDec.v_[2] = v_max;
		solution->accDec.v_[3] = v_f;
	}
	
	// Time maximizing profile
	LREAL dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a);
	
	if (dx < dx_l) {
		solution->decAcc.move = PATH_MOVE_DECACCPEAK;
		
		// Peak velocity
		v_p = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
		
		solution->decAcc.t_[1] = (v_0 - v_p) / a;
		solution->decAcc.t_[2] = solution->decAcc.t_[1];
		solution->decAcc.t_[3] = solution->decAcc.t_[2] + (v_f - v_p) / a;
		solution->decAcc.v_[1] = v_p;
		solution->decAcc.v_[2] = v_p;
		solution->decAcc.v_[3] = v_f;
	}
	else {
		solution->decAcc.move = PATH_MOVE_DECACCSATURATED;
		
		// Time at velocity limit
		dt_12 = (dx - dx_l) / v_min;
		
		solution->decAcc.t_[1] = (v_0 - v_min) / a;
		solution->decAcc.t_[2] = solution->decAcc.t_[1] + dt_12;
		solution->decAcc.t_[3] = solution->decAcc.t_[2] + (v_f - v_min) / a;
		solution->decAcc.v_[1] = v_min;
		solution->decAcc.v_[2] = v_min;
		solution->decAcc.v_[3] = v_f;
	}
	
	/* Common solution values */
	solution->accDec.dx 	= dx;
	solution->accDec.v_[0] 	= v_0;
	solution->decAcc.a 		= a;
	solution->decAcc.dx 	= dx;
	solution->decAcc.v_[0] 	= v_0;
	solution->decAcc.a 		= a;
	solution->dt_tilde 		= solution->decAcc.t_[3] - solution->accDec.t_[3];
	
	return PATH_ERROR_NONE;

} // End function
