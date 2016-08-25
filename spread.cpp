#include "spread.h"

extern STMemoryPool* mp;
extern long num_photon;
extern int num_view;
 
void spread(StructInput Input, StructRay*P_Ray, StructBox Box, StructResult* P_Result, bool LeafOrSoil, float **UpOrDown)
{
	int count;
	
	StructRay RayCollect = *P_Ray;
	StructRay *P_RayCollect;
	P_RayCollect = &RayCollect;

	P_RayCollect->Energy = new float;
	P_RayCollect->EnergyNew = new float;
	
	P_RayCollect->Energy = (float*)mp->Allocate(Input.SpectralNum,sizeof(float));
	P_RayCollect->EnergyNew = (float*)mp->Allocate(Input.SpectralNum,sizeof(float));
	

	for(num_view=0; num_view<Input.ViewDirectionNum; num_view++)
	{
		if(P_Ray->Energy == 0)
			P_Ray->Energy = 0;
		if(LeafOrSoil)
		{
			if(P_Ray->Direction && (UpOrDown[num_view][P_Ray->FaceScatter] < PI/2) || !P_Ray->Direction && !(UpOrDown[num_view][P_Ray->FaceScatter] < PI/2))
			{
				for(int i=0;i<Input.SpectralNum;i++)
					P_RayCollect->Energy[i] = abs(P_Ray->Energy[i] * cos(UpOrDown[num_view][P_Ray->FaceScatter]) * Input.tms[i]);
			}
			else
			{
				for(int i=0;i<Input.SpectralNum;i++)
					P_RayCollect->Energy[i] = abs(P_Ray->Energy[i] * cos(UpOrDown[num_view][P_Ray->FaceScatter]) * Input.rft[i]);
			}
		}
		else
			for(int i=0;i<Input.SpectralNum;i++)
				P_RayCollect->Energy[i] = abs(P_Ray->Energy[i] * cos(Input.ViewAngle[0][num_view] / 180 * PI));

	
		P_RayCollect->light[0] = Input.LightCollect[num_view][0];
		P_RayCollect->light[1] = Input.LightCollect[num_view][1];
		P_RayCollect->light[2] = Input.LightCollect[num_view][2];
		P_RayCollect->light[3] = Input.ViewAngle[0][num_view] / 180 * PI;
		P_RayCollect->light[4] = Input.ViewAngle[1][num_view] / 180 * PI;
		P_RayCollect->point[0] = P_Ray->point[0];
		P_RayCollect->point[1] = P_Ray->point[1];
		P_RayCollect->point[2] = P_Ray->point[2];



		P_Result->DecideCollect = -1;

		PointToBox(P_RayCollect,Box);
		count = 0;
		while(P_Result->DecideCollect == -1)
		{
			
			IntersectCollect(Input, P_RayCollect, Box, P_Result);
			if(P_Result->DecideCollect != 0)
				DeterInBox(P_RayCollect, Box, Input, P_Result, UpOrDown);
			count = count + 1;
			if(count == 100)
				P_Result->DecideCollect = 0;
		}

		if(P_Result->DecideCollect == 1)
		{
			for(int i=0;i<Input.SpectralNum;i++)
				P_Result->result_t[num_view][i] = P_Result->result_t[num_view][i] + P_RayCollect->Energy[i];
			if(P_Ray->num_rft == 0&&P_Ray->num_tms == 0&&P_Ray->num_soil==0)
			{
				for(int i=0;i<Input.SpectralNum;i++)
					P_Result->result_sl[num_view][i] = P_Result->result_sl[num_view][i] + P_RayCollect->Energy[i];
			}
			if(P_Ray->num_rft == 0&&P_Ray->num_tms == 0&&P_Ray->num_soil==1&&P_Ray->EverSpreadSingle == false)
			{
				for(int i=0;i<Input.SpectralNum;i++)
					P_Result->result_ss[num_view][i] = P_Result->result_ss[num_view][i] + P_RayCollect->Energy[i];
				
			}
		}
	}

	P_Ray->EverSpreadSingle = true;
	mp->Dispose(P_RayCollect->Energy,Input.SpectralNum,sizeof(float*));
	mp->Dispose(P_RayCollect->EnergyNew,Input.SpectralNum,sizeof(float*));
}


void IntersectCollect(StructInput Input, StructRay *P_Ray, StructBox Box, StructResult *P_Result)
{
	int NumberBox,NumFace;
	float angle_in;
	float** formula=(float**)mp->Allocate(3,sizeof(float*));
	for(int i=0;i<3;i++)
	{
		formula[i]=(float*)mp->Allocate(4,sizeof(float));									
	}
	float* answer=(float*)mp->Allocate(3,sizeof(float));											//声明结果变量//
	float* distance=(float*)mp->Allocate(5,sizeof(float));																				//while状态初始化//
																		
	for(int i=0;i<3;i++)
		answer[i]=0;

	NumberBox=P_Ray->PointBox[0]+P_Ray->PointBox[1]*Box.BoxStruct[0]+P_Ray->PointBox[2]*Box.BoxStruct[0]*Box.BoxStruct[1];
	for(int i=0;i<Box.FaceNum[NumberBox];i++)																	//逐面求交//
	{
		NumFace=Box.FaceBox[NumberBox][i];
		if( NumFace != P_Ray->FaceScatter)
		{
			angle_in=cal_angle(Input.face1[NumFace][0],Input.face1[ NumFace][1],Input.face1[ NumFace][2],P_Ray->light[0],P_Ray->light[1],P_Ray->light[2]);
			if(abso(angle_in-PI/2)>0.00001)														//判断平行,排除光线与平面平行可能//
			{
				answer=CalNode(Input, P_Ray, formula, answer, NumFace);						//计算交点//
				if(deter_in(answer, NumFace,Input)==1)											//判断是否在三角片内//
				{
					if(deter_dir(answer, NumFace,P_Ray)==1)										//判断相交方向与光线传播方向相同//
					{
						P_Result->DecideCollect=0;
					}
				}
			}
		}
	}

	for(int i=0;i<3;i++) mp->Dispose(formula[i],4,sizeof(float));
	mp->Dispose(formula,3,sizeof(float*));
	mp->Dispose(answer,3,sizeof(float));
	mp->Dispose(distance,5,sizeof(float));

}


