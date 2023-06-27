/*******************************************************************************
 * File: main.h
 * Author: Tyler Matijevich
 * Date: 2020-03-23
*******************************************************************************/

#include <string.h>
#include <stdint.h>
#include <math.h>

#ifdef OCTAVE_BUILD
  #pragma message "Octave build"
  #include "Octave\octave.h"
#else
  #include "PathPlan.h"
  #pragma message "Automation Runtime build"
#endif

double pow2(double x);
