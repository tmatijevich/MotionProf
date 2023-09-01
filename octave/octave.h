/*******************************************************************************
 * File: octave.h
 * Created: 2023-06-26
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file octave.h is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#ifndef MOTIONPROF_OCTAVE_H
#define MOTIONPROF_OCTAVE_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdint.h>

typedef enum MotionProfMoveEnum {
  MOTIONPROF_MOVE_NONE = 0,
  MOTIONPROF_MOVE_DECACC = 1,
  MOTIONPROF_MOVE_DECACC_SATURATED = 2,
  MOTIONPROF_MOVE_ACCDEC = 3,
  MOTIONPROF_MOVE_ACCDEC_SATURATED = 4,
  MOTIONPROF_MOVE_DECDEC = 10,
  MOTIONPROF_MOVE_ACCACC = 11
} MotionProfMoveEnum;

typedef struct MotionProfBaseOutputType {
  double t_[4];
  double dx;
  double v_[4];
  double a;
  enum MotionProfMoveEnum move;
} MotionProfBaseOutputType;

int32_t MotionProfAcc(double dt, double dx, double v_0, double v_f, 
                      double v_min, double v_max, 
                      MotionProfBaseOutputType *output);

#ifdef __cplusplus
};
#endif

#endif /* MOTIONPROF_OCTAVE_H */
