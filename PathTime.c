/*********************************************************************************
 * File:      PathTime.c
 * Author:    Tyler Matijevich 
 * Created:   2020-12-23
 *********************************************************************************/ 

#include <PathPlan.h>
#include "PathPlanMain.h"

/* Minimum time duration to move with acceleration over a distance */
DINT PathTime(LREAL dx, LREAL v_0, LREAL v_f, LREAL v_min, LREAL v_max, LREAL a, struct PathPlanBaseSolutionType* solution) {
	
	/* Reset the solution */
	USINT i;
	for(i = 0; i <= 3; i++) {
		solution->t_[i] = 0.0;
		solution->v_[i] = 0.0;
	}
	solution->dx 	= 0.0;
	solution->a 	= 0.0;
	solution->move 	= PATH_MOVE_NONE;
	
	/* Input requirements */
	// #1 Plausible velocity limits
	if((v_min < 0.0) || (v_max <= v_min)) {
		return PATH_ERROR_VELOCITYLIMIT;
	}
	
	// #2 Valid endpoint velocities
	else if((v_0 < v_min) || (v_0 > v_max) || (v_f < v_min) || (v_f > v_max)) {
		return PATH_ERROR_VELOCITYENDPT;
	}
	
	// #3 Positive inputs
	else if((dx <= 0.0) || (a <= 0.0)) {
		return PATH_ERROR_NONPOSITIVE;
	}
	
	// #4 Plausible move
	else if (dx < (fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a))) { // Distance requires higher acceleration
		return PATH_ERROR_MOVELIMIT;
	}
	
	// There is no time advantage to decelerating before the final velocity, therefore only AccDec profiles are considered
	LREAL dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
	
	/* Determine if the profile is saturated */
	LREAL v_1, t_12;
	if(dx < dx_u) { // AccDec with peak
		solution->move = PATH_MOVE_ACCDECPEAK;
		
		// Compute the peak velocity
		v_1 = sqrt(dx * a + (pow2(v_0) + pow2(v_f)) / 2.0);
		
		// Set the solution
		solution->t_[1] = (v_1 - v_0) / a;
		solution->t_[2] = solution->t_[1];
		solution->t_[3] = solution->t_[2] + (v_1 - v_f) / a;
		solution->v_[1] = v_1;
		solution->v_[2] = v_1;	
	} 
	else { // Saturated AccDec
		solution->move = PATH_MOVE_ACCDECSATURATED;
		
		// Compute the time at v_max
		t_12 = (dx - dx_u) / v_max;
		
		// Set the solution
		solution->t_[1] = (v_max - v_0) / a;
		solution->t_[2] = solution->t_[1] + t_12;
		solution->t_[3] = solution->t_[2] + (v_max - v_f) / a;
		solution->v_[1] = v_max;
		solution->v_[2] = v_max;
	} // dx_u?
	
	/* Set common solution values */
	solution->dx 	= dx;
	solution->v_[0] = v_0;
	solution->v_[3] = v_f;
	solution->a 	= a;
	
	return PATH_ERROR_NONE;

} // End function
