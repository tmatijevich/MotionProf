/*******************************************************************************
 * File: point.c
 * Created: 2020-04-01
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file point.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

#define SUBSEGMENT_MAX 3 * MOTIONPROF_POINT_MAX + 1

/* Velocity profile point interpolation */
int32_t MotionProfPoint(double x_0, double t_[], double v_[], uint8_t n, 
                        double t, double k, MotionProfPointOutputType *output) {
  
  /* Local variables */
  int8_t i, a, b, direction, m, u;
  double ut_[SUBSEGMENT_MAX] = {0}, ux_[SUBSEGMENT_MAX] = {0}, 
          uv_[SUBSEGMENT_MAX] = {0}, ua_[SUBSEGMENT_MAX] = {0}, 
          uj_[SUBSEGMENT_MAX] = {0};
  double dt, dv, a_bar, j;

  /* Check pointer and reset output */
  if (output == NULL)
    return -1;
  memset(output, 0, sizeof(*output));

  /* Check inputs against assumptions */
  /* Two or more points in velocity profile 2 <= n <= 21 */
  if (n < 2 || MOTIONPROF_POINT_MAX + 1 < n)
    return -1;
  
  /* Sequential time points t_i <= t_i+1*/
  for (i = 1; i < n; i++) {
    if (t_[i] < t_[i - 1])
      return -1;
  }

  /* Time interpolation within series t_0 <= t <= t_f */
  if (t < t_[0] || t_[n - 1] < t)
    return -1;

  /* Saturate jerk factor 1.0 <= k <= 2.0 */
  k = fmin(fmax(k, 1.0), 2.0);

  /* Initial time, position, and velocity */
  ut_[0] = t_[0];
  ux_[0] = x_0;
  uv_[0] = v_[0];

  /* Create each segment */
  for (i = 1; i < n; i++) {
    /* Derive subsegments with jerk */
    /* 
        0 1 2 3 4 5 6 7 8 
        ^     ^     ^
        a     b
              a     b 
    */
    a = (i - 1) * 3;
    b = i * 3;

    ut_[b] = t_[i];
    uv_[b] = v_[i];

    dt = ut_[b] - ut_[a];
    dv = uv_[b] - uv_[a];

    /* Constant velocity or no change in time */
    if (dt == 0 || dv == 0) {
      /* Stack */
      ut_[a + 1] = ut_[a];
      ut_[a + 2] = ut_[a];
      ux_[a + 1] = ux_[a];
      ux_[a + 2] = ux_[a];
      ux_[a + 3] = ux_[a] + uv_[a] * dt;
      uv_[a + 1] = uv_[a];
      uv_[a + 2] = uv_[a];
      ua_[a + 0] = 0.0;
      ua_[a + 1] = 0.0;
      ua_[a + 2] = 0.0;
      uj_[a + 0] = 0.0;
      uj_[a + 1] = 0.0;
      uj_[a + 2] = 0.0;
      continue;
    }

    /* Determine direction and nominal values */
    direction = dv > 0.0 ? 1 : -1;
    a_bar = fabs(dv) / dt;

    if (k == 1.0) {
      /* Constant acceleration and infinite jerk */
      ut_[a + 1] = ut_[a];
      ut_[a + 2] = ut_[a];
      ua_[a + 0] = dv / dt;
      ua_[a + 1] = ua_[a];
      ua_[a + 2] = ua_[a];
      uj_[a + 0] = 0.0;
      uj_[a + 1] = 0.0;
      uj_[a + 2] = 0.0;
    }
    else {
      j = pow2(k * a_bar) / (k * a_bar * dt - fabs(dv));
      ut_[a + 1] = ut_[a] + (k * a_bar) / j;
      ut_[a + 2] = ut_[b] - (k * a_bar) / j;
      ua_[a + 0] = 0.0;
      ua_[a + 1] = direction * k * a_bar;
      ua_[a + 2] = ua_[a + 1];
      uj_[a + 0] = direction * j;
      uj_[a + 1] = 0.0;
      uj_[a + 2] = -1.0 * uj_[a + 0];
    }

    /* Interpolate subsegments */
    uv_[a+1] = uv_[a+0] + ua_[a+0] * (ut_[a+1] - ut_[a+0]) + 0.5 * uj_[a+0] * pow2(ut_[a+1] - ut_[a+0]);
    uv_[a+2] = uv_[a+1] + ua_[a+1] * (ut_[a+2] - ut_[a+1]) + 0.5 * uj_[a+1] * pow2(ut_[a+2] - ut_[a+1]);
    ux_[a+1] = ux_[a+0] + uv_[a+0] * (ut_[a+1] - ut_[a+0]) + 0.5 * ua_[a+0] * pow2(ut_[a+1] - ut_[a+0]) + (1.0/6.0) * uj_[a+0] * pow(ut_[a+1] - ut_[a+0], 3.0);
    ux_[a+2] = ux_[a+1] + uv_[a+1] * (ut_[a+2] - ut_[a+1]) + 0.5 * ua_[a+1] * pow2(ut_[a+2] - ut_[a+1]) + (1.0/6.0) * uj_[a+1] * pow(ut_[a+2] - ut_[a+1], 3.0);
    ux_[a+3] = ux_[a+2] + uv_[a+2] * (ut_[a+3] - ut_[a+2]) + 0.5 * ua_[a+2] * pow2(ut_[a+3] - ut_[a+2]) + (1.0/6.0) * uj_[a+2] * pow(ut_[a+3] - ut_[a+2], 3.0);
  }

  /* Derive the number of subsegments */
  m = (n - 1) * 3 + 1;

  /* Find the subsegment for interpolation */
  if (t == t_[n - 1])
    u = m - 2;
  else {
    for (i = 1; i < m; i++) {
      if (t < ut_[i]) {
        u = i - 1;
        break;
      }
    }
  }

  /* Interpolate and set output */
  output->j = uj_[u];
  output->a = ua_[u] + uj_[u] * (t - ut_[u]);
  output->v = uv_[u] + ua_[u] * (t - ut_[u]) + 0.5 * uj_[u] * pow2(t - ut_[u]);
  output->x = ux_[u] + uv_[u] * (t - ut_[u]) + 0.5 * ua_[u] * pow2(t - ut_[u]) + (1.0 / 6.0) * pow(t - ut_[u], 3.0);
  
  return 0;
}
