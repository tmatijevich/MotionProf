/*******************************************************************************
 * File:      PathVel.c
 * Author:    Tyler Matijevich
 * Created:   2020-12-30
*******************************************************************************/

#include "PathPlanMain.h"

/* Maximum distance from move with acceleration in time */
long PathVel(double dt, double dx, double v_0, double v_f, double v_min, double v_max, double a, PathPlanBaseSolutionType *solution) {
	
	/* Declare local variables */
	double dt_bar, dx_min, dx_max, dx_bar, dx_a1, dx_a2, v_12, p_2, p_1, p_0;
	short sign_a1, sign_a2;
	long rootsReturn;
	PathPlanRootsSolutionType rootsSolution;
	
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
	/* Check duration */
	else if(dt < fabs(v_0 - v_f) / a)
		return PATH_ERROR_MOVELIMIT;
		
	/* Check distance */
	v_12 = (a * dt + v_0 + v_f) / 2.0; /* Test maximum v_12 */
	if(v_12 <= v_max) /* Peak */
		dx_max = (2.0 * pow2(v_12) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
	else /* Saturated */
		dx_max = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a) + v_max * (dt - ((2.0 * v_max - v_0 - v_f) / a));
	
	v_12 = (v_0 + v_f - a * dt) / 2.0; /* Test minimum v_12 */
	if(v_12 >= v_min) /* Peak (dip) */
		dx_min = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_12)) / (2.0 * a);
	else /* Saturated */
		dx_min = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a) + v_min * (dt - ((v_0 + v_f - 2.0 * v_min) / a));
	
	if(dx < dx_min || dx > dx_max) 
		return PATH_ERROR_MOVELIMIT;
	
	/* Compute nominal distance and time */
	dt_bar 	= fabs(v_0 - v_f) / a;
	dx_bar 	= fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a);
	dx_a1 	= v_0 * (dt - dt_bar) + dx_bar; /* Hold v_0 then accelerate to v_f */
	dx_a2 	= dx_bar + v_f * (dt - dt_bar); /* Accelerate to v_f then hold v_f */
	
	/* Compute sign of acceleration phases */
	sign_a1 = (short)(dx > dx_a1) - (short)(dx < dx_a1);
	sign_a2 = (short)(dx < dx_a2) - (short)(dx > dx_a2);
	
	/* Three cases */
	if(sign_a1 == sign_a2) {
		solution->move = sign_a1 >= 0 ? PATH_MOVE_ACCACC : PATH_MOVE_DECDEC;
		solution->v_[1] = sign_a1 == 0 ? v_f : (dx - dx_bar) / (dt - dt_bar);
	}
	else if(sign_a1 == 1 && sign_a2 == -1) { /* Acc/Dec */
		solution->move = PATH_MOVE_ACCDECSATURATED;
		
		p_2 = -1.0;
		p_1 = a * (dt - dt_bar) + 2.0 * fmax(v_0, v_f);
		p_0 = (-1.0) * pow2(fmax(v_0, v_f)) - a * (dx - dx_bar);
	}
	else { /* Dec/Acc */
		solution->move = PATH_MOVE_DECACCSATURATED;
		
		p_2 = 1.0;
		p_1 = a * (dt - dt_bar) - 2.0 * fmax(v_0, v_f);
		p_0 = pow2(fmax(v_0, v_f)) - a * (dx - dx_bar);
	}
	
	/* Find roots */
	if(solution->move == PATH_MOVE_ACCDECSATURATED || solution->move == PATH_MOVE_DECACCSATURATED) {
		rootsReturn = PathRoots(p_2, p_1, p_0, &rootsSolution);
		
		if(rootsReturn != PATH_ERROR_NONE)
			return rootsReturn;
		
		solution->v_[1] = solution->move == PATH_MOVE_ACCDECSATURATED ? fmin(rootsSolution.r_1, rootsSolution.r_2) : fmax(rootsSolution.r_1, rootsSolution.r_2);
	}
	
	/* Set remaining solution */
	solution->dx 	= dx;
	solution->v_[0] = v_0;
	solution->v_[2] = solution->v_[1];
	solution->v_[3] = v_f;
	solution->t_[1] = fabs(v_0 - solution->v_[1]) / a;
	solution->t_[2] = fmax(dt - fabs(solution->v_[2] - v_f) / a, solution->t_[1]); /* Ensure non-decreasing time given floating point computation */
	solution->t_[3] = dt;
	solution->a 	= a;
	
	return PATH_ERROR_NONE;

} /* Function defintiion */
