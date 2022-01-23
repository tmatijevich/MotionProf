/*******************************************************************************
 * File:      PathRoots.c
 * Author:    Tyler Matijevich
 * Created:   2020-05-13
*******************************************************************************/

#include "PathPlanMain.h"

/* Real roots of a second order polynomial (quadratic equation) */
long PathRoots(double p_2, double p_1, double p_0, PathPlanRootsSolutionType* solution) {
	
	/* Declare local variables */
	double discriminant = pow2(p_1) - 4.0 * p_2 * p_0;
	
	/* Reset solution */
	memset(solution, 0, sizeof(*solution));
	
	/* Check for roots */
	if(p_2 != 0.0) { /* Protect divide by zero */
		if(discriminant > 0.0) { /* Two real roots */
			solution->r_1 = ((-p_1) + sqrt(discriminant)) / (2.0 * p_2);
			solution->r_2 = ((-p_1) - sqrt(discriminant)) / (2.0 * p_2);
		} 
		else if(discriminant < 0.0) /* Two imaginary roots */
			return PATH_ERROR_NOSOLUTION2ND;
		
		else { /* Root multiple */
			solution->r_1 = (-p_1) / (2.0 * p_2);
			solution->r_2 = solution->r_1;
		}
	} 
	else { /* First order */
		if(p_1 != 0.0) { /* Single root */
			solution->r_1 = (-p_0) / p_1;
			solution->r_2 = solution->r_1; /* This function does not distinquish between a multi-root and single root */
		} else { /* No roots */
			return PATH_ERROR_NOSOLUTION1ST;
		} /* One root? */
	} /* Two roots? */
	
	return PATH_ERROR_NONE;

} /* Function definition */
