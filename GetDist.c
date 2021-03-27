/*******************************************************************************
 * File: GetDist.c
 * Author: Tyler Matijevich
 * Date: 2020-12-19
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the maximum distance when changing velocity with acceleration over time */
DINT GetDist(REAL dt, REAL v0, REAL vf, REAL vmin, REAL vmax, REAL a, struct PathPlanBaseSolutionType* Solution) {

	// Reset the solution
	USINT i;
	for(i = 0; i <= 3; i++) {
		Solution->t[i] = 0.0;
		Solution->v[i] = 0.0;
	}
	Solution->dx 	= 0.0;
	Solution->a 	= 0.0;
	Solution->Move 	= PATH_MOVE_NONE;
	
	/* Input requirements */
	// #1: Plausible velocity limits
	if((vmin < 0.0) || (vmax <= vmin)) {
		return PATH_ERROR_VEL_LIMITS_INVALID;
	}
	
	// #2: Endpoint velocities within limits
	else if((v0 < vmin) || (v0 > vmax) || (vf < vmin) || (vf > vmax)) {
		return PATH_ERROR_VEL_ENDPT_LIMIT;
	}
	
	// #3: Positive time duration and acceleration
	else if((dt <= 0.0) || (a <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4: Valid time duration given acceleration
	else if(dt < fabs(v0 - vf) / a) { // Requires acceleration higher than a
		return PATH_ERROR_ACC_LIMIT; 
	}
	
	// There is no distance advantage to decelerate then accelerate, therefore only ACC_DEC profiles will be considered
	LREAL VmaxTime = (2.0 * vmax - v0 - vf) / a;
	LREAL vpeak, dx12; // Use a higher precision for intermediate variables
	
	// Check if saturated profile
	if(dt < VmaxTime) { // Acc/dec profile with peak
		Solution->Move = PATH_ACC_DEC_PEAK;
		
		// Determine the peak velocity
		vpeak = (dt * a + v0 + vf) / 2.0;
		
		// Set the solution
		Solution->dx 	= (2.0 * pow2(vpeak) - pow2(v0) - pow2(vf)) / (2.0 * a);
		Solution->t[1] 	= (vpeak - v0) / a;
		Solution->t[2] 	= (vpeak - v0) / a;
		Solution->v[1] 	= vpeak;
		Solution->v[2] 	= vpeak;
	} else { // Acc/dec profile saturated at vmax
		Solution->Move = PATH_ACC_DEC_SATURATED;
		
		// Determine the distance at vmax
		dx12 = (dt - (2.0 * vmax - v0 - vf) / a) * vmax;
		
		// Set the solution
		Solution->dx 	= dx12 + (2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / (2.0 * a);
		Solution->t[1] 	= (vmax - v0) / a;
		Solution->t[2] 	= (vmax - v0) / a + dx12 / vmax;
		Solution->v[1] 	= vmax;
		Solution->v[2] 	= vmax;
	}
	
	// Set common solution values
	Solution->t[3] 	= dt;
	Solution->v[0] 	= v0;
	Solution->v[3] 	= vf;
	Solution->a 	= a;
	
	return PATH_ERROR_NONE;

} // End function definition

