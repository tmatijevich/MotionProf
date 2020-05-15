/*********************************************************************************
 * Copyright: B&R Industrial Automation
 * Author:    Tyler Matijevich
 * Created:   May 15, 2020/8:39 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

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
	
	return 1;
}
