/*******************************************************************************
 * File: GetTimeDiff.c
 * Author: Tyler Matijevich
 * Date: 2020-03-25
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the minimum acceleration to change velocity in time over a distance */
DINT GetTimeDiff(REAL dx, REAL v0, REAL vf, REAL vmin, REAL vmax, REAL a, struct PathPlanTimeDiffSolutionType* Solution) {

	// Reset the solution
	USINT i;
	for(i = 0; i <= 3; i++) {
		Solution->tA[i] = 0.0;
		Solution->tB[i] = 0.0;
		Solution->vA[i] = 0.0;
		Solution->vB[i] = 0.0;
	} 
	Solution->MoveA = PATH_MOVE_NONE;
	Solution->MoveB = PATH_MOVE_NONE;
	Solution->dx 	= 0.0;
	Solution->a 	= 0.0;
	Solution->tdiff = 0.0;
	
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
	else if(dx < fabs(pow2(v0) - pow2(vf)) / (2.0 * a)) { // Requires higher acceleration than given
		return PATH_ERROR_ACC_LIMIT; 
	}

	LREAL DistVmaxSatLimit, DistVminSatLimit, TimeVmax, TimeVmin;
	
	// Determine the time minimizing profile
	DistVmaxSatLimit = (2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / (2.0 * a);
	if(dx < DistVmaxSatLimit) { // Acc/dec profile with peak
		Solution->MoveA = PATH_ACC_DEC_PEAK;
		
		// Determine the peak velocity
		Solution->vA[1] = sqrt(dx * a + (pow2(v0) + pow2(vf)) / 2.0);
		Solution->vA[2] = Solution->vA[1];
		Solution->tA[1] = (Solution->vA[1] - v0) / a;
		Solution->tA[2] = (Solution->vA[1] - v0) / a;
		Solution->tA[3] = (Solution->vA[1] - v0) / a + (Solution->vA[2] - vf) / a;
		
	} else { // Acc/dec profile saturated at vmax
		Solution->MoveA = PATH_ACC_DEC_SATURATED;
		
		// Determine the time at set velocity
		TimeVmax = (dx - DistVmaxSatLimit) / vmax;
		Solution->vA[1] = vmax;
		Solution->vA[2] = vmax;
		Solution->tA[1] = (vmax - v0) / a;
		Solution->tA[2] = (vmax - v0) / a + TimeVmax;
		Solution->tA[3] = (vmax - v0) / a + TimeVmax + (vmax - vf) / a;
	} // Saturated?
	
	// Determine the time maximizing profile
	DistVminSatLimit = (pow2(v0) + pow2(vf) - 2.0 * pow2(vmin)) / (2.0 * a);
	if(dx < DistVminSatLimit) { // Dec/acc profile with dip
		Solution->MoveB = PATH_DEC_ACC_PEAK;
		
		// Determine the dip velocity
		Solution->vB[1] = sqrt((pow2(v0) + pow2(vf)) / 2.0 - dx * a);
		Solution->vB[2] = Solution->vB[1];
		Solution->tB[1] = (v0 - Solution->vB[1]) / a;
		Solution->tB[2] = (v0 - Solution->vB[1]) / a;
		Solution->tB[3] = (v0 - Solution->vB[1]) / a + (vf - Solution->vB[1]) / a;
		
	} else { // Dec/acc profile saturated at vmin
		Solution->MoveB = PATH_DEC_ACC_SATURATED;
		
		// Determine the time at set velocity
		TimeVmin = (dx - DistVminSatLimit) / vmin;
		Solution->vB[1] = vmin;
		Solution->vB[2] = vmin;
		Solution->tB[1] = (v0 - vmin) / a;
		Solution->tB[2] = (v0 - vmin) / a + TimeVmin;
		Solution->tB[3] = (v0 - vmin) / a + TimeVmin + (vf - vmin) / a;
	} // Saturated
	
	// Set the solution
	Solution->vA[0] = v0;
	Solution->vA[3] = vf;
	Solution->vB[0] = v0;
	Solution->vB[3] = vf;
	Solution->tdiff = Solution->tB[3] - Solution->tA[3];
	Solution->dx 	= dx;
	Solution->a 	= a;
	
	return PATH_ERROR_NONE;

} // End function definition
