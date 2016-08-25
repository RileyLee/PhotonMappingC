#ifndef _RAYTRACING_H_

#define _RAYTRACING_H_

#include "basetype.h"
#include "intersection.h"
#include"type_memory.h"
#include "result.h"
#include "Box.h"

extern long num_photon;



void raytracing(StructRay*P_RayOrigin, StructInput Input, StructBox Box, StructResult *result, float **UpOrDown);
void InitialNewRay(StructRay*P_RayOrigin, StructRay *P_Ray, StructInput Input);

#endif