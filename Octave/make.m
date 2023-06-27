clc
clear all
delete *.oct
mkoctfile octpathplanroots.cc ../Source/roots.c -DOCTAVE_BUILD
