/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich
 * Created:   May 15, 2020/10:01 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

/* Internal function - return the input squared */
static float sqf(float x) {return pow(x, 2.0);}

plcbit Kin_GetTimespan(float dx, float v0, float vf, float vmin, float vmax, float a, struct KinGetTimespanSoln_typ* soln) {
	/* Determine the window of time between the time minimizing and time maximizing velocity profiles */
	/* This function assumes positive kinematic values and infinite jerk */
	/* Date 2020-03-25 */
	/* Created by: Tyler Matijevich */
	
	/* Fallback/invalid result */
	soln->tspan = 0.0; soln->cs = 0;
	soln->tVmax1 = 0.0; soln->tVmax2 = 0.0; soln->tVmax = 0.0; soln->v1max = 0.0;
	soln->tVmin1 = 0.0; soln->tVmin2 = 0.0; soln->tVmin = 0.0; soln->v1min = 0.0;
	
	/* Condition #1: Plausible velocity limits */
	if((vmin < 0.0) || (vmax <= vmin)) 
		return 0;
	
	/* Condition #2: Endpoint velocities within limits */
	else if((v0 < vmin) || (v0 > vmax) || (vf < vmin) || (vf > vmax))
		return 0;
	
	/* Condition #3: Positive distance and acceleration */
	else if((dx <= 0.0) || (a <= 0.0))
		return 0;
	
	float v1;
		
	/* Determine the time minimizing, velocity maximizing profile */
	float dxVmaxInflection = (2.0 * sqf(vmax) - sqf(v0) - sqf(vf)) / (2.0 * a);
	if(dx < dxVmaxInflection) {
		/* Triangle profile */
		v1 = sqrt(dx * a + (sqf(v0) + sqf(vf)) / 2.0); soln->v1max = v1;
		soln->tVmax1 = (v1 - v0) / a;
		soln->tVmax2 = soln->tVmax1;
		soln->tVmax = (2.0 * v1 - v0 - vf) / a;
		soln->cs = 10;
	
	} else {
		/* Trapezoid profile */
		float tVmax12 = (dx - dxVmaxInflection) / vmax; soln->v1max = vmax;
		soln->tVmax1 = (vmax - v0) / a;
		soln->tVmax2 = soln->tVmax1 + tVmax12;
		soln->tVmax = soln->tVmax2 + (vmax - vf) / a;
		soln->cs = 20;
	}
	
	/* Determine the time maximizing, velocity minimizing profile */
	float dxVminInflection = (sqf(v0) + sqf(vf) - 2.0 * sqf(vmin)) / (2.0 * a);
	if(dx < dxVminInflection) {
		/* Triangle profile */
		v1 = sqrt((sqf(v0) + sqf(vf)) / 2.0 - dx * a); soln->v1min = v1;
		soln->tVmin1 = (v0 - v1) / a;
		soln->tVmin2 = soln->tVmin1;
		soln->tVmin = (v0 + vf - 2.0 * v1) / a;
		soln->cs = soln->cs + 1;
	
	} else {
		/* Trapezoid profile */
		float tVmin12 = (dx - dxVminInflection) / vmin; soln->v1min = vmin;
		soln->tVmin1 = (v0 - vmin) / a;
		soln->tVmin2 = soln->tVmin1 + tVmin12;
		soln->tVmin = soln->tVmin2 + (vf - vmin) / a;
		soln->cs = soln->cs + 2;
	}
	
	soln->tspan = soln->tVmin - soln->tVmax;
	
	return 1;
}
