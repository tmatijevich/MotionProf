/*******************************************************************************
 * File: octave_main.h
 * Created: 2023-09-13
 * 
 * Contributors: 
 * - Tyler Matijevich
 * 
 * License:
 *  This file octave_main.h is part of the MotionProf project released under 
 *  the GNU General Public License v3.0 agreement.  For more information, 
 *  please visit https://github.com/tmatijevich/MotionProf/blob/main/LICENSE.
 ******************************************************************************/

#ifndef OCTAVE_MAIN_H
#define OCTAVE_MAIN_H

#include <octave/oct.h>
#include "octave_motion_prof.h"

bool CheckArguments(const octave_value_list& args, const int count);

#endif /* OCTAVE_MAIN_H */
