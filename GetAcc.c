/*******************************************************************************
 * File: GetAcc.c
 * Author: Tyler Matijevich
 * Date: 2020-04-10
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the minimum acceleration to change velocity in time over a distance */
DINT GetAcc(REAL dt, REAL dx, REAL v0, REAL vf, REAL vmin, REAL vmax, struct PathPlanBaseSolutionType* Solution) {
	
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
	
	// #3: Positive time duration and distance
	else if((dt <= 0.0) || (dx <= 0.0)) {
		return PATH_ERROR_NONPOS_INPUT;
	}
	
	// #4: Valid distance given velocity limits
	else if((dx <= (vmin * dt)) || (dx >= (vmax * dt))) { // Requires infinite acceleration
		return PATH_ERROR_ACC_LIMIT; 
	}
	
	// The intermediate velocity point v12 is either >= v0, vf or <= v0, vf for a symmetric acc/dec profile
	// Determine if 1. ACC 2. DEC or 1. DEC 2. ACC
	REAL NominalDistance = 0.5 * dt * (v0 + vf);
	REAL VmaxDistance, VminDistance; // Distance at the saturation points
	
	if(dx >= NominalDistance) { // 1. ACC 2. DEC
		// Determine if saturated
		VmaxDistance = (2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / (2.0 * ((2.0 * vmax - v0 - vf) / dt));
		// NOTE: There is no dx >= NominalDistance when v0 = vf = vmax that also passes requirement #4. This protects against divide by zero.
		
		if(dx < VmaxDistance) { // Acc/dec profile with peak
			Solution->Move = PATH_ACC_DEC_PEAK;
			
		} else { // Acc/dec profile saturated at vmax
			Solution->Move 	= PATH_ACC_DEC_SATURATED;
			Solution->a		= ((2.0 * pow2(vmax) - pow2(v0) - pow2(vf)) / 2.0 - (2.0 * vmax - v0 - vf) * vmax) / (dx - dt * vmax); // Protected by requirement #4
			if(Solution->a > 0.0) { // Protect against divide by zero
				Solution->v[1] = vmax;
				Solution->v[2] = vmax;
				Solution->t[1] = (vmax - v0) / Solution->a;
				Solution->t[2] = dt - (vmax - vf) / Solution->a;
			} else { // Should not happen, a != 0.0 is required to complete a saturated move
				Solution->v[1] = v0;
				Solution->v[2] = v0;
				Solution->t[1] = 0.0;
				Solution->t[2] = 0.0;
			} // a > 0.0?
		} // VmaxDistance?
		
	} else { // 1. DEC 2. ACC
		// Determine if saturated
		VminDistance = (pow2(v0) + pow2(vf) - 2.0 * pow2(vmin)) / (2.0 * ((v0 + vf - 2.0 * vmin) / dt));
		// NOTE: There is no dx < NominalDistance when v0 = vf = vmin that also passes requirement #4. This protects against divide by zero.
		
		if(dx > VminDistance) { // Dec/acc profile with dip
			Solution->Move = PATH_DEC_ACC_PEAK;
			
		} else { // Dec/acc profile saturate at vmin
			Solution->Move 	= PATH_DEC_ACC_SATURATED;
			Solution->a 	= ((pow2(v0) + pow2(vf) - 2.0 * pow2(vmin)) / 2.0 - (v0 + vf - 2.0 * vmin) * vmin) / (dx - dt * vmin); // Protected by requirement #4
			if(Solution->a > 0.0) {
				Solution->v[1] = vmin;
				Solution->v[2] = vmin;
				Solution->t[1] = (v0 - vmin) / Solution->a;
				Solution->t[2] = dt - (vf - vmin) / Solution->a;
			} else { // Should not happen, a != 0.0 is required to complete a saturated move
				Solution->v[1] = v0;
				Solution->v[2] = v0;
				Solution->t[1] = 0.0;
				Solution->t[2] = 0.0;
			} // a > 0.0?
		} // VminDistance?
		
	} // NominalDistance?
	
	/* Find the roots of the second order peak solution */
	REAL p2, p1, p0;
	struct PathPlanRootsSolutionType RootsSolution;
	DINT RootsReturn;
	if((Solution->Move == PATH_ACC_DEC_PEAK) || (Solution->Move == PATH_DEC_ACC_PEAK)) {
		p2 = 2.0 * dt;
		p1 = -4.0 * dx;
		p0 = 2.0 * dx * (v0 + vf) - dt * (pow2(v0) + pow2(vf));
		RootsReturn = SecondOrderRoots(p2, p1, p0, &RootsSolution);
		if(RootsReturn == PATH_ERROR_NONE) { // Roots are valid
			if(Solution->Move == PATH_ACC_DEC_PEAK) { // Vmax
				Solution->v[1] = fmax(RootsSolution.r1, RootsSolution.r2);
				Solution->v[2] = Solution->v[1];
			} else { // Vmin
				Solution->v[1] = fmin(RootsSolution.r1, RootsSolution.r2);
				Solution->v[2] = Solution->v[1];
			}
			
			// Compute acceleration and protect against divide by zero
			Solution->a = fabs(2.0 * Solution->v[1] - v0 - vf) / dt;
			if(Solution->a > 0.0) {
				Solution->t[1] = fabs(Solution->v[1] - v0) / Solution->a;
				Solution->t[2] = Solution->t[1];
			} else { // A flat line, dx = NominalDistance and v0 = vf
				Solution->t[1] = 0.0;
				Solution->t[2] = 0.0;
			}
			
		} else { // Error occurred, invalid roots
			return RootsReturn;
		}
	}
	
	return PATH_ERROR_NONE;
	
} // End function definition
