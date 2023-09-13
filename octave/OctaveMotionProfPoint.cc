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

  /* Local variables */
  Matrix t_, v_;
  MotionProfPointOutputType output;
  int32_t status;
  octave_map octave_output;
  octave_value_list return_values(nargout);

  if (args.length() != 6)
    print_usage();

  if (!args(0).is_real_scalar() ||
      !args(1).is_real_matrix() || 
      !args(2).is_real_matrix() ||
      !args(3).is_real_scalar() ||
      !args(4).is_real_scalar() ||
      !args(5).is_real_scalar())
    print_usage();

  t_ = args(1).matrix_value();
  v_ = args(2).matrix_value();
  status = MotionProfPoint(args(0).double_value(),
                          (double*)t_.data(),
                          (double*)v_.data(),
                          args(3).double_value(),
                          args(4).double_value(),
                          args(5).double_value(),
                          &output);

  octave_output.setfield("status", octave_value(status));
  octave_output.setfield("x", octave_value(output.x));
  octave_output.setfield("v", octave_value(output.v));
  octave_output.setfield("a", octave_value(output.a));
  octave_output.setfield("j", octave_value(output.j));

  for (int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());

  if (nargout >= 1)
    return_values(0) = octave_output;

  return return_values;

}
