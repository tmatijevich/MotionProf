tic
clc
clear all
w = warning("off", "all");
delete *.oct
warning(w)
printf("Build OctaveMotionProfAcc\n")
mkoctfile OctaveMotionProfAcc.cc help.cc ../source/acc.c ../source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfTime\n")
mkoctfile OctaveMotionProfTime.cc help.cc ../source/time.c ../source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfDist\n")
mkoctfile OctaveMotionProfDist.cc help.cc ../source/dist.c ../source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfVel\n")
mkoctfile OctaveMotionProfVel.cc help.cc ../source/vel.c ../source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfTimeDiff\n")
mkoctfile OctaveMotionProfTimeDiff.cc help.cc ../source/time_diff.c ../source/time.c ../source/help.c -DOCTAVE_BUILD
printf("Build OctaveMotionProfPoint\n")
mkoctfile OctaveMotionProfPoint.cc help.cc ../source/point.c ../source/help.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
toc
