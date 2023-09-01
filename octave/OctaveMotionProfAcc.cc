/*******************************************************************************
 * File: OctaveMotionProfAcc.cc
 * Created: 2023-07-02
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file OctaveMotionProfAcc.cc is part of the MotionProf project released 
 *  under the GNU General Public License v3.0 agreement.  For more information, *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include <octave/oct.h>
#include "octave.h"

bool CheckArguments(const octave_value_list& args, const int count);

DEFUN_DLD(OctaveMotionProfAcc, args, nargout, "TODO: Help string") {

  if(!CheckArguments(args, 6))
    print_usage();

  int32_t status;
  MotionProfBaseOutputType output;
  status = MotionProfAcc(args(0).double_value(), args(1).double_value(),
                       args(2).double_value(), args(3).double_value(),
                       args(4).double_value(), args(5).double_value(),
                       &output);

  printf("MotionProfAcc call: Acc %.3f, Vel %.3f, Move %2d, Status %+10d\n", output.a, 
         output.v_[1], output.move, status);

  octave_map oct_output;
  oct_output.setfield("a", octave_value(output.a));
  oct_output.setfield("dx", octave_value(output.dx));
  Matrix t_(4, 1), v_(4, 1);
  for(int i = 0; i < 4; i++) {
    t_(i) = output.t_[i];
    v_(i) = output.v_[i];
  }
  oct_output.setfield("t_", octave_value(t_));
  oct_output.setfield("v_", octave_value(v_));

  octave_value_list return_values(nargout);

  for(int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());

  if(nargout >= 1)
    return_values(0) = oct_output;
  if(nargout >= 2)
    return_values(1) = octave_value(status);

  return return_values;

}

bool CheckArguments(const octave_value_list& args, const int count) {
  if(args.length() != count)
    return false;

  for(int i = 0; i < count; i++) {
    if(!args(i).is_real_scalar())
      return false;
  }

  return true;
}
