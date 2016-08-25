#include"basetype.h"
#include"type_memory.h"
#include"calculate.h"
#include"determine.h"
#include"box.h"
#include"intersection.h"
#include<cmath>

void spread(StructInput Input, StructRay*P_Ray, StructBox Box, StructResult* P_Result,bool LeafOrSoil, float **UpOrDown);
void IntersectCollect(StructInput Input,StructRay *P_Ray, StructBox Box, StructResult *result);