/*******************************************************************************
 * File:      PathTime.c
 * Author:    Tyler Matijevich
 * Created:   2020-12-23
*******************************************************************************/

#include "PathPlanMain.h"

/* Minimum time to move with acceleration over a distance */
long PathTime(double dx, double v_0, double v_f, double v_min, double v_max, double a, PathPlanBaseSolutionType *solution) {
	
	/* Declare local variables */
	double dx_u, v_p, t_12;
	
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
	else if(dx <= 0.0 || a <= 0.0)
		return PATH_ERROR_NONPOSITIVE;
	
	/* #4 Plausible move */
	else if(dx < fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a))
		return PATH_ERROR_MOVELIMIT;
	
	/* There is no time advantage to decelerating below the final velocity, therefore only Acc/Dec profiles will be considered */
	dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
	
	/* Saturated? */
	if(dx < dx_u) { /* Peak */
		solution->move = PATH_MOVE_ACCDECPEAK;
		
		/* Compute peak velocity */
		v_p = sqrt(dx * a + (pow2(v_0) + pow2(v_f)) / 2.0);
		
		/* Set solution */
		solution->t_[1] = (v_p - v_0) / a;
		solution->t_[2] = solution->t_[1];
		solution->t_[3] = solution->t_[2] + (v_p - v_f) / a;
		solution->v_[1] = v_p;
		solution->v_[2] = solution->v_[1];
	}
	else { /* Saturated */
		solution->move = PATH_MOVE_ACCDECSATURATED;
		
		/* Compute time at v_max */
		t_12 = (dx - dx_u) / v_max;
		
		/* Set solution */
		solution->t_[1] = (v_max - v_0) / a;
		solution->t_[2] = solution->t_[1] + t_12;
		solution->t_[3] = solution->t_[2] + (v_max - v_f) / a;
		solution->v_[1] = v_max;
		solution->v_[2] = v_max;
	}
	
	/* Set remaining solution */
	solution->dx 	= dx;
	solution->v_[0] = v_0;
	solution->v_[3] = v_f;
	solution->a 	= a;
	
	return PATH_ERROR_NONE;
	
} /* Function definition */
