# MotionProf [![Made for B&R](https://raw.githubusercontent.com/hilch/BandR-badges/dfd5e264d7d2dd369fd37449605673f779db437d/Made-For-BrAutomation.svg)](https://www.br-automation.com)

MotionProf is an [Automation Studio](https://www.br-automation.com/en-us/products/software/automation-software/automation-studio/) library with functions to derive and generate optimal motion profiles.

![2023-09-15_16 30 09](https://github.com/tmatijevich/MotionProf/assets/33841634/b59d07e6-3757-4a67-861b-89c0b5200d83)

**NOTE**: This is not an official library and is supported by the community.  MotionProf is provided as-is under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0.html) agreement.  Source code, documentation, and issues are managed through [GitHub](https://github.com/tmatijevich/MotionProf).

## Functions

Function | Description
---|---
[MotionProfAcc](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L2) | Minimum acceleration to move in time over a distance
[MotionProfTime](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L16) | Minimum time duration to move with acceleration over a distance
[MotionProfDist](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L30) | Maximum distance from move with acceleration in time
[MotionProfVel](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L44) | Minimum velocity to move with acceleration in time over a distance
[MotionProfTimeDiff](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L59) | Maximum difference in time duration between fastest and slowest profiles
[MotionProfAccTimeDiff](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L73) | Minimum acceleration to achieve moves throughout a time difference
[MotionProfAccTimeDiff](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L87) | Minimum acceleration to achieve moves throughout a time difference starting at standstill
[MotionProfPoint](https://github.com/tmatijevich/MotionProf/blob/main/MotionProf.fun#L101) | Velocity profile point interpolation

## Install

1. Download and extract the archive.
2. Import the MotionProf library to your Automation Studio project.
    1. Logical View
    2. Toolbox
    3. **Existing Library**

## Build

Clone the repository.

```bash
git clone https://github.com/tmatijevich/MotionProf.git
```

Or, add as **submodule** to an existing Automation Studio project repository.

```bash
cd <project_root>
git submodule add https://github.com/tmatijevich/MotionProf.git ./Logical/Libraries/MotionProf
```

### Octave

Several MotionProf functions can be cross-compiled in [GNU Octave](https://octave.org/), a free scientific programming language largely compatible with MATLAB.  Octave is utilized for plotting and testing MotionProf functions.

Launch Octave and run the build script.

```matlab
make
```

Use this example to plot the minimum time function.

```matlab
PlotMotionProfTime(0.25, 0.0, 0.5, 0.0, 2.0, 10.0, 1.2);
```

## Authors

- [tmatijevich](https://github.com/tmatijevich)
