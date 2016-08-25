#ifndef _DETERMINE_H_
#define _DETERMINE_H_

#define PI 3.1415926535898
#include"function.h"
#include<stdlib.h>
#include<iostream>
#include "basetype.h"

int deter_dir(float*answer,int i, StructRay *P_Ray);

int deter_in(float*answer,int i,StructInput input);
int function_xy(float**face,int a,int b,int c,int i,float*answer);
int function_yz(float**face,int a,int b,int c,int i,float*answer);
int function_xz(float**face,int a,int b,int c,int i,float*answer);

#endif
