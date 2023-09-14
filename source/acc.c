/*******************************************************************************
 * File: acc.c
 * Created: 2020-04-10
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file acc.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Minimum acceleration to move in time over a distance */
int32_t MotionProfAcc(double dt, double dx, double v_0, double v_f, 
                      double v_min, double v_max, 
                      MotionProfBaseOutputType *output) {

  /* Local variables */
  double dx_bar, dx_u, dx_l, c_x_u, c_t_u, c_x_l, c_t_l, p_2, p_1, p_0;
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

  if (dt <= 0.0 || dx <= 0.0)
    return MOTIONPROF_ERROR_INPUT_POSITIVE;

  /* Plausible distance */
  if (dx <= v_min * dt || v_max * dt <= dx)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* Nominal distance */
  dx_bar = 0.5 * dt * (v_0 + v_f);

  if (dx >= dx_bar) {
    /* Acc/Dec */
    c_x_u = pow2(v_max) - 0.5 * pow2(v_0) - 0.5 * pow2(v_f);
    c_t_u = 2.0 * v_max - v_0 - v_f;

    /* Maximum distance saturation limit */
    dx_u = (c_x_u * dt) / c_t_u;
    /* NOTE: There is no dx >= dx_bar when v_0 = v_f = v_max 
    that passes assumptions */

    if (dx < dx_u)
      output->move = MOTIONPROF_MOVE_ACCDEC;

    else {
      output->move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
      output->a = (c_x_u - v_max * c_t_u) / (dx - v_max * dt);
      if (output->a > 0.0) {
        output->v_[1] = v_max;
        output->v_[2] = v_max;
        output->t_[1] = (v_max - v_0) / output->a;
        output->t_[2] = dt - (v_max - v_f) / output->a;
      }
      else {
        output->v_[1] = v_0;
        output->v_[2] = v_0;
      }
    }
  }
  else {
    /* Dec/Acc */
    c_x_l = 0.5 * pow2(v_0) + 0.5 * pow2(v_f) - pow2(v_min);
    c_t_l = v_0 + v_f - 2.0 * v_min;

    /* Minimum distance saturation limit */
    dx_l = (c_x_l * dt) / c_t_l;
    /* NOTE: There is no dx < dx_bar when v_0 = v_f = v_max 
    that passes assumptions */

    if (dx > dx_l)
      output->move = MOTIONPROF_MOVE_DECACC;

    else {
      output->move = MOTIONPROF_MOVE_DECACC_SATURATED;
      output->a = (c_x_l - v_min * c_t_l) / (dx - v_min * dt);
      if (output->a > 0.0) {
        output->v_[1] = v_min;
        output->v_[2] = v_min;
        output->t_[1] = (v_0 - v_min) / output->a;
        output->t_[2] = dt - (v_f - v_min) / output->a;
      }
      else {
        output->v_[1] = v_0;
        output->v_[2] = v_0;
      }
    }
  }

  /* Find v_12 */
  if (output->move == MOTIONPROF_MOVE_ACCDEC || 
      output->move == MOTIONPROF_MOVE_DECACC) {
    p_2 = 2.0 * dt;
    p_1 = -4.0 * dx;
    p_0 = 2.0 * dx * (v_0 + v_f) - dt * (pow2(v_0) + pow2(v_f));

    roots_status = SecondOrderRoots(p_2, p_1, p_0, &roots_output);

    if (roots_status)
      return roots_status;

    if (output->move == MOTIONPROF_MOVE_ACCDEC) {
      output->v_[1] = fmax(roots_output.r_1, roots_output.r_2);
      output->v_[2] = output->v_[1];
    }
    else {
      output->v_[1] = fmin(roots_output.r_1, roots_output.r_2);
      output->v_[2] = output->v_[1];
    }

    output->a = fabs(2.0 * output->v_[1] - v_0 - v_f) / dt;
    if (output->a > 0.0) {
      output->t_[1] = fabs(output->v_[1] - v_0) / output->a;
      output->t_[2] = output->t_[1];
    }
  }

  output->t_[3] = dt;
  output->dx = dx;
  output->v_[0] = v_0;
  output->v_[3] = v_f;

  return 0;
}
