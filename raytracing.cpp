#include"raytracing.h"

extern STMemoryPool* mp;


void raytracing(StructRay*P_RayOrigin, StructInput Input, StructBox Box, StructResult *P_Result, float **UpOrDown)
{
	StructRay Ray = *P_RayOrigin;
	StructRay *P_Ray;
	P_Ray=&Ray;

	P_Ray->Energy = new float;
	P_Ray->EnergyNew = new float;
	
	P_Ray->Energy = (float*)mp->Allocate(Input.SpectralNum,sizeof(float));
	P_Ray->EnergyNew = (float*)mp->Allocate(Input.SpectralNum,sizeof(float));

	InitialNewRay(P_RayOrigin, P_Ray, Input);
	
	while(P_Ray->decide[1]!=0)
	{
		if(P_Ray->Times>=60)
			P_Ray->decide[1]=0;
		if(P_Ray->Times==80)
			P_Ray->Times=80;
		if(P_Ray->decide[5]==0)
			PointToBox(P_Ray,Box);
		intersection(P_Ray,Input,Box, P_Result, UpOrDown);
		
		if(P_Ray->decide[1]!=0)
			DeterInBox(P_Ray, Box, Input, P_Result, UpOrDown);
	}
	mp->Dispose(P_Ray->Energy,Input.SpectralNum,sizeof(float*));
	mp->Dispose(P_Ray->EnergyNew,Input.SpectralNum,sizeof(float*));
}


void InitialNewRay(StructRay*P_RayOrigin, StructRay *P_Ray, StructInput Input)
{
	for(int i=0;i<5;i++)
	{
		P_Ray->light[i] = P_RayOrigin->light_new[i];
	}
	P_Ray->decide[2] = 1;
	P_Ray->decide[1] = 1;
	P_Ray->decide[5] = 0;
	P_Ray->Times = 0;
	for(int i=0; i<Input.SpectralNum; i++)
		P_Ray->Energy[i] = P_RayOrigin->EnergyNew[i];
}