# MotionProf [![Made for B&R](https://raw.githubusercontent.com/hilch/BandR-badges/dfd5e264d7d2dd369fd37449605673f779db437d/Made-For-BrAutomation.svg)](https://www.br-automation.com)

MotionProf is an Automation Studio library with functions to derive and generate optimal motion profiles.

![2023-09-15_16 30 09](https://github.com/tmatijevich/MotionProf/assets/33841634/e68a4a9c-4481-4d25-912a-1510f0f2ae5e)

# Functions

Function | Parameters
---|---
MotionProfAcc | `a = MotionProfAcc(dt, dx, v_0, v_f, v_min, v_max)`
MotionProfTime | `dt = MotionProfTime(dx, v_0, v_f, v_min, v_max, a)`
MotionProfDist | `dx = MotionProfDist(dt, v_0, v_f, v_min, v_max, a)`
MotionProfVel | `v_12 = MotionProfVel(dt, dx, v_0, v_f, v_min, v_max, a)`
MotionProfPoint | `[x, v, a, j] = MotionProfPoint(x_0, t_[], v_[], n, t, k)`

Function | Description
---|---
MotionProfAcc | Minimum acceleration to move in time over a distance
MotionProfTime | Minimum time duration to move with acceleration over a distance
MotionProfDist | Maximum distance from move with acceleration in time
MotionProfVel | Minimum velocity to move with acceleration in time over a distance
MotionProfPoint | Velocity profile point interpolation

# Octave

This library includes wrappers to build all source functions in the Octave environment.  
[GNU Octave](https://octave.org/) is a free scientific programming language largely compatible with MATLAB.  
This library utilizes Octave for plotting and testing.
