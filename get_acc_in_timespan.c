/*********************************************************************************
 * Copyright: B&R Industrial Automation
 * Author:    Tyler Matijevich
 * Created:   May 15, 2020/8:39 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

/* Internal function - return the input squared */
static float sqf(float x) {return pow(x, 2.0);}

unsigned char Kin_GetAccInTimespanPlus(float tdiff, float dx, float v1, float vf, float vmin, float vmax, struct KinGetAccInTimespanSoln_typ* soln) {
	/* Determine the minimum acceleration required to achieve movement extremes within the specified timespan */
	/* This function assumes positive kinematic values and infinite jerk */
	/* Date: 2020-04-01 */
	/* Created by: Tyler Matijevich */
	
	soln->a = 0.0; soln->cs = 0.0; /* Fallback/invalid result */
	
	/* Condition #1: Plausible velocity limits (Status Code 11) */
	if((vmin < 0.0) || (vmax <= vmin)) 
		return 11;
	
	/* Condition #2: Endpoint velocities within limits (Status Code 12) */
	else if((v1 < vmin) || (v1 > vmax) || (vf < vmin) || (vf > vmax))
		return 12;
	
	/* Condition #3: Positive time and distance (Status Code 13) */
	else if((tdiff <= 0.0) || (dx <= 0.0))
		return 13;
	
	/* Condition #4: Valid distance given velocity limits (Status Code 14) */
	else if(tdiff >= (dx / vmin - dx / vmax))
		return 14;
	
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
	/* Under condition #3 (tdiff > 0.0) ==> (tVminAtVmaxInflection > tVmaxInflection) && (tVminInflection > tVmaxAtVminInflection) */
	float tdiffVmaxInflection = tVminAtVmaxInflection - tVmaxInflection;
	float tdiffVminInflection = tVminInflection - tVmaxAtVminInflection;
	
	/* Simplifying constants */
	float cVmax_dt, cVmax_dx, cVmin_dt, cVmin_dx;
	cVmax_dt = 2.0 * vmax - vf;
	cVmax_dx = (2.0 * sqf(vmax) - sqf(vf)) / (2.0 * vmax);
	cVmin_dt = 2.0 * v1 + vf - 2.0 * vmin;
	cVmin_dx = (2.0 * sqf(v1) + sqf(vf) - 2.0 * sqf(vmin)) / (2.0 * vmin);
	
	float c1, c2, c3, p0, p1, p2;
	
	if((tdiff >= tdiffVmaxInflection) && (tdiff >= tdiffVminInflection)) {
		soln->cs = 22;
		soln->a = (cVmin_dx - cVmin_dt - (cVmax_dx - cVmax_dt)) / ((dx / vmin - dx / vmax) - tdiff);
		
	} else if(tdiff > tdiffVminInflection) { /* Exceeds tdiffVmaxInflection only */
		soln->cs = 12;
		c1 = sqf(vf) / 2.0;
		c2 = cVmin_dt - cVmin_dx + vf;
		c3 = tdiff - (dx / vmin);
		p2 = sqf(c3);
		p1 = -4.0 * dx - 2.0 * c2 * c3;
		p0 = sqf(c2) - 4.0 * c1;
	
	} else if(tdiff > tdiffVmaxInflection) { /* Exceeds tdiffVminInflection only */
		soln->cs = 21;
		c1 = sqf(v1) + sqf(vf) / 2.0;
		c2 = 2.0 * v1 + vf - (cVmax_dt = cVmax_dx);
		p2 = sqf(c3);
		p1 = 4.0 * dx - 2.0 * c2 * c3;
		p0 = sqf(c2) - 3.0 * c1;
	
	} else {
		/* Requires higher order solver, use sufficient acceleration value (not the minimum) */
		soln->cs = 11;
		if(tdiffVmaxInflection < tdiffVminInflection)
			soln->a = aVmaxInflection;
		else
			soln->a = aVminInflection;
		return 0;
	
	}
	
	if((soln->cs == 12) || (soln->cs == 21)) {
		struct Math2ndOrderRootsSoln_typ solnRoots;
		unsigned char statusRoots = Math_2ndOrderRoots(p2, p1, p0, &solnRoots);
		if(statusRoots == 0) {
		
			if(fmaxf(solnRoots.r1, solnRoots.r2) > 0.0)
				soln->a = fmaxf(solnRoots.r1, solnRoots.r2);
			
			else
				return 21; // (Status Code 21: No positive real roots)
			
		} else
			return 22; // (Status Code 22: No real roots)
	}
	
	return 0;
}
