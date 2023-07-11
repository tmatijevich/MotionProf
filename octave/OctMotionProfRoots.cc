/*******************************************************************************
* File: OctMotionProfRoots.cc
* Author: Tyler Matijevich
* Date: 2023-06-26
*******************************************************************************/

#include <octave/oct.h>
#include "octave.h"

DEFUN_DLD(OctMotionProfRoots, args, nargout, "TODO: Help string") {
  
  octave_stdout << "Calling OctMotionProfRoots\n";

  bool valid_args = args.length() == 3;
  
  if(valid_args) {
    valid_args = args(0).is_real_scalar() && args(1).is_real_scalar() && 
                 args(2).is_real_scalar();
  }
  
  int32_t status;
  MotionProfRootsOutputType output;
  if(valid_args) {
    status = MotionProfRoots(args(0).double_value(), args(1).double_value(), 
                          args(2).double_value(), &output);

    octave_stdout << "Root 1: " << output.r_1 << ", Root 2: " << output.r_2 
                  << ", Status: " << status << "\n";
  }

  octave_value_list return_values(nargout);
  for(int i = 0; i < nargout; i++)
    return_values(i) = octave_value(Matrix());
  
  if(nargout >= 1 && valid_args) {
    Matrix oct_output(2,1);
    oct_output(0) = output.r_1;
    oct_output(1) = output.r_2;
    return_values(0) = octave_value(oct_output);
  }
  if(nargout >= 2) {
    if(valid_args) 
      return_values(1) = octave_value(status);
    else {
      uint8NDArray oct_status(1);
      oct_status(0) = -1;
      return_values(1) = octave_value(oct_status);
    }
  }
  
  return return_values;
}
