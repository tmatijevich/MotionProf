/*******************************************************************************
 * File:      PathAcc.c
 * Author:    Tyler Matijevich
 * Created:   2020-04-10
*******************************************************************************/

#include <PathPlan.h>
#include "PathPlanMain.h"

/* Minimum acceleration to move in a time over a distance */
DINT PathAcc(LREAL dt, LREAL dx, LREAL v_0, LREAL v_f, LREAL v_min, LREAL v_max, struct PathPlanBaseSolutionType* solution) {
	
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
	
	// #2 Valid endpoints velocities
	else if((v_0 < v_min) || (v_0 > v_max) || (v_f < v_min) || (v_f > v_max)) {
		return PATH_ERROR_VELOCITYENDPT;
	}
	
	// #3 Positive inputs
	else if((dt <= 0.0) || (dx <= 0.0)) {
		return PATH_ERROR_NONPOSITIVE;
	}
	
	// #4 Plausible move
	else if((dx <= (v_min * dt)) || (dx >= (v_max * dt))) { // Requires infinite acceleration
		return PATH_ERROR_MOVELIMIT; 
	}
	
	// The intermediate velocity point v_12 is either >= v_0, v_f or <= v_0, v_f for a symmetric AccDec profile
	// Determine if AccDec or DecAcc
	LREAL dx_bar = 0.5 * dt * (v_0 + v_f);
	LREAL dx_u, dx_l; // Distance at the saturation points
	
	if(dx >= dx_bar) { // AccDec
		// Determine if saturated
		dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * ((2.0 * v_max - v_0 - v_f) / dt));
		// NOTE: There is no dx >= dx_bar when v_0 = v_f = v_max that also passes requirement #4. This protects against divide by zero.
		
		if(dx < dx_u) { // AccDec profile with peak
			solution->move = PATH_MOVE_ACCDECPEAK;
			
		} else { // AccDec profile saturated at v_max
			solution->move 	= PATH_MOVE_ACCDECSATURATED;
			solution->a		= ((2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / 2.0 - (2.0 * v_max - v_0 - v_f) * v_max) / (dx - dt * v_max); // Protected by requirement #4
			if(solution->a > 0.0) { // Protect against divide by zero
				solution->v_[1] = v_max;
				solution->v_[2] = v_max;
				solution->t_[1] = (v_max - v_0) / solution->a;
				solution->t_[2] = dt - (v_max - v_f) / solution->a;
			} else { // Should not happen, a != 0.0 is required to complete a saturated move
				solution->v_[1] = v_0;
				solution->v_[2] = v_0;
				solution->t_[1] = 0.0;
				solution->t_[2] = 0.0;
			} // a > 0.0?
		} // dx_u?
		
	} else { // DecAcc
		// Determine if saturated
		dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * ((v_0 + v_f - 2.0 * v_min) / dt));
		// NOTE: There is no dx < NominalDistance when v_0 = v_f = v_min that also passes requirement #4. This protects against divide by zero.
		
		if(dx > dx_l) { // DecAcc profile with dip
			solution->move = PATH_MOVE_DECACCPEAK;
			
		} else { // DecAcc profile saturate at v_min
			solution->move 	= PATH_MOVE_DECACCSATURATED;
			solution->a 	= ((pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / 2.0 - (v_0 + v_f - 2.0 * v_min) * v_min) / (dx - dt * v_min); // Protected by requirement #4
			if(solution->a > 0.0) {
				solution->v_[1] = v_min;
				solution->v_[2] = v_min;
				solution->t_[1] = (v_0 - v_min) / solution->a;
				solution->t_[2] = dt - (v_f - v_min) / solution->a;
			} else { // Should not happen, a != 0.0 is required to complete a saturated move
				solution->v_[1] = v_0;
				solution->v_[2] = v_0;
				solution->t_[1] = 0.0;
				solution->t_[2] = 0.0;
			} // a > 0.0?
		} // dx_l?
		
	} // dx_bar?
	
	/* Find the roots of the second order peak solution */
	LREAL p_2, p_1, p_0;
	struct PathPlanRootsSolutionType rootsSolution;
	DINT rootsReturn;
	if((solution->move == PATH_MOVE_ACCDECPEAK) || (solution->move == PATH_MOVE_DECACCPEAK)) {
		p_2 = 2.0 * dt;
		p_1 = -4.0 * dx;
		p_0 = 2.0 * dx * (v_0 + v_f) - dt * (pow2(v_0) + pow2(v_f));
		rootsReturn = PathRoots(p_2, p_1, p_0, &rootsSolution);
		if(rootsReturn == PATH_ERROR_NONE) { // Roots are valid
			if(solution->move == PATH_MOVE_ACCDECPEAK) { 
				solution->v_[1] = fmax(rootsSolution.r_1, rootsSolution.r_2);
				solution->v_[2] = solution->v_[1];
			} else { // DecAcc
				solution->v_[1] = fmin(rootsSolution.r_1, rootsSolution.r_2);
				solution->v_[2] = solution->v_[1];
			}
			
			// Compute acceleration and protect against divide by zero
			solution->a = fabs(2.0 * solution->v_[1] - v_0 - v_f) / dt;
			if(solution->a > 0.0) {
				solution->t_[1] = fabs(solution->v_[1] - v_0) / solution->a;
				solution->t_[2] = solution->t_[1];
			} else { // A flat line, dx = dx_bar and v_0 = v_f
				solution->t_[1] = 0.0;
				solution->t_[2] = 0.0;
			}
			
		} else { // Error occurred, invalid roots
			return rootsReturn;
		}
	}
	
	/* Set common solution values */
	solution->t_[3] = dt;
	solution->dx 	= dx;
	solution->v_[0] = v_0;
	solution->v_[3] = v_f;
	
	return PATH_ERROR_NONE;
	
} // End function definition
