/*******************************************************************************
 * File: vel.c
 * Created: 2020-12-30
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file vel.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Minimum velocity to move with acceleration in time over a distance */
int32_t MotionProfVel(double dt, double dx, double v_0, double v_f,
                      double v_min, double v_max, double a,
                      MotionProfProfileType *output) {

  /* Local variables */
  double v_12, dt_01, dt_23, dt_bar, dt_01_hold = 0.0;
  double dx_u, dx_l, dx_max, dx_min, dx_bar;
  double dx_01_bar, dx_23_bar, dir_01, dir_23;
  double p_2, p_1, p_0;
  SecondOrderRootsOutputType roots_output;
  int32_t roots_status, move, n;
  uint8_t flag_roots = false;

  /* Check pointer and reset output */
  if (output == NULL)
    return MOTIONPROF_ERROR_NULL_POINTER;
  memset(output, 0, sizeof(*output));

  /* Check inputs against assumptions */
  if (v_min < 0.0 || v_max <= v_min)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (v_0 < v_min || v_max < v_0 || v_f < v_min || v_max < v_f)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (dt <= 0.0 || dx <= 0.0 || a <= 0.0)
    return MOTIONPROF_ERROR_INPUT_POSITIVE;

  /* Nominal duration */
  dt_bar = fabs(v_0 - v_f) / a;

  /* Plausible duration */
  if (dt < dt_bar)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* Plausible distance */
  /* Maximum v_12 */
  v_12 = (a * dt + v_0 + v_f) / 2.0;
  if (v_12 <= v_max)
    dx_max = (2.0 * pow2(v_12) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
  else {
    dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
    dx_max = dx_u + v_max * (dt - (2.0 * v_max - v_0 - v_f) / a);
  }
  
  /* Minimum v_12 */
  v_12 = (v_0 + v_f - a * dt) / 2.0;
  if (v_12 >= v_min)
    dx_min = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_12)) / (2.0 * a);
  else {
    dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a);
    dx_min = dx_l + v_min * (dt - (v_0 + v_f - 2.0 * v_min) / a);
  }

  if (dx < dx_min || dx_max < dx)
    return MOTIONPROF_ERROR_INPUT_MOVE;
  
  /* Nominal values */
  dx_bar = fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a);
  dx_01_bar = v_0 * (dt - dt_bar) + dx_bar;
  dx_23_bar = dx_bar + (dt - dt_bar) * v_f;
  dir_01 = (dx > dx_01_bar) - (dx < dx_01_bar);
  dir_23 = (dx < dx_23_bar) - (dx > dx_23_bar);

  if (dx == dx_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else if (v_0 == v_f) move = MOTIONPROF_MOVE_ZERO;
    else move = MOTIONPROF_MOVE_DEC;
    n = 2;
    /* Use v_12 variable as a placeholder for simplified arithmetic */
    v_12 = v_0;
  }
  else if (dx == dx_01_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else move = MOTIONPROF_MOVE_DEC;
    n = 3;
    v_12 = v_0;
    dt_01_hold = dt - dt_bar;
  }
  else if (dx == dx_23_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else move = MOTIONPROF_MOVE_DEC;
    n = 3;
    v_12 = v_f;
  }
  else if (dir_01 == dir_23) {
    if (dir_01 > 0.0) move = MOTIONPROF_MOVE_ACCACC;
    else move = MOTIONPROF_MOVE_DECDEC;
    n = 4;
    v_12 = (dx - dx_bar) / (dt - dt_bar);
  }
  else if (dir_01 > 0.0) {
    flag_roots = true;
    p_2 = -1.0;
    p_1 = a * (dt - dt_bar) + 2.0 * fmax(v_0, v_f);
    p_0 = -1.0 * pow2(fmax(v_0, v_f)) - a * (dx - dx_bar);
  }
  else {
    flag_roots = true;
    p_2 = -1.0;
    p_1 = a * (dt - dt_bar) - 2.0 * fmin(v_0, v_f);
    p_0 = pow2(fmin(v_0, v_f)) - a * (dx - dx_bar);
  }

  /* Find roots */
  if (flag_roots) {
    roots_status = SecondOrderRoots(p_2, p_1, p_0, &roots_output);

    if (roots_status)
      return roots_status;

    n = 4;
    if (dir_01 > 0.0) {
      v_12 = fmin(roots_output.r_1, roots_output.r_2);
      if (v_12 == v_max) move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
      else move = MOTIONPROF_MOVE_ACCDEC;
    }
    else {
      v_12 = fmax(roots_output.r_1, roots_output.r_2);
      if (v_12 == v_min) move = MOTIONPROF_MOVE_DECACC_SATURATED;
      else move = MOTIONPROF_MOVE_DECACC;
    }
  }

  dt_01 = fabs(v_0 - v_12) / a + dt_01_hold;
  dt_23 = fabs(v_12 - v_f) / a;
  switch(n) {
    case 4:
      output->TimePoints[2] = dt - dt_23;
      output->VelocityPoints[2] = v_12;
    case 3:
      output->TimePoints[1] = dt_01;
      output->VelocityPoints[1] = v_12;
    default:
      output->TimePoints[n - 1] = dt;
      output->VelocityPoints[0] = v_0;
      output->VelocityPoints[n - 1] = v_f;
  }
  output->NumberOfPoints = n;
  output->Distance = dx;
  output->Acceleration = a;
  output->MoveType = move;

  return 0;
}
