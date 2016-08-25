#ifndef _FUNCTION_H_
#define _FUNCTION_H_


#include<cmath>
#include<stdlib.h>
#include <time.h>
#include "basetype.h"

float abso(float x);
double abso(double x);

float* equation(float** formula, float *answer,int num);
float** exchange(int n1,int n2,int min,float** formula);
float** minus(int n1,int n2,int min, float** formula);

double random(double min,double max);

float** CalRoot(float* formula,float d,float** answer,StructRay *P_Ray);

int RoundInt(float data);

int round(float x);

#endif