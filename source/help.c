/*******************************************************************************
 * File: help.c
 * Created: 2020-05-13
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file help.c is part of the MotionProf project released under the
 *  GNU General Public License v3.0 agreement.  For more information, please 
 *  visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "main.h"

double pow2(double x) {
  return pow(x, 2.0);
}

/* Find real roots of second order polynomial (quadratic equation) */
int32_t SecondOrderRoots(double p_2, double p_1, double p_0, 
                        SecondOrderRootsOutputType *output) {
  
  /* Local variables */
  double discriminant = pow2(p_1) - 4.0 * p_2 * p_0;
  
  /* Check pointer */
  if(output == NULL)
    return -1;
  memset(output, 0, sizeof(*output));
  
  /* Check divide by zero */
  if(p_2 != 0.0) {
    /* Two real roots */
    if(discriminant > 0.0) {
      output->r_1 = (-p_1 + sqrt(discriminant)) / (2.0 * p_2);
      output->r_2 = (-p_1 - sqrt(discriminant)) / (2.0 * p_2);
    }
    /* Two imaginary roots */
    else if(discriminant < 0.0)
      return -1;
    /* Root multiple */
    else {
      output->r_1 = -p_1 / (2.0 * p_2);
      output->r_2 = output->r_1;
    }
  }
  else {
    /* Single root */
    if(p_1 != 0.0) {
      output->r_1 = -p_0 / p_1;
      /* This function does not distinguish between a single root and root 
      multiple */
      output->r_2 = output->r_1;
    }
    /* No roots */
    else
      return -1;
  }
  
  return 0;
}
