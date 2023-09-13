tic
clc
clear all
w = warning("off", "all");
delete *.oct
warning(w)
printf("Build OctaveMotionProfAcc\n")
mkoctfile OctaveMotionProfAcc.cc help.cc ../Source/acc.c ../Source/help.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
toc
