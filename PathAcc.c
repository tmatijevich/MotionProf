/*******************************************************************************
 * File:      PathAcc.c
 * Author:    Tyler Matijevich
 * Created:   2020-04-10
*******************************************************************************/

#include "PathPlanMain.h"

/* Raise value to power of two */
double pow2(double x) {
	return pow(x, 2.0); /* Implicit type promotion of floats and ints */
}

/* Minimum acceleration to move in time over a distance */
long PathAcc(double dt, double dx, double v_0, double v_f, double v_min, double v_max, PathPlanBaseSolutionType *solution) {
	
	/* Declare local variables */
	double dx_bar, dx_u, dx_l, p_2, p_1, p_0;
	PathPlanRootsSolutionType rootsSolution;
	long rootsReturn;
	
	/* Reset the solution */
	memset(solution, 0, sizeof(*solution));
	
	/* Input requirements */
	/* #1 Plausible velocity limits */
	if(v_min < 0.0 || v_max <= v_min)
		return PATH_ERROR_VELOCITYLIMIT;
		
	/* #2 Valid endpoint velocities */
	else if(v_0 < v_min || v_max < v_0 || v_f < v_min || v_max < v_f)
		return PATH_ERROR_VELOCITYLIMIT;
	
	/* #3 Positive inputs */
	else if(dt <= 0.0 || dx <= 0.0)
		return PATH_ERROR_NONPOSITIVE;
	
	/* #4 Plausible move */
	else if(dx <= v_min * dt || v_max * dt <= dx)
		return PATH_ERROR_MOVELIMIT;
	
	/* Intermediate velocity v_12 >= v_0, v_f or v_12 <= v_0, v_f in symmetric Acc + Dec profiles */
	/* Acc/Dec or Dec/Acc? */
	dx_bar = 0.5 * dt * (v_0 + v_f); /* Area of a trapezoid */
	
	if(dx >= dx_bar) { /* Acc/Dec */
		/* Saturated? */
		dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * ((2.0 * v_max - v_0 - v_f) / dt));
		/* NOTE: There is no dx >= dx_bar when v_0 = v_f = v_max that also passes requirement #4. This protects against divide by zero. */
		
		if(dx_u > dx) { /* Peak */
			solution->move = PATH_MOVE_ACCDECPEAK;
			
		} else { /* Saturated */
			solution->move 	= PATH_MOVE_ACCDECSATURATED;
			solution->a		= ((2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / 2.0 - (2.0 * v_max - v_0 - v_f) * v_max) / (dx - dt * v_max); /* Protected by requirement #4 */
			if(solution->a > 0.0) { /* Protect divide by zero */
				solution->v_[1] = v_max;
				solution->v_[2] = v_max;
				solution->t_[1] = (v_max - v_0) / solution->a;
				solution->t_[2] = dt - (v_max - v_f) / solution->a;
			} else { /* Handle floating point inaccuracy, should not occur */
				solution->v_[1] = v_0;
				solution->v_[2] = v_0;
				solution->t_[1] = 0.0;
				solution->t_[2] = 0.0;
			} /* a > 0? */
		} /* dx_u > dx? */
		
	} else { /* Dec/Acc */
		/* Saturated? */
		dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * ((v_0 + v_f - 2.0 * v_min) / dt));
		/* NOTE: There is no dx < NominalDistance when v_0 = v_f = v_min that also passes requirement #4. This protects against divide by zero. */
		
		if(dx_l < dx) { /* Peak (dip) */
			solution->move = PATH_MOVE_DECACCPEAK;
			
		} else { /* Saturated */
			solution->move 	= PATH_MOVE_DECACCSATURATED;
			solution->a 	= ((pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / 2.0 - (v_0 + v_f - 2.0 * v_min) * v_min) / (dx - dt * v_min); /* Protected by requirement #4 */
			if(solution->a > 0.0) {
				solution->v_[1] = v_min;
				solution->v_[2] = v_min;
				solution->t_[1] = (v_0 - v_min) / solution->a;
				solution->t_[2] = dt - (v_f - v_min) / solution->a;
			} else { /* Handle floating point inaccuracy, should not occur */
				solution->v_[1] = v_0;
				solution->v_[2] = v_0;
				solution->t_[1] = 0.0;
				solution->t_[2] = 0.0;
			} /* a > 0.0? */
		} /* dx_l < dx? */
		
	} /* dx >= dx_bar? */
	
	/* Find 2nd order roots for peak solution */
	if(solution->move == PATH_MOVE_ACCDECPEAK || solution->move == PATH_MOVE_DECACCPEAK) {
		p_2 = 2.0 * dt;
		p_1 = -4.0 * dx;
		p_0 = 2.0 * dx * (v_0 + v_f) - dt * (pow2(v_0) + pow2(v_f));
		
		rootsReturn = PathRoots(p_2, p_1, p_0, &rootsSolution);
		if(rootsReturn != PATH_ERROR_NONE)
			return rootsReturn;
			
		if(solution->move == PATH_MOVE_ACCDECPEAK) { 
			solution->v_[1] = fmax(rootsSolution.r_1, rootsSolution.r_2);
			solution->v_[2] = solution->v_[1];
		} else { /* Dec/Acc */
			solution->v_[1] = fmin(rootsSolution.r_1, rootsSolution.r_2);
			solution->v_[2] = solution->v_[1];
		}
		
		/* Acceleration magnitude */
		solution->a = fabs(2.0 * solution->v_[1] - v_0 - v_f) / dt;
		if(solution->a > 0.0) { /* Protect divide by zero */
			solution->t_[1] = fabs(solution->v_[1] - v_0) / solution->a;
			solution->t_[2] = solution->t_[1];
		} else { /* dx = dx_bar and v_0 = v_f */
			solution->t_[1] = 0.0;
			solution->t_[2] = 0.0;
		}
	} /* Peak movement? */
	
	/* Set remaining solution */
	solution->t_[3] = dt;
	solution->dx 	= dx;
	solution->v_[0] = v_0;
	solution->v_[3] = v_f;
	
	return PATH_ERROR_NONE;
	
} /* Function definition */
