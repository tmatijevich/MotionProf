tic
clc
clear all
w = warning("off", "all");
delete *.oct
warning(w)
printf("Build OctPathPlanRoots\n")
mkoctfile octpathplanroots.cc ../Source/roots.c -DOCTAVE_BUILD
printf("Build OctPathPlanAcc\n")
mkoctfile OctPathPlanAcc.cc ../Source/acc.c ../Source/roots.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
toc
