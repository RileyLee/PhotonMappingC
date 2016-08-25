#ifndef _CALCULATE_H_
#define _CALCULATE_H_

#include<cmath>
#define PI 3.1415926535898
#include "function.h"
#include "basetype.h"

void cal_light(StructInput input,StructRay*P_Ray);
float cal_angle(float a1,float b1,float c1,float a2,float b2,float c2);

#endif