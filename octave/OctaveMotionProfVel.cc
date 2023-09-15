/*******************************************************************************
 * File: OctaveMotionProfVel.cc
 * Created: 2023-09-15
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file OctaveMotionProfVel.cc is part of the MotionProf project 
 *  released under the GNU General Public License v3.0 agreement. 
 *  For more information, please visit 
 *  https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "octave_main.h"

DEFUN_DLD(OctaveMotionProfVel, args, nargout, "TODO: Help string") {

  /* Local variables */
  int32_t status;
  MotionProfBaseOutputType output;
  Matrix t_(4, 1), v_(4, 1);
  octave_map octave_output;
  octave_value_list return_values(nargout);
  
  if (!CheckArguments(args, 7))
    print_usage();

  status = MotionProfVel(args(0).double_value(), args(1).double_value(),
                        args(2).double_value(), args(3).double_value(),
                        args(4).double_value(), args(5).double_value(),
                        args(6).double_value(), &output);

  printf("MotionProfVel call: Vel %.3f, Move %2d, Status %+10d\n", 
        output.v_[1], output.move, status);

  octave_output.setfield("status", octave_value(status));
  octave_output.setfield("a", octave_value(output.a));
  octave_output.setfield("dx", octave_value(output.dx));
  for (int i = 0; i < 4; i++) {
    t_(i) = output.t_[i];
    v_(i) = output.v_[i];
  }
  octave_output.setfield("t_", octave_value(t_));
  octave_output.setfield("v_", octave_value(v_));

  for (int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());

  if (nargout >= 1)
    return_values(0) = octave_output;

  return return_values;
}
