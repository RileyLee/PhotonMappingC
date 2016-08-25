#ifndef _BOX_H_
#define _BOX_H_

#include "function.h"
#include "initial.h"
#include "spread.h"


void PointToBoxA(StructRay*P_Ray, StructBox Box);
void DeterInBox(StructRay*P_Ray, StructBox Box, StructInput Input, StructResult *P_Result, float**UpOrDown);
int IntersectX(StructRay*P_Ray, StructBox Box,float* node, int datax);
int IntersectY(StructRay*P_Ray, StructBox Box,float* node, int datay);
int IntersectZ(StructRay*P_Ray, StructBox Box,float* node, int dataz);
void PointToBox(StructRay*P_Ray, StructBox Box);

#endif