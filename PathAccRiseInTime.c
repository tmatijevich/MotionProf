/*******************************************************************************
 * File:      PathAccRiseInTime.c
 * Author:    Tyler Matijevich
 * Date:      2020-04-01
*******************************************************************************/

#include <PathPlan.h>
#include "PathPlanMain.h"

/* Minimum acceleration of a move with rise in a window of time */
DINT PathAccRiseInTime(LREAL dt_tilde, LREAL dx, LREAL v_1, LREAL v_f, LREAL v_min, LREAL v_max, struct PathPlanTimeDiffSolutionType* solution) {
	
	/* Reset the solution */
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
	// #1 Plausible velocity limits
	if((v_min < 0.0) || (v_max <= v_min)) {
		return PATH_ERROR_VEL_LIMITS_INVALID;
	}
	
	// #2 Valid endpoints velocities
	else if((v_1 < v_min) || (v_1 > v_max) || (v_f < v_min) || (v_f > v_max)) {
		return PATH_ERROR_VEL_ENDPT_LIMIT;
	}
	
	// #3 Positive inputs
	else if((dt_tilde <= 0.0) || (dx <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4 Plausible move
	else if(dt_tilde >= (dx / v_min - dx / v_max)) {
		return PATH_ERROR_ACC_LIMIT;
	}
	
	/* Declare limit variables */
	LREAL a_u, a_l, dt_u, dt_l, v_p, dt_23, dt_u_hat, dt_l_hat;
	
	/* Determine the saturated acceleration limit for each profile */
	a_u = (2.0 * pow2(v_max) - pow2(v_f)) / (2.0 * dx); // Acceleration values exceeding this will saturate the AccDec profile
	a_l = (2.0 * pow2(v_1) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * dx);
	
	/* Determine the saturated time duration limit for each profile */
	dt_u = (2.0 * v_max - v_f) / a_u; // Time durations subceeding this limit will saturate the AccDec profile
	dt_l = (2.0 * v_1 + v_f - 2.0 * v_min) / a_l;
	
	/* Determine the time duration at the saturated acceleration limit of the alternate profile */
	if(a_u > a_l) {
		// AccDec profile is not saturated at a_l
		v_p = sqrt(dx * a_l + pow2(v_f) / 2.0);
		dt_l_hat = (2.0 * v_p - v_f) / a_l;
		
		// DecAcc profile is saturated at a_u
		dt_23 = (dx - (2.0 * pow2(v_1) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a_u)) / v_min;
		dt_l_hat = (2.0 * v_1 + v_f - 2.0 * v_min) / a_u + dt_23;
	} 
	else {
		// AccDec profile is saturated at a_l
		dt_23 = (dx - (2.0 * pow2(v_max) - pow2(v_f)) / (2.0 * a_l)) / v_max;
		dt_l_hat = (2.0 * v_max - v_f) / a_l + dt_23;
		
		// DecAcc profile is not saturated at a_u
		v_p = sqrt((2.0 * pow2(v_1) + 2.0 * pow2(v_f)) / 2.0 - dx * a_u);
		dt_u_hat = (2.0 * v_1 + v_f - 2.0 * v_p) / a_u;
	}
	
	/* Determine the time differences at both saturation limits */
	LREAL dt_u_tilde = dt_u_hat - dt_u; // For the same acceleration a_u, the AccDec profile is always quicker than the DecAcc profile
	LREAL dt_l_tilde = dt_l - dt_l_hat;
	
	/* Compute reduction constants */
	LREAL c_dtu = 2.0 * v_max - v_f;
	LREAL c_dxu = (2.0 * pow2(v_max) - pow2(v_f)) / (2.0 * v_max);
	LREAL c_dtl = 2.0 * v_1 + v_f - 2.0 * v_min;
	LREAL c_dxl = (2.0 * pow2(v_1) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * v_min);
	
	/* There are four cases to consider given the time difference */
	// 1. dt_tilde >= dt_u_tilde && dt_tilde >= dt_l_tilde
	// 2. dt_tilde  < dt_u_tilde && dt_tilde >= dt_l_tilde
	// 3. dt_tilde >= dt_u_tilde && dt_tilde  < dt_l_tilde
	// 4. dt_tilde  < dt_u_tilde && dt_tilde  < dt_l_tilde 
	
	BOOL flagRootSolution = false;
	LREAL c_1, c_2, c_3, p_2, p_1, p_0;
	
	// #1 Time difference exceeds both saturation limits
	if((dt_tilde >= dt_u_tilde) && (dt_tilde >= dt_l_tilde)) {
		// Compute the acceleration
		solution->accDec.move = PATH_ACC_DEC_SATURATED;
		solution->decAcc.move = PATH_DEC_ACC_SATURATED;
		
		solution->accDec.a = (c_dtl - c_dtu - (c_dxl - c_dxu)) / (dt_tilde - dx * ((1.0 / v_min) - (1.0 / v_max)));
		solution->decAcc.a = solution->accDec.a;
	}
	
	// #2 Time difference below dt_u_tilde
	else if((dt_tilde < dt_u_tilde) && (dt_tilde >= dt_l_tilde)) {
		// Prepare for 2nd order solution
		solution->accDec.move = PATH_ACC_DEC_PEAK;
		solution->decAcc.move = PATH_DEC_ACC_SATURATED;
		
		c_1 = pow2(v_f) / 2.0;
		c_2 = c_dtl - c_dxl + v_f;
		c_3 = dt_tilde - (dx / v_min);
		p_2 = pow2(c_3);
		p_1 = - 4.0 * dx - 2.0 * c_2 * c_3;
		p_0 = pow2(c_2) - 4.0 * c_1;
		flagRootSolution = true;
	}
	
	// #3 Time difference below dt_l_tilde
	else if ((dt_tilde >= dt_u_tilde) && (dt_tilde < dt_l_tilde)) {
		// Prepare for 2nd order solution
		solution->accDec.move = PATH_ACC_DEC_SATURATED;
		solution->decAcc.move = PATH_DEC_ACC_PEAK;
		
		c_1 = (2.0 * pow2(v_1) + pow2(v_f)) / 2.0;
		c_2 = 2.0 * v_1 + v_f - (c_dtu - c_dxu);
		c_3 = dt_tilde + (dx / v_max);
		p_2 = pow2(c_3);
		p_1 = 4.0 * dx - 2.0 * c_2 * c_3;
		p_0 = pow2(c_2) - 4.0 * c_1;
		flagRootSolution = true;
	}
	
	return PATH_ERROR_NONE;
	
} // End function
