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
                      MotionProfBaseOutputType *output) {

  /* Local variables */
  double v_12, dx_max, dx_min, dt_bar, dx_bar;
  double dx_01_bar, dx_23_bar, dir_01, dir_23;
  double p_2, p_1, p_0;
  SecondOrderRootsOutputType roots_output;
  int32_t roots_status;

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

  /* Plausible distance */
  if (dt < fabs(v_0 - v_f) / a)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* Plausible distance */
  /* Maximum v_12 */
  v_12 = (a * dt + v_0 + v_f) / 2.0;
  if (v_12 <= v_max)
    dx_max = (2.0 * pow2(v_12) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
  else 
    dx_max = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a) +
            v_max * (dt - (2.0 * v_max - v_0 - v_f) / a);
  
  /* Minimum v_12 */
  v_12 = (v_0 + v_f - a * dt) / 2.0;
  if (v_12 >= v_min)
    dx_min = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_12)) / (2.0 * a);
  else 
    dx_min = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a) + 
            v_min * (dt - (v_0 + v_f - 2.0 * v_min) / a);

  if (dx < dx_min || dx_max < dx)
    return MOTIONPROF_ERROR_INPUT_MOVE;
  
  /* Nominal values */
  dt_bar = fabs(v_0 - v_f) / a;
  dx_bar = fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a);
  dx_01_bar = v_0 * (dt - dt_bar) + dx_bar;
  dx_23_bar = dx_bar + (dt - dt_bar) * v_f;
  dir_01 = (dx > dx_01_bar) - (dx < dx_01_bar);
  dir_23 = (dx < dx_23_bar) - (dx > dx_23_bar);

  /* Solve three cases */
  if (dir_01 == dir_23) {
    if (dir_01 > 0.0)
      output->move = MOTIONPROF_MOVE_ACCACC;
    else
      output->move = MOTIONPROF_MOVE_DECDEC;
    
    if (dir_01 == 0.0)
      /* dt == dt_bar and dx == dx_01_bar == dx_23_bar */
      output->v_[1] = v_f;
    else
      output->v_[1] = (dx - dx_bar) / (dt - dt_bar);
  }
  else if (dir_01 > 0.0) {
    output->move = MOTIONPROF_MOVE_ACCDEC_SATURATED;

    p_2 = -1.0;
    p_1 = a * (dt - dt_bar) + 2.0 * fmax(v_0, v_f);
    p_0 = (-1.0) * pow2(fmax(v_0, v_f)) - a * (dx - dx_bar);
  }
  else {
    output->move = MOTIONPROF_MOVE_DECACC_SATURATED;

    p_2 = -1.0;
    p_1 = a * (dt - dt_bar) - 2.0 * fmin(v_0, v_f);
    p_0 = pow2(fmin(v_0, v_f)) - a * (dx - dx_bar);
  }

  /* Find roots */
  if (output->move == MOTIONPROF_MOVE_ACCDEC_SATURATED ||
      output->move == MOTIONPROF_MOVE_DECACC_SATURATED) {

    roots_status = SecondOrderRoots(p_2, p_1, p_0, &roots_output);

    if (roots_status)
      return roots_status;

    if (output->move == MOTIONPROF_MOVE_ACCDEC_SATURATED)
      output->v_[1] = fmin(roots_output.r_1, roots_output.r_2);
    else
      output->v_[1] = fmax(roots_output.r_1, roots_output.r_2);
  }

  output->v_[0] = v_0;
  output->v_[2] = output->v_[1];
  output->v_[3] = v_f;
  output->t_[1] = fabs(output->v_[1] - v_0) / a;
  output->t_[2] = dt - fabs(output->v_[1] - v_f) / a;
  output->t_[3] = dt;
  output->dx = dx;
  output->a = a;

  return 0;
}
