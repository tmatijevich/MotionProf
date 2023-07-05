clc
clear all
delete *.oct
printf("Build OctPathPlanRoots\n")
mkoctfile octpathplanroots.cc ../Source/roots.c -DOCTAVE_BUILD
printf("Build OctPathPlanAcc\n")
mkoctfile OctPathPlanAcc.cc ../Source/acc.c ../Source/roots.c -DOCTAVE_BUILD
printf("Build attempt finished\n")
