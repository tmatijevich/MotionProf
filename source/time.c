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
                      MotionProfBaseOutputType *output) {

  /* Local variables */
  double dx_u, t_12, v_12;

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

  /* Plausible distance */
  if (dx < fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a))
    return MOTIONPROF_ERROR_INPUT_MOVE;

  /* There is not time advantage to using a DecAcc profile over an AccDec 
  profile */
  /* Maximum distance saturation limit */
  dx_u = (2.0 * pow2(v_max) - pow2(v_0) - pow2(v_f)) / (2.0 * a);

  if (dx < dx_u) {
    output->move = MOTIONPROF_MOVE_ACCDEC;

    v_12 = sqrt(dx * a + (pow2(v_0) + pow2(v_f)) / 2.0);

    output->t_[1] = (v_12 - v_0) / a;
    output->t_[2] = output->t_[1];
    output->t_[3] = output->t_[2] + (v_12 - v_f) / a;
    output->v_[1] = v_12;
    output->v_[2] = v_12;
  }
  else {
    output->move = MOTIONPROF_MOVE_ACCDEC_SATURATED;

    t_12 = (dx - dx_u) / v_max;

    output->t_[1] = (v_max - v_0) / a;
    output->t_[2] = output->t_[1] + t_12;
    output->t_[3] = output->t_[2] + (v_max - v_f) / a;
    output->v_[1] = v_max;
    output->v_[2] = v_max;
  }

  output->dx = dx;
  output->v_[0] = v_0;
  output->v_[3] = v_f;
  output->a = a;
  
  return 0;
}
