/*******************************************************************************
 * File:      PathPoint.c
 * Author:    Tyler Matijevich
 * Created:   2020-04-01
*******************************************************************************/

#include "PathPlanMain.h"
#define PATH_POINT_SUBNODES (PATH_POINT_INDEX + 1) * 3 - 2

/* Position, velocity, and acceleration at a point in time along linear segments with parabolic blends */
long PathPoint(double x_0, double t_[], double v_[], unsigned char n, double t, double k, PathPlanPointSolutionType* solution) {

	/* Declare local variables */
	short i, ui, a, b, a_dir;
	double ut_[PATH_POINT_SUBNODES], ux_[PATH_POINT_SUBNODES], uv_[PATH_POINT_SUBNODES], ua_[PATH_POINT_SUBNODES], uj_[PATH_POINT_SUBNODES];
	double dt, dv, a_bar, uj;
	
	/* Reset the solution */
	memset(solution, 0, sizeof(*solution));
	
	/* Input requirements */
	/* #1 Number of points */
	if(n < 2 || n > PATH_POINT_INDEX + 1) 
		return PATH_ERROR_POINTLIMIT;
	
	/* #2 Non-decreasing time points */
	for(i = 1; i < n; i++) { 
		if(t_[i] < t_[i - 1]) /* Time points may be numerically equal, e.g. an unsaturated profile solution */
			return PATH_ERROR_TIMESEQUENCE;
	}
	
	/* #3 Request time limit */
	if(t < t_[0] || t > t_[n - 1]) 
		return PATH_ERROR_TIMELIMIT;
	
	/* #4 Jerk factor limit */
	if(k < 1.0 || k > 2.0) 
		return PATH_ERROR_JERKLIMIT;
	
	/* Set initial time, position, and velocity */
	ut_[0] = t_[0];
	ux_[0] = x_0;
	uv_[0] = v_[0];
	
	/* There are n-1 "macro" segments */
	/* There are m-1 "micro" segments where m = (n-1)*3 + 1 */
	/* Loop through each macro segment */
	for(i = 1; i < n; i++) {
		/* Position macro endpoints in micro segment */
		/* 0 1 2 3 4 5 6 7 8 
		   ^     ^     ^ 
		   a     b
		         a     b      */
		b = i * 3;
		a = b - 3;
		
		/* Assign time and velocity at the end node */
		ut_[b] = t_[i];
		uv_[b] = v_[i];
		
		/* Definitions
		        a+1, a+2, a+3 for position
		   a+0, a+1, a+2, a+3 for time and velocity
		   a+0, a+1, a+2      for acceleration and jerk
		*/
		
		/* Check if the time or velocity points are equal */
		if(ut_[a] == ut_[b] || uv_[a] == uv_[b]) {
			/* Stack to the beginning of the macro segment */
			ut_[a + 1] = ut_[a];
			ut_[a + 2] = ut_[a];
			ux_[a + 1] = ux_[a];
			ux_[a + 2] = ux_[a];
			ux_[a + 3] = ux_[a] + uv_[a] * (ut_[a + 3] - ut_[a]); /* ux_[b] */
			uv_[a + 1] = uv_[a];
			uv_[a + 2] = uv_[a];
			ua_[a + 0] = 0.0;
			ua_[a + 1] = 0.0;
			ua_[a + 2] = 0.0;
			uj_[a + 0] = 0.0;
			uj_[a + 1] = 0.0;
			uj_[a + 2] = 0.0;
		}
		else {
			if(uv_[b] > uv_[a]) a_dir = 1; /* Determine the direction of acceleration */
			else a_dir = -1;
			
			/* Determine nominal acceleration magnitude */
			dt = ut_[b] - ut_[a];
			dv = uv_[b] - uv_[a];
			a_bar = fabs(dv) / dt;
			
			if(k == 1.0) { /* For numeric purposes, infinite jerk (protect divide by zero) */
				/* Stack to the beginning of the macro segment */
				ut_[a + 1] = ut_[a];
				ut_[a + 2] = ut_[a];
				ua_[a + 0] = dv / dt;
				ua_[a + 1] = ua_[a];
				ua_[a + 2] = ua_[a];
				uj_[a + 0] = 0.0; /* Technically infinite */
				uj_[a + 1] = 0.0;
				uj_[a + 2] = 0.0;
			}
			else {
				/* a_bar != 0.0 because dv != 0.0 */
				uj = pow2(k * a_bar) / (k * a_bar * dt - fabs(dv)); /* k * a_bar > abs(dv) / dt because k > 1.0 */
				/* Numerator and denominator are non-zero */
				
				ut_[a + 1] = ut_[a] + (k * a_bar) / uj;
				ut_[a + 2] = ut_[b] - (k * a_bar) / uj;
				ua_[a + 0] = 0.0;
				ua_[a + 1] = a_dir * k * a_bar;
				ua_[a + 2] = ua_[a + 1];
				uj_[a + 0] = a_dir * uj;
				uj_[a + 1] = 0.0;
				uj_[a + 2] = (-1.0) * uj_[a];
			}
			/* Compute position and velocity along the sub nodes */
			uv_[a+1] = uv_[a+0] + ua_[a+0] * (ut_[a+1] - ut_[a+0]) + 0.5 * uj_[a+0] * pow2(ut_[a+1] - ut_[a+0]);
			uv_[a+2] = uv_[a+1] + ua_[a+1] * (ut_[a+2] - ut_[a+1]) + 0.5 * uj_[a+1] * pow2(ut_[a+2] - ut_[a+1]);
			ux_[a+1] = ux_[a+0] + uv_[a+0] * (ut_[a+1] - ut_[a+0]) + 0.5 * ua_[a+0] * pow2(ut_[a+1] - ut_[a+0]) + (1.0/6.0) * uj_[a+0] * pow(ut_[a+1] - ut_[a+0], 3.0);
			ux_[a+2] = ux_[a+1] + uv_[a+1] * (ut_[a+2] - ut_[a+1]) + 0.5 * ua_[a+1] * pow2(ut_[a+2] - ut_[a+1]) + (1.0/6.0) * uj_[a+1] * pow(ut_[a+2] - ut_[a+1], 3.0);
			ux_[a+3] = ux_[a+2] + uv_[a+2] * (ut_[a+3] - ut_[a+2]) + 0.5 * ua_[a+2] * pow2(ut_[a+3] - ut_[a+2]) + (1.0/6.0) * uj_[a+2] * pow(ut_[a+3] - ut_[a+2], 3.0);
		}
		
	}
	
	/* Find the requested segment */
	ui = PATH_POINT_SUBNODES; /* Force invalid solution */
	if(t == t_[n - 1]) 
		ui = PATH_POINT_SUBNODES - 1; /* Set as the final micro segment */
	else {
		for(i = 1; i < PATH_POINT_SUBNODES; i++) {
			if(t < ut_[i]) { /* Look ahead at next sub node */
				ui = i - 1;
				break;
			}
		}
	}
	if(ui >= PATH_POINT_SUBNODES) /* Make sure the micro segment was found */
		return PATH_ERROR_TIMELIMIT;
	
	/* Set solution */
	solution->j = uj_[ui];
	solution->a = ua_[ui] + uj_[ui] * (t - ut_[ui]);
	solution->v = uv_[ui] + ua_[ui] * (t - ut_[ui]) + 0.5 * uj_[ui] * pow2(t - ut_[ui]);
	solution->x = ux_[ui] + uv_[ui] * (t - ut_[ui]) + 0.5 * ua_[ui] * pow2(t - ut_[ui]) + (1.0 / 6.0) * uj_[ui] * pow(t - ut_[ui], 3.0);
	
	return PATH_ERROR_NONE;

} /* Function defintion */
