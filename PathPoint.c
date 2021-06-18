/*******************************************************************************
 * File:      PathPoint.c
 * Author:    Tyler Matijevich
 * Created:   2020-04-01
*******************************************************************************/

#include <PathPlan.h>
#include "PathPlanMain.h"

/* Linearly-interpolated position, velocity, and acceleration along a velocity-point profile */
DINT PathPoint(LREAL x_0, LREAL t_[PATH_POINTS_MAX_INDEX + 1], LREAL v_[PATH_POINTS_MAX_INDEX + 1], USINT n, LREAL t, struct PathPlanPointSolutionType* solution) {

	/* Reset the solution */
	solution->x = 0.0;
	solution->v = 0.0;
	solution->a = 0.0;
	
	/* Input Requirements */
	// #1 Number of points
	if((n < 2) || (n > PATH_POINTS_MAX_INDEX + 1)) { 
		// Invalid number of points
		return PATH_ERROR_POINTLIMIT;
	}
	
	// #2 Sequential times
	USINT i; // Loop index variable
	for(i = 1; i < n; i++) { // 1..4 if 5 points
		// Check that the time values are non-decreasing
		if(t_[i] < t_[i - 1]) {
			return PATH_ERROR_TIMESEQUENCE;
		}
	}
	
	// #3 Request time
	if((t < t_[0]) || (t > t_[n - 1])) {
		// The request time must be within the specified arrays
		return PATH_ERROR_TIMELIMIT;
	}
	
	// Declare position and acceleration arrays
	LREAL x_[PATH_POINTS_MAX_INDEX + 1]; // Initial position of each segment
	LREAL a_[PATH_POINTS_MAX_INDEX + 1]; // Set acceleration along each segment (defined from 0..n-2)
	
	/* Compute starting positions and acceleration for each segment */
	x_[0] = x_0;
	for(i = 1; i < n; i++) {
		// Use the previous position plus the area under the curve
		x_[i] = x_[i - 1] + 0.5 * (v_[i] + v_[i - 1]) * (t_[i] - t_[i - 1]);
		// Compute acceleration, protect against divide by zero
		if(t_[i] == t_[i - 1]) {
			a_[i - 1] = 0.0;
		} else {
			a_[i - 1] = (v_[i] - v_[i - 1]) / (t_[i] - t_[i - 1]);
		}
	}
	
	/* Find the requested segment */
	USINT seg;
	if(t == t_[n - 1]) {
		seg = n - 1; // Set as the final segment
	} else {
		for(i = 1; i < n; i++) {
			if(t < t_[i]) { // i looks ahead at the endpoint of the segment
				seg = i - 1;
				break; // Break when found
			}
		}
	}
	
	/* Set solution */
	solution->a = a_[seg];
	solution->v = v_[seg] + a_[seg] * (t - t_[seg]);
	solution->x = x_[seg] + v_[seg] * (t - t_[seg]) + 0.5 * a_[seg] * pow2(t - t_[seg]);
	
	return PATH_ERROR_NONE;

} // End PathPoint call
