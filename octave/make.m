tic
clc
clear all
w = warning("off", "all");
delete *.oct
warning(w)
printf("Build OctMotionProfRoots\n")
mkoctfile OctMotionProfRoots.cc ../Source/roots.c -DOCTAVE_BUILD
printf("Build OctMotionProfAcc\n")
mkoctfile OctMotionProfAcc.cc ../Source/acc.c ../Source/roots.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
toc
