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
                      MotionProfBaseOutputType *output) {

  /* Local variables */
  double dt_u, dx_12, v_12;

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

  /* Plausible distance */
  if (dt < fabs(v_0 - v_f) / a)
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* There is not distance advantage to using a DecAcc profile over an AccDec 
  profile */
  /* Maximum distance saturation limit */
  dt_u = (2.0 * v_max - v_0 - v_f) / a;

  if (dt < dt_u) {
    output->move = MOTIONPROF_MOVE_ACCDEC;

    v_12 = (dt * a + v_0 + v_f) / 2.0;

    output->dx = (2.0 * pow2(v_12) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
    output->t_[1] = (v_12 - v_0) / a;
    output->t_[2] = output->t_[1];
    output->v_[1] = v_12;
    output->v_[2] = v_12;
  }
  else {
    output->move = MOTIONPROF_MOVE_ACCDEC_SATURATED;

    dx_12 = (dt - (2.0 * v_max - v_0 - v_f) / a) * v_max;

    output->dx = dx_12 + 
                (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);
    output->t_[1] = (v_max - v_0) / a;
    output->t_[2] = output->t_[1] + dx_12 / v_max;
    output->v_[1] = v_max;  
    output->v_[2] = v_max;
  }

  output->t_[3] = dt;
  output->v_[0] = v_0;
  output->v_[3] = v_f;
  output->a = a;

  return 0;
}
