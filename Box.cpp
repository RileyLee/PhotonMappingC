#include "Box.h"

extern int num_view;



void PointToBoxA(StructRay*P_Ray, StructBox Box)
{
	P_Ray->PointBox[0]=RoundInt((P_Ray->point[0]-Box.BoxOrigin[0])/Box.BoxSpace[0]);
	P_Ray->PointBox[1]=RoundInt((P_Ray->point[1]-Box.BoxOrigin[1])/Box.BoxSpace[1]);
	P_Ray->PointBox[2]=RoundInt((P_Ray->point[2]-Box.BoxOrigin[2])/Box.BoxSpace[2]);
}


void DeterInBox(StructRay*P_Ray, StructBox Box, StructInput Input, StructResult *P_Result, float**UpOrDown)
{
	float node[3];
	float maxi;

	P_Ray->decide[3]=0;

	if(abso(P_Ray->light[0])>=0.00001&&P_Ray->decide[3]==0)											//排除平行于x轴情况//
	{
		if(IntersectX(P_Ray, Box,node,0)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[0]>0)
			{
				P_Ray->PointBox[0]=P_Ray->PointBox[0]-1;
				P_Ray->decide[3]=1;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{
				P_Ray->PointBox[0]=Box.BoxStruct[0]-1;
				P_Ray->point[0]=Box.BoxX[Box.BoxStruct[0]];
				P_Ray->point[1]=node[1];
				P_Ray->point[2]=node[2];
				P_Ray->decide[3]=2;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}

		}
		
		else if(IntersectX(P_Ray, Box,node,1)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[0]<(Box.BoxStruct[0]-1))
			{
				P_Ray->PointBox[0]=P_Ray->PointBox[0]+1;
				P_Ray->decide[3]=1;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{
				P_Ray->PointBox[0]=0;
				P_Ray->point[0]=Box.BoxX[0];
				P_Ray->point[1]=node[1];
				P_Ray->point[2]=node[2];
				P_Ray->decide[3]=2;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
		}
	}
	if(abso(P_Ray->light[1])>=0.00001&&P_Ray->decide[3]==0)											//排除平行于y轴情况//
	{
		if(IntersectY(P_Ray, Box,node,0)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[1]>0)
			{
				P_Ray->PointBox[1]=P_Ray->PointBox[1]-1;
				P_Ray->decide[3]=1;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{
				P_Ray->PointBox[1]=Box.BoxStruct[1]-1;
				P_Ray->point[0]=node[0];
				P_Ray->point[1]=Box.BoxY[Box.BoxStruct[1]];
				P_Ray->point[2]=node[2];
				P_Ray->decide[3]=2;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}

		}

		else if(IntersectY(P_Ray, Box,node,1)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[1]<(Box.BoxStruct[1]-1))
			{
				P_Ray->PointBox[1]=P_Ray->PointBox[1]+1;
				P_Ray->decide[3]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{
				P_Ray->PointBox[1]=0;
				P_Ray->point[0]=node[0];
				P_Ray->point[1]=Box.BoxY[0];
				P_Ray->point[2]=node[2];
				P_Ray->decide[3]=2;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
		}
	}
	if(abso(P_Ray->light[2])>=0.00001&&P_Ray->decide[3]==0)											//排除平行于z轴情况//
	{
		if(IntersectZ(P_Ray, Box,node,0)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[2]>0)
			{
				P_Ray->PointBox[2]=P_Ray->PointBox[2]-1;
				P_Ray->decide[3]=1;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{		
				maxi = 0;
				for(int i=0;i<Input.SpectralNum;i++)
				{
					P_Ray->Energy[i] = P_Ray->Energy[i] * Input.rft_soil[i];
					maxi = (P_Ray->Energy[i] > maxi) ? P_Ray->Energy[i] : maxi;
				}
				if(maxi > Input.Energymax)
				{
					P_Ray->point[0]=node[0];
					P_Ray->point[1]=node[1];
					P_Ray->point[2]=Box.BoxZ[0];
					P_Ray->point_p[0]=P_Ray->point[0];
					P_Ray->point_p[1]=P_Ray->point[1];
					P_Ray->point_p[2]=P_Ray->point[2];
					P_Ray->light[3]=asin(sqrt(random(0,1)));
					P_Ray->light[4]=random(0,PI*2);
					P_Ray->light[0]=cos(P_Ray->light[4])*sin(P_Ray->light[3]);											//计算反射面倾角向量//
					P_Ray->light[0]=(abs(P_Ray->light[0])<0.00001)?0:P_Ray->light[0];
					P_Ray->light[1]=sin(P_Ray->light[3])*sin(P_Ray->light[4]);
					P_Ray->light[1]=(abs(P_Ray->light[1])<0.00001)?0:P_Ray->light[1];
					P_Ray->light[2]=cos(P_Ray->light[3]);
					P_Ray->light[2]=(abs(P_Ray->light[2])<0.00001)?0:P_Ray->light[2];
					P_Ray->decide[3]=2;
					P_Ray->decide[5]=1;
					P_Ray->FaceScatter = -1;
					P_Ray->num_soil = P_Ray->num_soil+1;

					spread(Input, P_Ray, Box, P_Result,  false,UpOrDown);
					


				}
				else
					P_Ray->decide[1]=0;										//被地面吸收//
			}
		}

		else if(IntersectZ(P_Ray, Box,node,1)==1&&P_Ray->decide[3]==0)
		{
			if(P_Ray->PointBox[2]<(Box.BoxStruct[2]-1))
			{
				P_Ray->PointBox[2]=P_Ray->PointBox[2]+1;
				P_Ray->decide[3]=1;
				P_Ray->decide[5]=1;
				P_Ray->Times=P_Ray->Times+1;
			}
			else
			{
				P_Result->DecideCollect = 1;
				P_Ray->decide[1] = 0;
				P_Ray->decide[3]=1;
			}
		}

	}
	if(P_Ray->decide[3]==1)
	{
		P_Ray->point[0]=node[0];
		P_Ray->point[1]=node[1];
		P_Ray->point[2]=node[2];
	}
	if(P_Ray->decide[3]==0&&P_Ray->decide[1]!=0)
		P_Ray->decide[1]=0;
}


int IntersectX(StructRay*P_Ray, StructBox Box,float* node, int datax)
{
	float medium;
	float Ymax,Ymin,Zmax,Zmin;
	int result=0;
	node[0]=Box.BoxX[P_Ray->PointBox[0]+datax];											//反面0，正面1//
	medium=(node[0]-P_Ray->point[0])/P_Ray->light[0];
	node[1]=medium*P_Ray->light[1]+P_Ray->point[1];
	node[2]=medium*P_Ray->light[2]+P_Ray->point[2];
	Ymin=Box.BoxY[P_Ray->PointBox[1]];
	Ymax=Ymin+Box.BoxSpace[1];
	Zmin=Box.BoxZ[P_Ray->PointBox[2]];
	Zmax=Zmin+Box.BoxSpace[2];
	if(node[1]>=Ymin&&node[1]<=Ymax&&node[2]>=Zmin&&node[2]<=Zmax)
		if((node[0]-P_Ray->point[0])*P_Ray->light[0]>0)
			result=1;
	return result;
}


int IntersectY(StructRay*P_Ray, StructBox Box,float* node, int datay)
{
	float medium;
	float Xmax,Xmin,Zmax,Zmin;
	int result=0;
	node[1]=Box.BoxY[P_Ray->PointBox[1]+datay];											//反面0，正面1//
	medium=(node[1]-P_Ray->point[1])/P_Ray->light[1];
	node[0]=medium*P_Ray->light[0]+P_Ray->point[0];
	node[2]=medium*P_Ray->light[2]+P_Ray->point[2];
	Xmin=Box.BoxX[P_Ray->PointBox[0]];
	Xmax=Xmin+Box.BoxSpace[0];
	Zmin=Box.BoxZ[P_Ray->PointBox[2]];
	Zmax=Zmin+Box.BoxSpace[2];
	if(node[0]>=Xmin&&node[0]<=Xmax&&node[2]>=Zmin&&node[2]<=Zmax)
		if((node[1]-P_Ray->point[1])*P_Ray->light[1]>0)
			result=1;
	return result;
}

int IntersectZ(StructRay*P_Ray, StructBox Box,float* node, int dataz)
{
	float medium;
	float Xmax,Xmin,Ymax,Ymin;
	int result=0;
	node[2]=Box.BoxZ[P_Ray->PointBox[2]+dataz];											//反面0，正面1//
	medium=(node[2]-P_Ray->point[2])/P_Ray->light[2];
	node[0]=medium*P_Ray->light[0]+P_Ray->point[0];
	node[1]=medium*P_Ray->light[1]+P_Ray->point[1];
	Xmin=Box.BoxX[P_Ray->PointBox[0]];
	Xmax=Xmin+Box.BoxSpace[0];
	Ymin=Box.BoxY[P_Ray->PointBox[1]];
	Ymax=Ymin+Box.BoxSpace[1];
	if(node[0]>=Xmin&&node[0]<=Xmax&&node[1]>=Ymin&&node[1]<=Ymax)
		if((node[2]-P_Ray->point[2])*P_Ray->light[2]>0)
			result=1;
	return result;
}


void PointToBox(StructRay*P_Ray, StructBox Box)
{
	PointToBoxA(P_Ray, Box);

	P_Ray->decide[3]=0;

	//if(P_Ray->point[0]==Box.BoxX[Box.BoxStruct[0]])
	//	P_Ray->PointBox[0]=2;
	//if(P_Ray->point[1]==Box.BoxY[Box.BoxStruct[1]])
	//	P_Ray->PointBox[1]=2;
	if(P_Ray->point[2]==Box.BoxZ[Box.BoxStruct[2]])													//此种情况下，光线仅为太阳入射光//
	{
		P_Ray->PointBox[2]=Box.BoxStruct[2]-1;
		P_Ray->decide[3]=2;
			//P_Ray->decide[5]=1;
	}
}

