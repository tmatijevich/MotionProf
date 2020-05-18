/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich
 * Created:   May 15, 2020/11:33 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

plcbit Kin_GetVelProfPoint(float x0, float timePoints[KIN_MAX_PROF_POINTS + 1], float velPoints[KIN_MAX_PROF_POINTS + 1], unsigned char n, float t, struct KinGetVelProfPointSoln_typ* soln) {
	/* Determine the point on a piecewise linear velocity profile */
	/* Date: 2020-04-01 */
	/* Created by: Tyler Matijevich */
	
	soln->x = 0.0; soln->v = 0.0; soln->a = 0.0; soln->segment = 0; /* Fallback/invalid solution */
	
	/* Condition #1: Number of points */
	if((n < 2) || (n > KIN_MAX_PROF_POINTS))
		return 0;
	
	/* Condition #2: Sequential times */
	unsigned char i;
	for(i = 2; i <= n; i++) {
		if(timePoints[i] < timePoints[i - 1]) 
			return 0;
	}
	
	/* Condition #3: Request time */
	if((t < timePoints[1]) || (t > timePoints[n]))
		return 0;
	
	float posPoints[KIN_MAX_PROF_POINTS + 1];
	float accPoints[KIN_MAX_PROF_POINTS + 1];
	
	/* Compute starting position and acceleration of each section */
	posPoints[1] = x0;
	for(i = 2; i <= n; i++) {
		posPoints[i] = posPoints[i - 1] + 0.5 * (velPoints[i] + velPoints[i - 1]) * (timePoints[i] - timePoints[i - 1]);
		if(timePoints[i] == timePoints[i - 1])
			accPoints[i - 1] = 0.0;
		else
			accPoints[i - 1] = (velPoints[i] - velPoints[i - 1]) / (timePoints[i] - timePoints[i - 1]);
		/*
		 * posPoints[n] is never used since seg is within [1,n-1]
		 * accPoints[n] is undefined, because it is never used
		 */
	}
	
	/* Find the requested segment */
	unsigned char seg;
	if(t == timePoints[n])
		seg = n - 1;
	else {
		for(i = 2; i <= n; i++) {
			if(t < timePoints[i]) {
				seg = i - 1; 
				break;
			}
		}
	}
	
	/* Compute the solution */
	soln->segment = seg;
	soln->a = accPoints[seg];
	soln->v = velPoints[seg] + accPoints[seg] * (t - timePoints[seg]);
	soln->x = posPoints[seg] + velPoints[seg] * (t - timePoints[seg]) + 0.5 * accPoints[seg] * pow(t - timePoints[seg], 2.0);
	
	return 1;
}
