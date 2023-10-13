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
  uint8_t equal_to_dx_bar, equal_to_v_max, equal_to_v_min, invalid_move, 
          flag_roots = false;

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

  /* Nominal distance */
  dx_bar = 0.5 * dt * (v_0 + v_f);
  equal_to_dx_bar = dx == dx_bar;

  /* Plausible distance */
  invalid_move = dx < v_min * dt || v_max * dt < dx;
  
  equal_to_v_min = v_0 == v_min && v_f == v_min;
  invalid_move |= dx == v_min * dt && (!equal_to_dx_bar || !equal_to_v_min);
  
  equal_to_v_max = v_0 == v_max && v_f == v_max;
  invalid_move |= dx == v_max * dt && (!equal_to_dx_bar || !equal_to_v_max);

  if (invalid_move)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  if (dx > dx_bar) {
    /* Acc/Dec */
    c_x_u = pow2(v_max) - 0.5 * pow2(v_0) - 0.5 * pow2(v_f);
    c_t_u = 2.0 * v_max - v_0 - v_f;

    /* Distance at upper saturation limit */
    dx_u = (c_x_u * dt) / c_t_u;

    if (dx > dx_u) {
      move = MOTIONPROF_MOVE_ACCDEC_SATURATED;
      n = 4;
      a = (v_max * c_t_u - c_x_u) / (v_max * dt - dx);
      /*   v_max * dt > dx > dx_u */
      /*        v_max * dt > (c_x_u * dt) / c_t_u */
      /* ==> v_max * c_t_u > c_x_u */
      /* Both numerator and denominator are positive */
      v_12 = v_max;
      t_1 = (v_max - v_0) / a;
      t_2 = dt - (v_max - v_f) / a;
    }
    else if (dx == dx_u) {
      move = MOTIONPROF_MOVE_ACCDEC;
      n = 3;
      a = 2.0 * v_max - v_0 - v_f / dt;
      v_12 = v_max;
      t_1 = (v_max - v_0) / a;
    }
    else {
      move = MOTIONPROF_MOVE_ACCDEC;
      flag_roots = true;
    }
  }
  else if (equal_to_dx_bar) {
    if (v_f > v_0) move = MOTIONPROF_MOVE_ACC;
    else if (v_f == v_0) move = MOTIONPROF_MOVE_NONE;
    else move = MOTIONPROF_MOVE_DEC;
    n = 2;
    a = fabs(v_f - v_0) / dt;
  }
  else {
    /* Dec/Acc */
    c_x_l = 0.5 * pow2(v_0) + 0.5 * pow2(v_f) - pow2(v_min);
    c_t_l = v_0 + v_f - 2.0 * v_min;

    /* Distance at lower saturation limit */
    dx_l = (c_x_l * dt) / c_t_l;

    if (dx > dx_l) {
      move = MOTIONPROF_MOVE_DECACC;
      flag_roots = true;
    }
    else if (dx == dx_l) {
      move = MOTIONPROF_MOVE_DECACC;
      n = 3;
      a = (v_0 + v_f - 2.0 * v_min) / dt;
      v_12 = v_min;
      t_1 = (v_0 - v_min) / a;
    }
    else {
      move = MOTIONPROF_MOVE_DECACC_SATURATED;
      n = 4;
      a = (c_x_l - v_min * c_t_l) / (dx - v_min * dt);
      /*   v_min * dt < dx < dx_l */
      /*        v_min * dt < (c_x_l * dt) / c_t_l */
      /* ==> v_min * c_t_l < c_x_l*/
      v_12 = v_min;
      t_1 = (v_0 - v_min) / a;
      t_2 = dt - (v_f - v_min) / a;
    }
  }

  /* Find v_12 */
  if (flag_roots) {
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
    n = 3;
    t_1 = fabs(v_12 - v_0) / a;
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
  output->MoveType = move;

  return 0;
}
