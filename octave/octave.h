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

typedef enum PathPlanMoveEnum {
  PATHPLAN_MOVE_NONE = 0,
  PATHPLAN_MOVE_DEC_ACC = 1,
  PATHPLAN_MOVE_DEC_ACC_SATURATED = 2,
  PATHPLAN_MOVE_ACC_DEC = 3,
  PATHPLAN_MOVE_ACC_DEC_SATURATED = 4,
  PATHPLAN_MOVE_DEC_DEC = 10,
  PATHPLAN_MOVE_ACC_ACC = 11
} PathPlanMoveEnum;

typedef struct PathPlanRootsOutputType {
  double r_1;
  double r_2;
} PathPlanRootsOutputType;

typedef struct PathPlanBaseOutputType {
  double t_[4];
  double dx;
  double v_[4];
  double a;
  enum PathPlanMoveEnum move;
} PathPlanBaseOutputType;

int32_t PathPlanRoots(double p_2, double p_1, double p_0,
                      PathPlanRootsOutputType *output);
int32_t PathPlanAcc(double dt, double dx, double v_0, double v_f, double v_min,
                    double v_max, PathPlanBaseOutputType *output);

#ifdef __cplusplus
};
#endif
