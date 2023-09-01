/*******************************************************************************
 * File: main.h
 * Author: Tyler Matijevich
 * Date: 2020-03-23
*******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <math.h>

#ifdef OCTAVE_BUILD
  #include "../Octave/octave.h"
#else
  #include "MotionProf.h"
#endif

typedef struct SecondOrderRootsOutputType {
  double r_1;
  double r_2;
} SecondOrderRootsOutputType;

double pow2(double x);
int32_t SecondOrderRoots(double p_2, double p_1, double p_0, 
                        SecondOrderRootsOutputType *output);
