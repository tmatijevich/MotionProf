/*********************************************************************************
 * Copyright: B&R Industrial Automation 
 * Author:    Tyler Matijevich
 * Created:   May 14, 2020/5:46 PM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>
 
unsigned char Kin_GetAcc(float dt, float dx, float v0, float vf, float vmin, float vmax, struct KinGetAccSoln_typ* soln) {
	/* Determine the minimum acceleration to change velocity in time over a distance */
	/* This function assumes positive kinematic values and infinite jerk */
	/* Date: 2020-04-10 */
	/* Created by: Tyler Matijevich */
	
	soln->a = 0.0; soln->v12 = 0.0; soln->t1 = 0.0; soln->t2 = 0.0; /* Fallback/invalid result */
	
	/* Condition #1: Plausible velocity limits (Status Code 11) */
	if((vmin < 0.0) || (vmax <= vmin)) 
		return 11;
	
	/* Condition #2: Endpoint velocities within limits (Status Code 12) */
	else if((v0 < vmin) || (v0 > vmax) || (vf < vmin) || (vf > vmax))
		return 12;
	
	/* Condition #3: Positive time and distance (Status Code 13) */
	else if((dt <= 0.0) || (dx <= 0.0))
		return 13;
	
	/* Condition #4: Valid distance given velocity limits (Status Code 14) */
	else if((dx <= (dt * vmin)) || (dx >= (dt * vmax)))
		return 14;

	/* Inflection distance */
	float dxInflection = 0.5 * dt * (v0 + vf);
	
	if(dx >= dxInflection) {
		float dxVmaxInflection = (2.0 * pow(vmax, 2.0) - pow(v0, 2.0) - pow(vf, 2.0)) / (2.0 * ((2.0 * vmax - v0 - vf) / dt));
		
		if(dx < dxVmaxInflection) {
			/* Triangle profile */
			soln->cs = 10;
		} else {
			/* Trapezoid profile */
			soln->cs = 20;
			soln->a = ((2.0 * pow(vmax, 2.0) - pow(v0, 2.0) - pow(vf, 2.0)) / 2.0 - (2.0 * vmax - v0 - vf) * vmax) / (dx - dt * vmax);
			soln->v12 = vmax;
			soln->t1 = (vmax - v0) / soln->a;
			soln->t2 = dt - (vmax - vf) / soln->a;
		}
	
	} else {
		float dxVminInflection = (pow(v0, 2.0) + pow(vf, 2.0) - 2.0 * pow(vmin, 2.0)) / (2.0 * ((v0 + vf - 2.0 * vmin) / dt));
		
		if(dx > dxVminInflection) {
			/* Triangle profile */
			soln->cs = 1;
		} else {
			/* Trapezoid profile */
			soln->cs = 2;
			soln->a = ((pow(v0, 2.0) + pow(vf, 2.0) - 2.0 * pow(vmin, 2.0)) / 2.0 - (v0 + vf - 2.0 * vmin) * vmin) / (dx - dt * vmin);
			soln->v12 = vmin;
			soln->t1 = (v0 - vmin) / soln->a;
			soln->t2 = dt - (vf - vmin) / soln->a;
		}
	}
	
	/* Use the quadratic formula to determine the intermediate velocity of the triangle profile */
	if((soln->cs == 10) || (soln->cs == 1)) {
		struct Math2ndOrderRootsSoln_typ solnRoots;
		if(Math_2ndOrderRoots(2.0 * dt, -4.0 * dx, 2.0 * dx * (v0 + vf) - dt * (pow(v0, 2.0) + pow(vf, 2.0)), &solnRoots)) {
			if(soln->cs == 10)
				soln->v12 = fmaxf(solnRoots.r1, solnRoots.r2);
			else
				soln->v12 = fminf(solnRoots.r1, solnRoots.r2);
			
			soln->a = fabsf(2.0 * soln->v12 - v0 - vf) / dt;
			soln->t1 = soln->t2 = fabsf(soln->v12 - v0) / soln->a;
			
		} else 
			return 21; // (Status Code 21: Invalid 2nd order roots)
	}
		
	return 0;
}
