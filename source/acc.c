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
                      MotionProfProfileType *output) {

  /* Local variables */
  double dx_bar, dx_u, dx_l, c_x_u, c_t_u, c_x_l, c_t_l, p_2, p_1, p_0;
  SecondOrderRootsOutputType roots_output;
  int32_t roots_status, move, n;
  double t_1, t_2, v_12, a;

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
      move = MOTIONPROF_MOVE_ACCDEC;

    else {
      move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
      a = (c_x_u - v_max * c_t_u) / (dx - v_max * dt);
      if (a > 0.0) {
        n = 4;
        v_12 = v_max;
        t_1 = (v_max - v_0) / a;
        t_2 = dt - (v_max - v_f) / a;
      }
      else 
        n = 2;
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
      move = MOTIONPROF_MOVE_DECACC;

    else {
      move = MOTIONPROF_MOVE_DECACC_SATURATED;
      a = (c_x_l - v_min * c_t_l) / (dx - v_min * dt);
      if (a > 0.0) {
        n = 4;
        v_12 = v_min;
        t_1 = (v_0 - v_min) / a;
        t_2 = dt - (v_f - v_min) / a;
      }
      else 
        n = 2;
    }
  }

  /* Find v_12 */
  if (move == MOTIONPROF_MOVE_ACCDEC || 
      move == MOTIONPROF_MOVE_DECACC) {
    p_2 = 2.0 * dt;
    p_1 = -4.0 * dx;
    p_0 = 2.0 * dx * (v_0 + v_f) - dt * (pow2(v_0) + pow2(v_f));

    roots_status = SecondOrderRoots(p_2, p_1, p_0, &roots_output);

    if (roots_status)
      return roots_status;

    if (move == MOTIONPROF_MOVE_ACCDEC) 
      v_12 = fmax(roots_output.r_1, roots_output.r_2);
    else 
      v_12 = fmin(roots_output.r_1, roots_output.r_2);

    a = fabs(2.0 * v_12 - v_0 - v_f) / dt;
    if (a > 0.0) {
      n = 3;
      t_1 = fabs(v_12 - v_0) / a;
    }
    else 
      n = 2;
  }

  switch(n) {
    case 4:
      output->TimePoints[2] = t_2;
      output->VelocityPoints[2] = v_12;
    case 3:
      output->TimePoints[1] = t_1;
      output->VelocityPoints[1] = v_12;
    default:
      output->TimePoints[n - 1] = dt;
      output->VelocityPoints[0] = v_0;
      output->VelocityPoints[n - 1] = v_f;
  }
  output->NumberOfPoints = n;
  output->Distance = dx;
  output->Acceleration = a;
  output->Move = move;

  return 0;
}
