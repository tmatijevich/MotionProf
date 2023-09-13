/*******************************************************************************
 * File: OctaveMotionProfPoint.cc
 * Created: 2023-07-02
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file OctaveMotionProfPoint.cc is part of the MotionProf project 
 *  released under the GNU General Public License v3.0 agreement.  
 *  For more information, 
 *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "octave_main.h"

DEFUN_DLD(OctaveMotionProfPoint, args, nargout, "TODO: Help string") {

  if (!CheckArguments(args, 6))
    print_usage();

  printf("Success");

  octave_value_list return_values(nargout);

  for (int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());

  return return_values;

}
