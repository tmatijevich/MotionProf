# MotionProf [![Made for B&R](https://raw.githubusercontent.com/hilch/BandR-badges/dfd5e264d7d2dd369fd37449605673f779db437d/Made-For-BrAutomation.svg)](https://www.br-automation.com)

MotionProf is an Automation Studio library with functions to derive and generate optimal motion profiles.

# Functions

Function | Parameters | Description
---|---|---
MotionProfAcc | `a = MotionProfAcc(dt, dx, v_0, v_f, v_min, v_max)` | Minimum acceleration to move in time over a distance
MotionProfTime | `dt = MotionProfTime(dx, v_0, v_f, v_min, v_max, a)` | Minimum time duration to move with acceleration over a distance
MotionProfDist | `dx = MotionProfDist(dt, v_0, v_f, v_min, v_max, a)` | Maximum distance from move with acceleration in time
MotionProfVel | `v_12 = MotionProfVel(dt, dx, v_0, v_f, v_min, v_max, a)` | Minimum velocity to move with acceleration in time over a distance
MotionProfPoint | `[x, v, a, j] = MotionProfPoint(x_0, t_[], v_[], n, t, k)` | Velocity profile point interpolation

# Octave

This library includes wrappers to build all source functions in the Octave environment.  
[GNU Octave](https://octave.org/) is a free scientific programming language largely compatible with MATLAB.  
This library utilizes Octave for plotting and testing.
