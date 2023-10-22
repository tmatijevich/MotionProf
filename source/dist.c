/*******************************************************************************
 * File: dist.c
 * Created: 2020-12-29
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file dist.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Maximum distance from move with acceleration in time */
int32_t MotionProfDist(double dt, double v_0, double v_f,
                      double v_min, double v_max, double a,
                      MotionProfProfileType *output) {

  /* Local variables */
  double dt_bar, dt_u, dx_01, dx_12, dx_23, v_12;
  int32_t move, n;

  /* Check pointer and reset output */
  if (output == NULL)
    return MOTIONPROF_ERROR_NULL_POINTER;
  memset(output, 0, sizeof(*output));

  /* Check inputs against assumptions */
  if (v_min < 0.0 || v_max <= v_min)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (v_0 < v_min || v_max < v_0 || v_f < v_min || v_max < v_f)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (dt <= 0.0 || a <= 0.0)
    return MOTIONPROF_ERROR_INPUT_POSITIVE;

  /* Nominal time */
  dt_bar = fabs(v_0 - v_f) / a;

  /* Plausible time */
  /* Profile duration cannot subceed accelerating from v_0 to v_f */
  if (dt < dt_bar)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* There is no maximum distance advantage to 
     using a dec/acc profile over an acc/dec profile */
  /* Duration at upper saturation limit */
  dt_u = (2.0 * v_max - v_0 - v_f) / a;

  if (dt == dt_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else if (v_0 == v_f) move = MOTIONPROF_MOVE_ZERO;
    else move = MOTIONPROF_MOVE_DEC;
    n = 2;
    /* Use v_12 variable as a placeholder for simplified arithmetic */
    v_12 = v_0;
  }
  else if (dt > dt_u) {
    move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
    n = 4;
    dx_12 = (dt - (2.0 * v_max - v_0 - v_f) / a) * v_max;
    v_12 = v_max;
  }
  else if (dt == dt_u) {
    move = MOTIONPROF_MOVE_ACCDEC;
    n = 3;
    v_12 = v_max;
  }
  else {
    move = MOTIONPROF_MOVE_ACCDEC;
    n = 3;
    v_12 = (dt * a + v_0 + v_f) / 2.0;
  }

  dx_01 = fabs(pow2(v_12) - pow2(v_0)) / (2.0 * a);
  dx_23 = fabs(pow2(v_12) - pow2(v_f)) / (2.0 * a);
  switch(n) {
    case 4:
      output->Distance += dx_12;
      output->TimePoints[2] = (v_12 - v_0) / a + dx_12 / v_12;
      output->VelocityPoints[2] = v_12;
    case 3:
      output->TimePoints[1] = (v_12 - v_0) / a;
      output->VelocityPoints[1] = v_12;
    default:
      output->Distance += dx_01 + dx_23;
      output->TimePoints[n - 1] = output->TimePoints[n - 2] + 
                                  fabs(v_12 - v_f) / a;
      output->VelocityPoints[0] = v_0;
      output->VelocityPoints[n - 1] = v_f;
  }
  output->NumberOfPoints = n;
  output->Acceleration = a;
  output->MoveType = move;

  return 0;
}
