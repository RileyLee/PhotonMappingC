#include<stdlib.h>
#include "function.h"
#include "calculate.h"
#include "determine.h"
#include<math.h>
#include"type_memory.h"
#include<stdlib.h>
#include <time.h>
#include "basetype.h"
#include "box.h"
#include "raytracing.h"



void intersection(StructRay *P_Ray, StructInput Input,StructBox Box, StructResult *result, float **UpOrDown);
int Initial_again(StructInput Input, int NumFace, int again);
float* CalNode(StructInput Input, StructRay*P_Ray, float **formula, float*answer, int NumFace);
void GenerateScatter(StructInput Input,StructRay *P_Ray, StructBox Box, StructResult *result,int NumFaceRFT, float ** UpOrDown);
void GenerateTransmision(StructInput Input,StructRay *P_Ray, StructBox Box, StructResult *result,int NumFaceRFT, float ** UpOrDown);
void LambertTop(StructInput Input,StructRay *P_Ray, int NumFaceRFT);
void LambertBottom(StructInput Input,StructRay *P_Ray, int NumFaceRFT);