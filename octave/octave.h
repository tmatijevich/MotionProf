/*******************************************************************************
 * File: octave.h
 * Author: Tyler Matijevich
 * Date: 2023-06-26
*******************************************************************************/

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

typedef struct MotionProfRootsOutputType {
  double r_1;
  double r_2;
} MotionProfRootsOutputType;

typedef struct MotionProfBaseOutputType {
  double t_[4];
  double dx;
  double v_[4];
  double a;
  enum MotionProfMoveEnum move;
} MotionProfBaseOutputType;

int32_t MotionProfRoots(double p_2, double p_1, double p_0,
                      MotionProfRootsOutputType *output);
int32_t MotionProfAcc(double dt, double dx, double v_0, double v_f, double v_min,
                    double v_max, MotionProfBaseOutputType *output);

#ifdef __cplusplus
};
#endif
