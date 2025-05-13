#ifndef PNG_H
#define PNG_H

#include "main.h"
#include "openmv.h"
#include"JY61P.h"

#define PNG_WIDTH 320
#define PNG_HEIGHT 240
#define u0 160
#define v0 120
#define dx 1.4
#define f 1.15*1000
#define N_PNG 3
#define S_area 554*0.000001
#define Cl 2.58
#define l 0.224
#define m 0.3

void PNG_Init();
void PNG_TASK();    

#endif
