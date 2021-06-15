/*******************************************************************************
 * File: PathRoots.c
 * Author: Tyler Matijevich
 * Date: 2020-05-13
*******************************************************************************/

// <file> - System header files
// "file" - Program header files
#include <PathPlan.h>
#include "PathPlanMain.h"

/* Return the real roots of a second order polynomial */
DINT PathRoots(LREAL p_2, LREAL p_1, LREAL p_0, struct PathPlanRootsSolutionType* solution) {

	/* Reset solution */
	solution->r_1 = 0.0;
	solution->r_2 = 0.0;
	
	// Compute the discriminant
	LREAL discriminant = pow2(p_1) - 4.0 * p_2 * p_0;
	
	// Check for two roots
	if(p_2 != 0.0) { // Protect against divide by zero
		if(discriminant > 0.0) { // Two real roots
			solution->r_1 = ((-p_1) + sqrt(discriminant)) / (2.0 * p_2);
			solution->r_2 = ((-p_1) - sqrt(discriminant)) / (2.0 * p_2);
		} else if(discriminant < 0.0) { // Imaginary roots
			return PATH_ERROR_2ND_ORDER_NO_SOLN;
		} else { // Root multiple
			solution->r_1 = (-p_1) / (2.0 * p_2);
			solution->r_2 = solution->r_1;
		} // discriminant?
		
	} else { // First order
		if(p_1 != 0.0) { // Single root
			solution->r_1 = (-p_0) / p_1;
			solution->r_2 = solution->r_1; // This function does not distinquish between a multiple root and a single root
		} else { // No roots
			return PATH_ERROR_1ST_ORDER_NO_SOLN;
		} // Single root?
		
	} // Two roots?
	
	return PATH_ERROR_NONE;

} // End function definition
