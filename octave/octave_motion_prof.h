/*******************************************************************************
 * File: octave_motion_prof.h
 * Created: 2023-06-26
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file octave_motion_prof.h is part of the MotionProf project released 
 *  under the GNU General Public License v3.0 agreement.  For more information, 
 *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#ifndef OCTAVE_MOTIONPROF_H
#define OCTAVE_MOTIONPROF_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdint.h>

#define MOTIONPROF_POINT_MAX 20U
#define MOTIONPROF_ERROR_NO_SOLUTION (-4)
#define MOTIONPROF_ERROR_INPUT_POSITIVE (-3)
#define MOTIONPROF_ERROR_INPUT_MOVE (-2)
#define MOTIONPROF_ERROR_INPUT_VELOCITY (-1)
#define MOTIONPROF_ERROR_NULL_POINTER (-5)

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

typedef struct MotionProfPointOutputType {
  double x;
  double v;
  double a;
  double j;
} MotionProfPointOutputType;

typedef struct MotionProfTimeDiffOutputType {
  double dt_tilde;
  struct MotionProfBaseOutputType AccDec;
  struct MotionProfBaseOutputType DecAcc;
} MotionProfTimeDiffOutputType;

int32_t MotionProfAcc(double dt, double dx, double v_0, double v_f, 
                      double v_min, double v_max, 
                      MotionProfBaseOutputType *output);
int32_t MotionProfTime(double dx, double v_0, double v_f, 
                      double v_min, double v_max, double a, 
                      MotionProfBaseOutputType *output);
int32_t MotionProfDist(double dt, double v_0, double v_f,
                      double v_min, double v_max, double a,
                      MotionProfBaseOutputType *output);
int32_t MotionProfVel(double dt, double dx, double v_0, double v_f,
                      double v_min, double v_max, double a,
                      MotionProfBaseOutputType *output);
int32_t MotionProfTimeDiff(double dx, double v_0, double v_f, double v_min,
                          double v_max, double a, 
                          MotionProfTimeDiffOutputType *output);
int32_t MotionProfPoint(double x_0, double t_[], double v_[], uint8_t n, 
                        double t, double k, MotionProfPointOutputType *output);

#ifdef __cplusplus
};
#endif

#endif /* OCTAVE_MOTIONPROF_H */
