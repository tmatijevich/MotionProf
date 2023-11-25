/*******************************************************************************
 * File: acc_time_diff.c
 * Created: 2020-04-01
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file acc_time_diff.c is part of the MotionProf project released under 
 *  the GNU General Public License v3.0 agreement.  For more information, 
 *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Minimum acceleration to achieve moves throughout a time difference */
int32_t MotionProfAccTimeDiff(double dt_tilde, double dx, 
                          double v_0, double v_f, double v_min, double v_max, 
                          MotionProfTimeDiffType *output) {

  /* Local variables */
  double a_u, dt_u, dt_u_hat, dt_u_tilde, c_t_u, c_x_u;
  double a_l, dt_l, dt_l_hat, dt_l_tilde, c_t_l, c_x_l;
  double dt_12, v_12, a;
  double c_1, c_2, c_3, p_0, p_1, p_2;
  SecondOrderRootsOutputType roots_output;
  int32_t flag_roots = false, roots_status, move_ad, move_da, n_ad, n_da;

  /* Check pointer and reset output */
  if (output == NULL)
    return MOTIONPROF_ERROR_NULL_POINTER;
  memset(output, 0, sizeof(*output));

  /* Check inputs against assumptions */
  if (v_min <= 0.0 || v_max <= v_min)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (v_0 < v_min || v_max < v_0 || v_f < v_min || v_max < v_f)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  if (dt_tilde <= 0.0 || dx <= 0.0)
    return MOTIONPROF_ERROR_INPUT_POSITIVE;

  if (dt_tilde >= dx / v_min - dx / v_max)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* Derive nominal values and constants */
  /* AccDec */
  c_t_u = 2.0 * v_max - v_0 - v_f;
  c_x_u = pow2(v_max) - 0.5 * pow2(v_0) - 0.5 * pow2(v_f);
  a_u = c_x_u / dx;
  dt_u = c_t_u / a_u;

  /* DecAcc */
  c_t_l = v_0 + v_f - 2.0 * v_min;
  c_x_l = 0.5 * pow2(v_0) + 0.5 * pow2(v_f) - pow2(v_min);
  a_l = c_x_l / dx;
  dt_l = c_t_l / a_l;

  /* Time durations at alternate profile's saturation limit */
  if (a_u > a_l) {
    /* AccDec is not saturated at a_l */
    v_12 = sqrt(dx * a_l + (pow2(v_0) + pow2(v_f)) / 2.0);
    /* Time duration at lower saturation limit's acceleration of AccDec */
    dt_l_hat = (2.0 * v_12 - v_0 - v_f) / a_l;

    /* DecAcc is saturated at a_u */
    dt_12 = (dx - c_x_l / a_u) / v_min;
    /* Time duration at upper saturation limit's acceleration of DecAcc */
    dt_u_hat = c_t_l / a_u + dt_12;
  }
  else {
    /* AccDec is saturated at a_l */
    dt_12 = (dx - c_x_u / a_l) / v_max;
    /* Time duration at lower saturation limit's acceleration of AccDec */
    dt_l_hat = c_t_u / a_l + dt_12;

    /* DecAcc is saturated at a_u */
    v_12 = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a_u);
    /* Time duration at upper saturation limit's acceleration of DecAcc */
    dt_u_hat = (v_0 + v_f - 2.0 * v_12) / a_u;
  }

  /* Time differences at saturation limits */
  dt_u_tilde = dt_u_hat - dt_u;
  dt_l_tilde = dt_l - dt_l_hat;

  /* Solve for four cases */
  /* 1) dt_tilde >= dt_u_tilde && dt_tilde >= dt_l_tilde */
  /* 2) dt_tilde  < dt_u_tilde && dt_tilde >= dt_l_tilde */
  /* 3) dt_tilde >= dt_u_tilde && dt_tilde  < dt_l_tilde */
  /* 4) dt_tilde  < dt_u_tilde && dt_tilde  < dt_l_tilde */

  if (dt_tilde >= dt_u_tilde && dt_tilde >= dt_l_tilde) {
    move_ad = MOTIONPROF_MOVE_ACCDEC_SATURATED;
    move_da = MOTIONPROF_MOVE_DECACC_SATURATED;
    n_ad = 4;
    n_da = 4;

    a = (c_t_l - c_t_u - (c_x_l / v_min - c_x_u / v_max)) / 
        (dt_tilde - dx * (1.0 / v_min - 1.0 / v_max));
  }
  else if (dt_tilde < dt_u_tilde && dt_tilde >= dt_l_tilde) {
    move_ad = MOTIONPROF_MOVE_ACCDEC;
    move_da = MOTIONPROF_MOVE_DECACC_SATURATED;
    n_ad = 3;
    n_da = 4;
    flag_roots = true;

    c_1 = (pow2(v_0) + pow2(v_f)) / 2.0;
    c_2 = c_t_l - c_x_l / v_min + v_0 + v_f;
    c_3 = dt_tilde - dx / v_min;
    p_2 = pow2(c_3);
    p_1 = -4.0 * dx - 2.0 * c_2 * c_3;
    p_0 = pow2(c_2) - 4.0 * c_1;
  }
  else if (dt_tilde >= dt_u_tilde && dt_tilde < dt_l_tilde) {
    move_ad = MOTIONPROF_MOVE_ACCDEC_SATURATED;
    move_da = MOTIONPROF_MOVE_DECACC;
    n_ad = 4;
    n_da = 3;
    flag_roots = true;

    c_1 = (pow2(v_0) + pow2(v_f)) / 2.0;
    c_2 = c_x_u / v_max - c_t_u + v_0 + v_f;
    c_3 = dt_tilde + dx / v_max;
    p_2 = pow2(c_3);
    p_1 = 4.0 * dx - 2.0 * c_2 * c_3;
    p_0 = pow2(c_2) - 4.0 * c_1;
  }
  /* This requires a higher-order solve
     a larger acceleration magnitude is used sub-optimally
     resulting in a larger time difference */
  else if (dt_u_tilde > dt_l_tilde) {
    move_ad = MOTIONPROF_MOVE_ACCDEC;
    move_da = MOTIONPROF_MOVE_DECACC_SATURATED;
    n_ad = 3;
    n_da = 4;
    a = a_l;
  }
  else {
    move_ad = MOTIONPROF_MOVE_ACCDEC_SATURATED;
    move_da = MOTIONPROF_MOVE_DECACC;
    n_ad = 4;
    n_da = 3;
    a = a_u;
  }

  if (flag_roots) {
    roots_status = SecondOrderRoots(p_2, p_1, p_0, &roots_output);

    if (roots_status)
      return roots_status;

    a = fmax(roots_output.r_1, roots_output.r_2);
  }

  /* AccDec */
  v_12 = sqrt(dx * a + (pow2(v_0) + pow2(v_f)) / 2.0);
  switch (n_ad) {
    case 4:
      dt_12 = (dx - c_x_u / a) / v_max;
      v_12 = v_max;
      output->AccDec.TimePoints[2] = (v_12 - v_0) / a + dt_12;
      output->AccDec.VelocityPoints[2] = v_12;

    case 3:
      output->AccDec.TimePoints[1] = (v_12 - v_0) / a;
      output->AccDec.VelocityPoints[1] = v_12;

    default:
      output->AccDec.TimePoints[n_ad - 1] = output->AccDec.TimePoints[n_ad - 2]
                                          + (v_12 - v_f) / a;
      output->AccDec.VelocityPoints[0] = v_0;  
      output->AccDec.VelocityPoints[n_ad - 1] = v_f;
  }
  output->AccDec.NumberOfPoints = n_ad;
  output->AccDec.Distance = dx;
  output->AccDec.Acceleration = a;
  output->AccDec.MoveType = move_ad;

  /* DecAcc */
  v_12 = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
  switch (n_da) {
    case 4:
      dt_12 = (dx - c_x_l / a) / v_min;
      v_12 = v_min;
      output->DecAcc.TimePoints[2] = (v_0 - v_12) / a + dt_12;
      output->DecAcc.VelocityPoints[2] = v_12;

    case 3:
      output->DecAcc.TimePoints[1] = (v_0 - v_12) / a;
      output->DecAcc.VelocityPoints[1] = v_12;

    default:
      output->DecAcc.TimePoints[n_da - 1] = output->DecAcc.TimePoints[n_da - 2]
                                          + (v_f - v_12) / a;
      output->DecAcc.VelocityPoints[0] = v_0;  
      output->DecAcc.VelocityPoints[n_da - 1] = v_f;
  }
  output->DecAcc.NumberOfPoints = n_da;
  output->DecAcc.Distance = dx;
  output->DecAcc.Acceleration = a;
  output->DecAcc.MoveType = move_da;

  output->TimeDifference = output->DecAcc.TimePoints[n_da - 1] - 
                          output->AccDec.TimePoints[n_ad - 1];

  return 0;
}
