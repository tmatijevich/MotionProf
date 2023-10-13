/*******************************************************************************
 * File: main.h
 * Created: 2020-03-23
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file main.h is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#ifndef MOTIONPROF_MAIN_H
#define MOTIONPROF_MAIN_H

/* C libraries */
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* Header switch for build environments */
#ifdef OCTAVE_BUILD
  #include "../Octave/octave_motion_prof.h"
#else
  #include "MotionProf.h"
#endif

/* Types */
typedef struct SecondOrderRootsOutputType {
  double r_1;
  double r_2;
} SecondOrderRootsOutputType;

/* Prototypes */
double pow2(double x);
int32_t SecondOrderRoots(double p_2, double p_1, double p_0, 
                        SecondOrderRootsOutputType *output);

#endif /* MOTIONPROF_MAIN_H */
