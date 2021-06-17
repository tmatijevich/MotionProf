/*******************************************************************************
 * File: PathAccInTimeDiffWithRise.c
 * Author: Tyler Matijevich
 * Date: 2020-04-01
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the minimum acceleration of two paths with a difference in time durations. The paths include a rise from standstill. */
DINT PathAccRiseInTime(REAL dt_tilde, REAL dx, REAL v_1, REAL v_f, REAL v_min, REAL v_max, struct PathPlanTimeDiffSolutionType* solution) {
	// Reset the solution
	USINT i;
	for(i = 0; i <= 4; i++) {
		solution->accDec.t[i] = 0.0;
		solution->accDec.v[i] = 0.0;
		solution->decAcc.t[i] = 0.0;
		solution->decAcc.v[i] = 0.0;
	}
	solution->accDec.dx 	= 0.0;
	solution->accDec.a 		= 0.0;
	solution->accDec.move 	= PATH_MOVE_NONE;
	solution->decAcc.dx 	= 0.0;
	solution->decAcc.a 		= 0.0;
	solution->decAcc.move 	= PATH_MOVE_NONE;
	
	/* Input requirements */
	// #1: Plausible velocity limits
	if((v_min < 0.0) || (v_max <= v_min)) {
		return PATH_ERROR_VEL_LIMITS_INVALID;
	}
	
	// #2: Endpoint velocities within limits
	else if((v_1 < v_min) || (v_1 > v_max) || (v_f < v_min) || (v_f > v_max)) {
		return PATH_ERROR_VEL_ENDPT_LIMIT;
	}
	
	// #3: Positive time difference and distance
	else if((dt_tilde <= 0.0) || (dx <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4: Valid time difference given distance and velocity limits
	else if(dt_tilde >= (dx / v_min - dx / v_max)) {
		return PATH_ERROR_ACC_LIMIT;
	}
	
	/* Declare limit variables */
	LREAL a_u, a_l, dt_u, dt_l, v_p, dt_23, dt_u_hat, dt_l_hat;
	
	/* Determine the saturated acceleration limit for each profile */
	a_u = (2.0 * pow2(v_max) - pow2(v_f)) / (2.0 * dx); // Acceleration values exceeding this will saturate the acc-dec profile
	a_l = (2.0 * pow2(v_1) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * dx);
	
	/* Determine the saturated time duration limit for each profile */
	dt_u = (2.0 * v_max - v_f) / a_u;
	dt_l = (2.0 * v_1 + v_f - 2.0 * v_min) / a_l;
	
	/* Determine the time duration at the saturated acceleration limit of the alternate profile */
	if(a_u > a_l) {
		// Acc-dec profile is not saturated at a_l
		v_p = sqrt(dx * a_l + pow2(v_f) / 2.0);
		dt_l_hat = (2.0 * v_p - v_f) / a_l;
		
		// Dec-acc profile saturated at a_u
		dt_23 = (dx - (2.0 * pow2(v_1) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a_u)) / v_min;
		dt_l_hat = (2.0 * v_1 + v_f - 2.0 * v_min) / a_u + dt_23;
	} 
//	else {
//		// Acc-dec is saturated at a_l
//		dt_23 = (dx - (2.0 * pow2(v_max) - pow2(v_f)) / (2.0 * a_l)) / v_max;
//		dt_l_hat = (2.0 * v_max - v_f) / a_l + dt_23;
//		
//		// Dec-acc profile is not saturated at a_u
//		v_p = sqrt((2.0
	
} // End function