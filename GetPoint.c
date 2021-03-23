/*******************************************************************************
 * File: GetPoint.c
 * Author: Tyler Matijevich
 * Date: 2020-04-01
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Determine the point on a piecewise linear velocity profile */
DINT GetPoint(REAL InitialPosition, REAL TimePoints[PATH_MAX_INDEX_POINTS + 1], REAL VelocityPoints[PATH_MAX_INDEX_POINTS + 1], USINT NumberOfPoints, REAL Time, struct PathPlanPointSolutionType* Solution) {

	// Reset the solution
	Solution->x = 0.0;
	Solution->v = 0.0;
	Solution->a = 0.0;
	
	/* Input Requirements */
	// #1: Number of points
	if((NumberOfPoints < 2) || (NumberOfPoints > PATH_MAX_INDEX_POINTS + 1)) {
		// Invalid number of points
		return PATH_ERROR_POINT_LIMIT;
	}
	
	// #2: Sequential times
	USINT i; // Loop index variable
	for(i = 1; i < NumberOfPoints; i++) { // 1..4 if 5 points
		// Check that the time values are non-decreasing
		if(TimePoints[i] < TimePoints[i - 1]) {
			return PATH_ERROR_SEQ_POINTS;
		}
	}
	
	// #3: Request time
	if((Time < TimePoints[0]) || (Time > TimePoints[NumberOfPoints - 1])) {
		// The request time must be within the specified arrays
		return PATH_ERROR_TIME_POINT;
	}
	
	// Declare position and acceleration arrays
	REAL t[PATH_MAX_INDEX_POINTS + 1];
	REAL x[PATH_MAX_INDEX_POINTS + 1]; // Initial position at the start of each segment
	REAL v[PATH_MAX_INDEX_POINTS + 1]; // Copy the input array
	REAL a[PATH_MAX_INDEX_POINTS + 1]; // Set acceleration along each segment (only defined from 0..NumberOfPoints-2)
	
	/* Compute starting positions and acceleration for each segment */
	t[0] = TimePoints[0];
	x[0] = InitialPosition;
	v[0] = VelocityPoints[0];
	for(i = 1; i < NumberOfPoints; i++) {
		// Copy velocity and time
		t[i] = TimePoints[i];
		v[i] = VelocityPoints[i];
		// Use the previous position plus the area under the curve
		x[i] = x[i - 1] + 0.5 * (v[i] + v[i - 1]) * (t[i] - t[i - 1]);
		// Compute acceleration, protect against divide by zero
		if(t[i] == t[i - 1]) {
			a[i - 1] = 0.0;
		} else {
			a[i - 1] = (v[i] - v[i - 1]) / (t[i] - t[i - 1]);
		}
	}
	
	/* Find the requested segment */
	USINT seg;
	if(Time == t[NumberOfPoints - 1]) {
		seg = NumberOfPoints - 1; // Set as the final segment
	} else {
		for(i = 1; i < NumberOfPoints; i++) {
			if(Time < t[i]) { // i looks ahead at the endpoint of the segment
				seg = i - 1;
				break; // Break when found
			}
		}
	}
	
	/* Set solution */
	Solution->a = a[seg];
	Solution->v = v[seg] + a[seg] * (Time - t[seg]);
	Solution->x = x[seg] + v[seg] * (Time - t[seg]) + 0.5 * a[seg] * pow2f(Time - t[seg]);
	
	return PATH_ERROR_NONE;

} // End GetPoint call
