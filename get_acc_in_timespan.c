/*********************************************************************************
 * Copyright: B&R Industrial Automation
 * Author:    Tyler Matijevich
 * Created:   May 15, 2020/8:39 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

/* Internal function - return the input squared */
static float sqf(float x) {return pow(x, 2.0);}

plcbit Kin_GetAccInTimespanPlus(float tdiff, float dx, float v1, float vf, float vmin, float vmax, struct KinGetAccInTimespanSoln_typ* soln) {
	/* Determine the minimum acceleration required to achieve movement extremes within the specified timespan */
	/* This function assumes positive kinematic values and infinite jerk */
	/* Date: 2020-04-01 */
	/* Created by: Tyler Matijevich */
	
	soln->a = 0.0; soln->cs = 0.0; /* Fallback/invalid result */
	
	/* Condition #1: Plausible velocity limits */
	if((vmin < 0.0) || (vmax <= vmin)) 
		return 0;
	
	/* Condition #2: Endpoint velocities within limits */
	else if((v1 < vmin) || (v1 > vmax) || (vf < vmin) || (vf > vmax))
		return 0;
	
	/* Condition #3: Positive time and distance */
	else if((tdiff <= 0.0) || (dx <= 0.0))
		return 0;
	
	/* Condition #4: Valid distance given velocity limits */
	else if(tdiff >= (dx / vmin - dx / vmax))
		return 0;
	
	/* Inflection calculations - given dx, what timespans exceeds the inflection points? */
	float aVmaxInflection, aVminInflection, tVmaxInflection, tVminInflection;
	float v2peak, t12max, tVmaxAtVminInflection;
	float v2dip, t12min, tVminAtVmaxInflection;
	
	aVmaxInflection = (2.0 * sqf(vmax) - sqf(vf)) / (2.0 * dx);
	aVminInflection = (2.0 * sqf(v1) + sqf(vf) - 2.0 * sqf(vmin)) / (2.0 * dx);
	
	tVmaxInflection = (2.0 * vmax - vf) / aVmaxInflection;
	tVminInflection = (2.0 * v1 + vf - 2.0 * vmin) / aVminInflection;
	
	/* Time duration for max vel profile @ aVminInflection */
	if(aVminInflection < aVmaxInflection){
		v2peak = sqrt(dx * aVminInflection + sqf(vf) / 2.0);
		tVmaxAtVminInflection = (2.0 * v2peak - vf) / aVminInflection;
	} else {
		t12max = (dx - (2.0 * sqf(vmax) - sqf(vf)) / (2.0 * aVminInflection)) / vmax;
		tVmaxAtVminInflection = (2.0 * vmax - vf) / aVminInflection + t12max; 
	}
	
	/* Time duration for min vel profile @ aVmaxInflection */
	if(aVmaxInflection < aVminInflection) {
		v2dip = sqrt(sqf(v1) + sqf(vf) / 2.0 - dx * aVmaxInflection);
		tVminAtVmaxInflection = (2.0 * v1 + vf - 2.0 * v2dip) / aVmaxInflection;
	} else {
		t12min = (dx - (2.0 * sqf(v1) + sqf(vf) - 2.0 * sqf(vmin)) / (2.0 * aVmaxInflection)) / vmin;
		tVminAtVmaxInflection = (2.0 * v1 + vf - 2.0 * vmin) / aVmaxInflection + t12min;
	}
	
	return 1;
}
