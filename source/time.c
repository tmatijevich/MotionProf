/*******************************************************************************
 * File: time.c
 * Created: 2020-12-23
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file time.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Minimum time duration to move with acceleration over a distance */
int32_t MotionProfTime(double dx, double v_0, double v_f, 
                      double v_min, double v_max, double a, 
                      MotionProfProfileType *output) {

  /* Local variables */
  double dx_bar, dx_u, t_12, v_12;
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

  if (dx <= 0.0 || a <= 0.0)
    return MOTIONPROF_ERROR_INPUT_POSITIVE;

  /* Nominal distance */
  dx_bar = fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a);

  /* Plausible distance */
  if (dx < dx_bar)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* There is not time advantage to using a DecAcc profile over an AccDec 
  profile */
  /* Maximum distance saturation limit */
  dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);

  if (dx == dx_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else if (v_0 == v_f) move = MOTIONPROF_MOVE_ZERO;
    else move = MOTIONPROF_MOVE_DEC;
    n = 2;
    v_12 = v_0;
  }
  
  else if (dx > dx_u) {
    move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
    n = 4;
    t_12 = (dx - dx_u) / v_max;
    v_12 = v_max;
  }

  else if (dx == dx_u) {
    move = MOTIONPROF_MOVE_ACCDEC;
    n = 3;
    v_12 = v_max;
  }

  else {
    move = MOTIONPROF_MOVE_ACCDEC;
    n = 3;
    v_12 = sqrt(dx * a + (pow2(v_0) + pow2(v_f)) / 2.0);
  }

  switch(n) {
    case 4:
      output->TimePoints[2] = (v_12 - v_0) / a + t_12;
      output->VelocityPoints[2] = v_12;
    case 3:
      output->TimePoints[1] = (v_12 - v_0) / a;
      output->VelocityPoints[1] = v_12;
    default:
      output->TimePoints[n - 1] = output->TimePoints[n - 2] + 
                                  fabs(v_12 - v_f) / a;
      output->VelocityPoints[0] = v_0;
      output->VelocityPoints[n - 1] = v_f;
  }
  output->NumberOfPoints = n;
  output->Distance = dx;
  output->Acceleration = a;
  output->MoveType = move;
  
  return 0;
}
