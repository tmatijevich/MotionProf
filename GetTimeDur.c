/*******************************************************************************
 * File: GetTime.c
 * Author: Tyler Matijevich
 * Date: 2020-12-23
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the minimum time duration to change velocity with acceleration over a distance */
DINT GetTimeDur(REAL dx, REAL v0, REAL vf, REAL vmin, REAL vmax, REAL a, struct PathPlanBaseSolutionType* Solution) {

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
	
	// #3: Positive distance and acceleration
	else if((dx <= 0.0) || (a <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4: Valid distance given acceleration
	else if(dx < fabs(pow2(v0) - pow2(vf)) / (2.0 * a)) { // Requires acceleration higher than a
		return PATH_ERROR_ACC_LIMIT;
	}
	
	// There is no time advantage to decelerating below the final velocity, therefore only ACC_DEC profile will be considered
	LREAL VmaxDistance = (2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / (2.0 * a);
	LREAL vpeak, t12; // Use higher precision for intermediate variables
	
	// Check if saturated profile
	if(dx < VmaxDistance) { // Acc/dec profile with peak
		Solution->Move = PATH_ACC_DEC_PEAK;
		
		// Determine the peak velocity
		vpeak = sqrt(dx * a + (pow2(v0) + pow2(vf)) / 2.0);
		
		// Set the solution
		Solution->t[1] = (vpeak - v0) / a;
		Solution->t[2] = (vpeak - v0) / a;
		Solution->t[3] = (vpeak - v0) / a + (vpeak - vf) / a;
		Solution->v[1] = vpeak;
		Solution->v[2] = vpeak;
		
	} else { // Acc/dec profile saturated at vmax
		Solution->Move = PATH_ACC_DEC_SATURATED;
		
		// Determine the time at vmax
		t12 = (dx - VmaxDistance) / vmax;
		
		// Set the solution
		Solution->t[1] = (vmax - v0) / a;
		Solution->t[2] = (vmax - v0) / a + t12;
		Solution->t[3] = (vmax - v0) / a + t12 + (vmax - vf) / a;
		Solution->v[1] = vmax;
		Solution->v[2] = vmax;
	}
	
	// Set common solution values
	Solution->dx 	= dx;
	Solution->v[0] 	= v0;
	Solution->v[3] 	= vf;
	Solution->a 	= a;
	
	return PATH_ERROR_NONE;

} // End function definition
