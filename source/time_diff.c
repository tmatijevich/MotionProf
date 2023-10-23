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
                          MotionProfTimeDiffType *output) {

  /* Local variables */
  int32_t time_status, move, n;
  double dx_bar, dx_l, dt_12, v_12;

  /* Check pointer and reset output */
  if (output == NULL)
    return MOTIONPROF_ERROR_NULL_POINTER;
  memset(output, 0, sizeof(*output));
  
  /* Use MotionProfTime for time-minimizing profile */
  time_status = MotionProfTime(dx, v_0, v_f, v_min, v_max, a, &output->AccDec);
  if (time_status)
    return time_status;

  /* Additional check on minimum velocity */
  /* The time difference could be infinite if the minimum velocity is zero */
  if (v_min <= 0.0)
    return MOTIONPROF_ERROR_INPUT_VELOCITY;

  /* Derive time-maximizing profile */
  /* Nominal distance */
  dx_bar = fabs(pow2(v_0) - pow2(v_f)) / (2.0 * a);

  /* Distance and lower saturation limit */
  dx_l = (pow2(v_0) + pow2(v_f) - 2.0 * pow2(v_min)) / (2.0 * a);

  if (dx == dx_bar) {
    if (v_0 < v_f) move = MOTIONPROF_MOVE_ACC;
    else if (v_0 == v_f) move = MOTIONPROF_MOVE_ZERO;
    else move = MOTIONPROF_MOVE_DEC;
    n = 2;
    v_12 = v_0;
  }
  else if (dx < dx_l) {
    move = MOTIONPROF_MOVE_DECACC;
    n = 3;
    v_12 = sqrt((pow2(v_0) + pow2(v_f)) / 2.0 - dx * a);
  }
  else if (dx == dx_l) {
    move = MOTIONPROF_MOVE_DECACC;
    n = 3;
    v_12 = v_min;
  }
  else {
    move = MOTIONPROF_MOVE_DECACC_SATURATED;
    n = 4;
    dt_12 = (dx - dx_l) / v_min;
    v_12 = v_min;
  }

  switch (n) {
    case 4:
      output->DecAcc.TimePoints[2] = (v_0 - v_12) / a + dt_12;
      output->DecAcc.VelocityPoints[2] = v_12;
    case 3:
      output->DecAcc.TimePoints[1] = (v_0 - v_12) / a;
      output->DecAcc.VelocityPoints[1] = v_12;
    default:
      output->DecAcc.TimePoints[n - 1] = output->DecAcc.TimePoints[n - 2] + 
                                        fabs(v_f - v_12) / a;
      output->DecAcc.VelocityPoints[0] = v_0;
      output->DecAcc.VelocityPoints[n - 1] = v_f;
  }

  output->DecAcc.NumberOfPoints = n;
  output->DecAcc.Distance = dx;
  output->DecAcc.Acceleration = a;
  output->DecAcc.MoveType = move;
  output->TimeDifference = 
        output->DecAcc.TimePoints[output->DecAcc.NumberOfPoints - 1] - 
        output->AccDec.TimePoints[output->AccDec.NumberOfPoints - 1];

  return 0;
}
