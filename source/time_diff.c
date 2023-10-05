/*******************************************************************************
 * File: time_diff.c
 * Created: 2020-03-25
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file time_diff.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

/* Maximum difference in time duration between fastest and slowest profiles */
int32_t MotionProfTimeDiff(double dx, double v_0, double v_f, double v_min,
                          double v_max, double a, 
                          MotionProfTimeDiffOutputType *output) {

  /* Local variables */
  int32_t time_status;
  double dx_l, dt_12;

  /* Check pointer and reset output */
  if (output == NULL)
    return MOTIONPROF_ERROR_NULL_POINTER;
  memset(output, 0, sizeof(*output));
  
  /* Use MotionProfTime for time-minimizing profile */
  time_status = MotionProfTime(dx, v_0, v_f, v_min, v_max, a, &output->AccDec);
  if (time_status)
    return time_status;

  /* Derive time-maximizing profile */
  /* Minimum distance saturation limit */
  dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a);

  if (dx < dx_l) {
    output->DecAcc.move = MOTIONPROF_MOVE_DECACC;
    output->DecAcc.v_[1] = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
    output->DecAcc.v_[2] = output->DecAcc.v_[1];
    output->DecAcc.t_[1] = (v_0 - output->DecAcc.v_[1]) / a;
    output->DecAcc.t_[2] = output->DecAcc.t_[1];
    output->DecAcc.t_[3] = output->DecAcc.t_[2] + 
                          (v_f - output->DecAcc.v_[1]) / a;
  }
  else {
    output->DecAcc.move = MOTIONPROF_MOVE_DECACC_SATURATED;
    dt_12 = (dx - dx_l) / v_min;
    output->DecAcc.v_[1] = v_min;
    output->DecAcc.v_[2] = v_min;
    output->DecAcc.t_[1] = (v_0 - v_min) / a;
    output->DecAcc.t_[2] = output->DecAcc.t_[1] + dt_12;
    output->DecAcc.t_[3] = output->DecAcc.t_[2] + (v_f - v_min) / a;
  }

  output->AccDec.dx = dx;
  output->AccDec.v_[0] = v_0;
  output->AccDec.v_[3] = v_f;
  output->AccDec.a = a;
  output->DecAcc.dx = dx;
  output->DecAcc.v_[0] = v_0;
  output->DecAcc.v_[3] = v_f;
  output->DecAcc.a = a;
  output->dt_tilde = output->DecAcc.t_[3] - output->AccDec.t_[3];

  return 0;
}
