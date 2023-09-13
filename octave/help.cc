/*******************************************************************************
 * File: help.cc
 * Created: 2023-09-13
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file help.cc is part of the MotionProf project released under 
 *  the GNU General Public License v3.0 agreement.  For more information, 
 *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#include "octave_main.h"

bool CheckArguments(const octave_value_list& args, const int count) {
  if (args.length() != count)
    return false;

  for (int i = 0; i < count; i++) {
    if (!args(i).is_real_scalar())
      return false;
  }

  return true;
}
