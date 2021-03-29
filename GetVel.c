/*******************************************************************************
 * File: GetVel.c
 * Author: Tyler Matijevich
 * Date: 2020-12-30
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the minimum acceleration to change velocity in time over a distance */
DINT GetVel(REAL dt, REAL dx, REAL v0, REAL vf, REAL vmin, REAL vmax, REAL a, struct PathPlanBaseSolutionType* Solution) {
	
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
	
	// #3: Positive time duration, distance, and acceleration
	else if((dt <= 0.0) || (dx <= 0.0) || (a <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4: Valid time duration and distance given acceleration
	else if((dt < fabs(v0 - vf) / a) || (dx < fabs(pow2(v0) - pow2(vf)) / (2.0 * a))) { // Requires higher acceleration than given
		return PATH_ERROR_ACC_LIMIT; 
	}
	
	/* Check if the distance can be fulfilled given the time duration, velocity limits, and acceleration */
	LREAL vpeak, vdip, MaxDistance, MinDistance; // Use higher precision for intermediate variables
	
	// First check the max distance
	vpeak = (dt * a + v0 + vf) / 2.0; // A test variable
	if(vpeak <= vmax) { // Acc/dec profile with peak
		MaxDistance = (2.0 * pow2(vpeak) - pow2(v0) - pow2(vf)) / (2.0 * a);
	} else { // Acc/dec profile saturated at vmax
		MaxDistance = (2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / (2.0 * a) + vmax * (dt - ((2.0 * vmax - v0 - vf) / a));
	} 
	if(dx > MaxDistance) { // Unachievable distance
		return PATH_ERROR_ACC_LIMIT; 
	}
	
	// Next check the min distance
	vdip = (dt * a - v0 - vf) / 2.0; // A test variable
	if(vdip >= vmin) { // Dec/acc profile with dip
		MinDistance = (pow2(v0) + pow2(vf) - 2.0 * pow2(vdip)) / (2.0 * a);
	} else { // Dec/acc profile saturated at vmin
		MinDistance = (pow2(v0) + pow2(vf) - 2.0 * pow2(vmin)) / (2.0 * a) + vmin * (dt - ((v0 + vf - 2.0 * vmin) / a));
	}
	if(dx < MinDistance) { // Unachievable distance
		return PATH_ERROR_ACC_LIMIT;
	}
	
	/* Determine the nominal time duration and distance deciding the directions of the acceleration phases */
	LREAL NominalTime = fabs(v0 - vf) / a;
	LREAL NominalDistance = fabs(pow2(v0) - pow2(vf)) / (2.0 * a);
	LREAL a1Distance = v0 * (dt - NominalTime) + NominalDistance; // Distance if no time in phase 1
	LREAL a2Distance = NominalDistance + vf * (dt - NominalTime); // Distance if no time in phase 2
	
	/* Determine the sign of both acceleration phases */
	SINT a1Sign, a2Sign;
	
	if(dx > a1Distance) {
		a1Sign = 1;
	} else if(dx < a1Distance) {
		a1Sign = -1;
	} else { 
		a1Sign = 0;
	}
	
	if(dx > a2Distance) {
		a1Sign = -1;
	} else if(dx < a2Distance) {
		a1Sign = 1;
	} else { 
		a1Sign = 0;
	}
	
	/* Solve for the three cases */
	LREAL p2, p1, p0;
	
	if((a1Sign == 1) && (a2Sign == -1)) { // ACC_DEC
		// Assume the move is a saturated approaching a peak profile only when dx = MaxDistance and vpeak <= vmax
		Solution->Move = PATH_ACC_DEC_SATURATED;
		
		p2 = -1.0 / a;
		p1 = dt - NominalTime + (2.0 * fmax(v0, vf)) / a;
		p0 = NominalDistance - dx - pow2(fmax(v0, vf)) / a;
		
	} else if(a1Sign == a2Sign) { // ACC_ACC or DEC_DEC
		if(a1Sign == 1) {
			Solution->Move = PATH_ACC_ACC;
		} else { 
			Solution->Move = PATH_DEC_DEC;
		}
		
		Solution->v[1] = (dx - NominalDistance) / (dt - NominalTime);
		
	} else { // DEC_ACC
		Solution->Move = PATH_DEC_ACC_SATURATED;
		
		p2 = 1.0 / a;
		p1 = dt - NominalTime - (2.0 * fmin(v0, vf)) / a;
		p0 = NominalDistance - dx + pow2(fmin(v0, vf)) / a;
	}
	
	return PATH_ERROR_NONE;
	
} // End function definition