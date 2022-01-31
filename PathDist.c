/*******************************************************************************
 * File:      PathDist.c
 * Author:    Tyler Matijevich
 * Created:   2020-12-29
*******************************************************************************/

#include "PathPlanMain.h"

/* Maximum distance from move with acceleration in time */
long PathDist(double dt, double v_0, double v_f, double v_min, double v_max, double a, PathPlanBaseSolutionType *solution) {
	
	/* Declare local variables */
	double dt_u, dx_12, v_12;
	
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
	else if(dt <= 0.0 || a <= 0.0)
		return PATH_ERROR_NONPOSITIVE;
	
	/* #4 Plausible move */
	else if(dt < fabs(v_0 - v_f) / a)
		return PATH_ERROR_MOVELIMIT;
	
	/* There is no advantage from decelerating then accelerating during move, only Acc/Dec profiles will be considered */
	dt_u = (2.0 * v_max - v_0 - v_f) / a;
	
	/* Saturated? */
	if(dt < dt_u) { /* Peak */
		solution->move = PATH_MOVE_ACCDECPEAK;
		
		/* Compute intermediate velocity */
		v_12 = (dt * a + v_0 + v_f) / 2.0;
		
		/* Set solution */
		solution->dx = (2.0 * pow2(v_12) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
		solution->t_[1] = (v_12 - v_0) / a;
		solution->t_[2] = solution->t_[1];
		solution->v_[1] = v_12;
		solution->v_[2] = v_12;
	}
	else { /* Saturated */
		solution->move = PATH_MOVE_ACCDECSATURATED;
		
		/* Compute distance at v_max */
		dx_12 = (dt - (2.0 * v_max - v_0 - v_f) / a) * v_max;
		
		/* Set solution */
		solution->dx = dx_12 + (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
		solution->t_[1] = (v_max - v_0) / a;
		solution->t_[2] = solution->t_[1];
		solution->v_[1] = v_max;
		solution->v_[2] = v_max;
	}
	
	/* Set remaining solution */
	solution->t_[3] = dt;
	solution->v_[0] = v_0;
	solution->v_[3] = v_f;
	solution->dx 	= (pow2(solution->v_[1]) - pow2(solution->v_[0])) / (2.0 * a) + (solution->t_[2] - solution->t_[1]) * solution->v_[1] + (pow2(solution->v_[2]) - pow2(solution->v_[3])) / (2.0 * a);
	solution->a 	= a;
	
	return PATH_ERROR_NONE;
	
} /* Function defintion */
