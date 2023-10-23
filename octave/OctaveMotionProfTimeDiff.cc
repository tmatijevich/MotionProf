/*******************************************************************************
 * File: OctaveMotionProfTimeDiff.cc
 * Created: 2023-10-06
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file OctaveMotionProfTimeDiff.cc is part of the MotionProf project 
 *  released under the GNU General Public License v3.0 agreement. 
 *  For more information, please visit 
 *  https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "octave_main.h"

DEFUN_DLD(OctaveMotionProfTimeDiff, args, nargout, "TODO: Help string") {

  /* Local variables */
  int32_t status;
  MotionProfTimeDiffType output;
  Matrix t_(4, 1), v_(4, 1);
  octave_map octave_acc_dec, octave_dec_acc, octave_output;
  octave_value_list return_values(nargout);

  if (!CheckArguments(args, 6))
    print_usage();

  status = MotionProfTimeDiff(args(0).double_value(), args(1).double_value(),
                              args(2).double_value(), args(3).double_value(),
                              args(4).double_value(), args(5).double_value(),
                              &output);

  printf("MotionProfTimeDiff call: Time Diff %.3f, Status %+10d\n",
          output.TimeDifference, status);

  octave_acc_dec.setfield("dx", octave_value(output.AccDec.Distance));
  for (int i = 0; i <= output.AccDec.NumberOfPoints; i++) {
    t_(i) = output.AccDec.TimePoints[i];
    v_(i) = output.AccDec.VelocityPoints[i];
  }
  octave_acc_dec.setfield("n", octave_value(output.AccDec.NumberOfPoints));
  octave_acc_dec.setfield("t_", octave_value(t_));
  octave_acc_dec.setfield("dx", octave_value(output.AccDec.Distance));
  octave_acc_dec.setfield("v_", octave_value(v_));
  octave_acc_dec.setfield("a", octave_value(output.AccDec.Acceleration));
  octave_acc_dec.setfield("move", octave_value(output.AccDec.MoveType));

  octave_dec_acc.setfield("dx", octave_value(output.DecAcc.Distance));
  for (int i = 0; i <= output.DecAcc.NumberOfPoints; i++) {
    t_(i) = output.DecAcc.TimePoints[i];
    v_(i) = output.DecAcc.VelocityPoints[i];
  }
  octave_dec_acc.setfield("n", octave_value(output.DecAcc.NumberOfPoints));
  octave_dec_acc.setfield("t_", octave_value(t_));
  octave_dec_acc.setfield("dx", octave_value(output.DecAcc.Distance));
  octave_dec_acc.setfield("v_", octave_value(v_));
  octave_dec_acc.setfield("a", octave_value(output.DecAcc.Acceleration));
  octave_dec_acc.setfield("move", octave_value(output.DecAcc.MoveType));

  octave_output.setfield("status", octave_value(status));
  octave_output.setfield("accdec", (Cell)octave_acc_dec);
  octave_output.setfield("decacc", (Cell)octave_dec_acc);

  for (int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());

  if (nargout >= 1)
    return_values(0) = octave_output;

  return return_values;
}
