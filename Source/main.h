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
  #include "PathPlan.h"
#endif

double pow2(double x);
