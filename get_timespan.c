/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    matijevict 
 * Created:   May 15, 2020/10:01 AM 
 *********************************************************************************/ 
#include "PathKin.h"
#include <math.h>

/* Internal function - return the input squared */
static float sqf(float x) {return pow(x, 2.0);}

plcbit Kin_GetTimespan(float dx, float v0, float vf, float vmin, float vmax, float a, struct KinGetTimespanSoln_typ* soln) {
	return 1;
}
