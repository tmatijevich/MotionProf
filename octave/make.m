tic
clc
clear all
w = warning("off", "all");
delete *.oct
warning(w)
printf("Build OctaveMotionProfAcc\n")
mkoctfile OctaveMotionProfAcc.cc help.cc ../Source/acc.c ../Source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfTime\n")
mkoctfile OctaveMotionProfTime.cc help.cc ../Source/time.c ../Source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfDist\n")
mkoctfile OctaveMotionProfDist.cc help.cc ../Source/dist.c ../Source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfVel\n")
mkoctfile OctaveMotionProfVel.cc help.cc ../Source/vel.c ../Source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfPoint\n")
mkoctfile OctaveMotionProfPoint.cc help.cc ../Source/point.c ../Source/help.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
toc
