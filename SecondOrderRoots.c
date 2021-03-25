/*******************************************************************************
 * File: SecondOrderRoots.c
 * Author: Tyler Matijevich
 * Date: 2020-05-13
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Return the real roots of a second order polynomial */
DINT SecondOrderRoots(REAL p2, REAL p1, REAL p0, struct PathPlanRootsSolutionType* Solution) {

	/* Reset solution */
	Solution->r1 = 0.0;
	Solution->r2 = 0.0;
	
	// Compute the discriminant
	REAL Discriminant = pow2f(p1) - 4.0 * p2 * p0;
	
	// Check for two roots
	if(p2 != 0.0) { // Protect against divide by zero
		if(Discriminant > 0.0) { // Two real roots
			Solution->r1 = ((-p1) + sqrt(Discriminant)) / (2.0 * p2);
			Solution->r2 = ((-p1) - sqrt(Discriminant)) / (2.0 * p2);
		} else if(Discriminant < 0.0) { // Imaginary roots
			return PATH_ERROR_2ND_ORDER_NO_SOLN;
		} else { // Root multiple
			Solution->r1 = (-p1) / (2.0 * p2);
			Solution->r2 = Solution->r1;
		} // Discriminant?
		
	} else { // First order
		if(p1 != 0.0) { // Single root
			Solution->r1 = (-p0) / p1;
			Solution->r2 = Solution->r1;
		} else { // No roots
			return PATH_ERROR_1ST_ORDER_NO_SOLN;
		} // Single root?
		
	} // Two roots?
	
	return PATH_ERROR_NONE;

} // End function definition
