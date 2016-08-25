#include "basetype.h"
#include "calculate.h"
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include"function.h"
#include<stdio.h>
#define PI 3.1415926535898



void initia_f(StructInput *p_input, StructResult Result);
void initia_p(StructInput *p_input, StructBox Box);
void initia_ray(StructRay *P_Ray, StructInput input);
StructBox initial_box(StructInput Input, char **argv);
void datainput(StructInput*p_input, StructRay *P_Ray, char **argv);

StructResult initia_r(StructInput Input);
void InitialUpOrDown(float **UpOrDown, StructInput* P_Input, StructResult Result);