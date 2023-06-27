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

typedef struct PathPlanRootsOutputType {
  double r_1;
  double r_2;
} PathPlanRootsOutputType;

int32_t PathPlanRoots(double p_2, double p_1, double p_0, PathPlanRootsOutputType *output);

#ifdef __cplusplus
};
#endif
