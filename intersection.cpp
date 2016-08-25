#include "intersection.h"

extern STMemoryPool* mp;
extern long num_photon;
extern int num_view;



void intersection(StructRay *P_Ray, StructInput Input, StructBox Box, StructResult*P_Result, float **UpOrDown)
{
	int again,NumberBox,NumFace;
	float dis,angle_in;
	float** formula=(float**)mp->Allocate(3,sizeof(float*));
	for(int i=0;i<3;i++)
	{
		formula[i]=(float*)mp->Allocate(4,sizeof(float));									
	}
	float* answer=(float*)mp->Allocate(3,sizeof(float));											//�����������//
	float* distance=(float*)mp->Allocate(5,sizeof(float));
	P_Ray->decide[2]=1;																				//while״̬��ʼ��//
	P_Ray->decide[1]=1;
	while(P_Ray->decide[2]>0&&P_Ray->decide[1]!=0)
	{
		P_Ray->decide[2]=0;																			//�������ж�ѭ���ڳ�ʼ��//
		distance[0]=0;
		distance[1]=-1;
		again=0;
		for(int i=0;i<3;i++)
			answer[i]=0;

		NumberBox=P_Ray->PointBox[0]+P_Ray->PointBox[1]*Box.BoxStruct[0]+P_Ray->PointBox[2]*Box.BoxStruct[0]*Box.BoxStruct[1];
		for(int i=0;i<Box.FaceNum[NumberBox];i++)																	//������//
		{
			NumFace=Box.FaceBox[NumberBox][i];
			if( NumFace != P_Ray->FaceScatter)
			{
				angle_in=cal_angle(Input.face1[ NumFace][0],Input.face1[ NumFace][1],Input.face1[ NumFace][2],P_Ray->light[0],P_Ray->light[1],P_Ray->light[2]);
				if(abso(angle_in-PI/2)>0.00001)														//�ж�ƽ��,�ų�������ƽ��ƽ�п���//
				{
					answer=CalNode(Input, P_Ray, formula, answer, NumFace);						//���㽻��//
					if(deter_in(answer, NumFace,Input)==1)											//�ж��Ƿ�������Ƭ��//
					{
						if(deter_dir(answer, NumFace,P_Ray)==1)										//�ж��ཻ��������ߴ���������ͬ//
						{
							if(abso(answer[0]-P_Ray->point[0])>0.00001||abso(answer[1]-P_Ray->point[1])>0.00001||abso(answer[2]-P_Ray->point[2])>0.00001)
							{
								P_Ray->decide[2]=P_Ray->decide[2]+1;													//�������//
								dis=(P_Ray->point[0]-answer[0])*(P_Ray->point[0]-answer[0])+(P_Ray->point[1]-answer[1])*(P_Ray->point[1]-answer[1])+(P_Ray->point[2]-answer[2])*(P_Ray->point[2]-answer[2]);
								if(distance[0]==0&&distance[1]==-1)									//�ж��ҳ��������//
								{
									distance[0]=dis;
									distance[1]=(float) NumFace;
									distance[2]=answer[0];
									distance[3]=answer[1];
									distance[4]=answer[2];
								}
								else if(distance[0]>dis)											//����������Ա�֤distance[0]=0ʱҲ�ܸ�ֵ//
								{
									distance[0]=dis;
									distance[1]=(float) NumFace;
									distance[2]=answer[0];
									distance[3]=answer[1];
									distance[4]=answer[2];
								}
							}
							else
								P_Ray->decide[1]=0;
						}
					}
				}
			}
		}
		if(distance[0]!=0&&distance[1]!=-1)															//��ȡ��̾����ߺ�//
           P_Ray->FaceScatter=(int)distance[1];

		if(P_Ray->decide[2]!= 0)																	//�볡���޽���//
		{
			P_Ray->point_p[0]=P_Ray->point[0];															
			P_Ray->point_p[1]=P_Ray->point[1];
			P_Ray->point_p[2]=P_Ray->point[2];
			P_Ray->point[0]=distance[2];																//��ֵ���¶˵�//
			P_Ray->point[1]=distance[3];
			P_Ray->point[2]=distance[4];

			P_Ray->Direction = cal_angle(Input.face1[P_Ray->FaceScatter][0],Input.face1[P_Ray->FaceScatter][1],Input.face1[P_Ray->FaceScatter][2],P_Ray->light[0],P_Ray->light[1],P_Ray->light[2]) < PI/2;
			
			GenerateScatter(Input, P_Ray, Box, P_Result, P_Ray->FaceScatter, UpOrDown);
			GenerateTransmision(Input, P_Ray, Box, P_Result, P_Ray->FaceScatter, UpOrDown);

			
			spread(Input, P_Ray, Box, P_Result, true,UpOrDown);
			
			
			P_Ray->decide[1]=0;
			
			
		}

	}
	for(int i=0;i<3;i++) mp->Dispose(formula[i],4,sizeof(float));
	mp->Dispose(formula,3,sizeof(float*));
	mp->Dispose(answer,3,sizeof(float));
	mp->Dispose(distance,5,sizeof(float));

}





int Initial_again(StructInput Input, int NumFace, int again)
{
	if( NumFace>0)																					//�ж��Ƿ��ظ����㽻��//
		{
			if(abso(Input.face1[ NumFace-1][0]-Input.face1[ NumFace][0])<0.0001&&abso(Input.face1[ NumFace-1][1]-Input.face1[ NumFace][1])<0.0001&&abso(Input.face1[ NumFace-1][2]-Input.face1[ NumFace][2])<0.0001)																							//�ж���һ����ǰһ���Ƿ�ƽ��//
			{
				if(abso(Input.face1[ NumFace-1][3]-Input.face1[ NumFace][3])<0.0001)										//�ж��ų�ƽ�в��غ����������//
					if(again!=2)
						again=1;
				else
					again=0;
			}
			else if(abso(Input.face1[ NumFace-1][0]+Input.face1[ NumFace][0])<0.0001&&abso(Input.face1[ NumFace-1][1]+Input.face1[ NumFace][1])<0.0001&&abso(Input.face1[ NumFace-1][2]+Input.face1[ NumFace][2])<0.0001)
			{
				if(abso(Input.face1[ NumFace-1][3]-Input.face1[ NumFace][3])<0.0001)
					if(again!=2)
						again=1;
				else
					again=0;
			}
			else
				again=0;
		}
		else
			again=0;
	return again;
}

float* CalNode(StructInput Input, StructRay*P_Ray, float **formula, float*answer, int NumFace)
{
	formula[0][0]=Input.face1[NumFace][0];
	formula[0][1]=Input.face1[NumFace][1];
	formula[0][2]=Input.face1[NumFace][2];
	formula[0][3]=Input.face1[NumFace][3];

	if(abs(P_Ray->light[3]) > 0.00001)
	{
		formula[1][0]=P_Ray->light[1];
		formula[1][1]=-P_Ray->light[0];
		formula[1][2]=0;
		formula[1][3]=P_Ray->light[1]*P_Ray->point[0]-P_Ray->light[0]*P_Ray->point[1];

		if(P_Ray->light[1]!=0)																	//����B=0ʱ�������޽�//
		{
			formula[2][0]=0;
			formula[2][1]=P_Ray->light[2];
			formula[2][2]=-P_Ray->light[1];
			formula[2][3]=P_Ray->light[2]*P_Ray->point[1]-P_Ray->light[1]*P_Ray->point[2];
		}
		else
		{
			formula[2][0]=P_Ray->light[2];
			formula[2][1]=0;
			formula[2][2]=-P_Ray->light[0];
			formula[2][3]=P_Ray->light[2]*P_Ray->point[0]-P_Ray->light[0]*P_Ray->point[2];
		}

		answer=equation(formula,answer, NumFace);
	}
	else
	{
		answer[0]=P_Ray->point[0];
		answer[1]=P_Ray->point[1];
		answer[2]=(formula[0][3] - formula[0][0] * answer[0] - formula[0][1] * answer[1]) / formula[0][2];
	}

	return answer;
}



void GenerateScatter(StructInput Input,StructRay *P_Ray, StructBox Box, StructResult *P_Result, int NumFaceRFT, float ** UpOrDown)
{
	float maxi=0;
	for(int i=0;i<Input.SpectralNum;i++)
	{
		P_Ray->EnergyNew[i] = P_Ray->Energy[i] * Input.rft[i];
		maxi = (P_Ray->EnergyNew[i] > maxi) ? P_Ray->EnergyNew[i] : maxi;
	}
	if(maxi > Input.Energymax)
	{
		P_Ray->num_rft = P_Ray->num_rft+1;
		if(!P_Ray->Direction)			//����//
		{
			LambertTop(Input,P_Ray, NumFaceRFT);
		}
		else																				//����//
		{
			LambertBottom(Input,P_Ray, NumFaceRFT);
		}
		PointToBox(P_Ray, Box);
		if(P_Ray->point[0]==Box.BoxX[0]||P_Ray->point[0]==Box.BoxX[Box.BoxStruct[0]]||P_Ray->point[1]==Box.BoxY[0]||P_Ray->point[1]==Box.BoxY[Box.BoxStruct[1]])
			P_Ray->decide[1]=0;
		raytracing(P_Ray, Input, Box, P_Result, UpOrDown);
		P_Ray->num_rft = P_Ray->num_rft - 1;
	}
	
}


void GenerateTransmision(StructInput Input,StructRay *P_Ray, StructBox Box, StructResult *P_Result, int NumFaceRFT, float ** UpOrDown)
{
	float maxi=0;
	for(int i=0;i<Input.SpectralNum;i++)
	{
		P_Ray->EnergyNew[i] = P_Ray->Energy[i] * Input.tms[i];
		maxi = (P_Ray->EnergyNew[i] > maxi) ? P_Ray->EnergyNew[i] : maxi;
	}
	if(maxi > Input.Energymax)
	{
		P_Ray->num_tms = P_Ray->num_tms + 1;
		if(!P_Ray->Direction)		//����//
		{
			LambertBottom(Input,P_Ray, NumFaceRFT);
		}
		else																				//����//
		{
			LambertTop(Input,P_Ray, NumFaceRFT);
		}
		PointToBox(P_Ray, Box);
		if(P_Ray->point[0]==Box.BoxX[0]||P_Ray->point[0]==Box.BoxX[Box.BoxStruct[0]]||P_Ray->point[1]==Box.BoxY[0]||P_Ray->point[1]==Box.BoxY[Box.BoxStruct[1]])
			P_Ray->decide[1]=0;
		raytracing(P_Ray, Input, Box, P_Result, UpOrDown);
		P_Ray->num_tms = P_Ray->num_tms - 1;
	}
}

void LambertTop(StructInput Input,StructRay *P_Ray, int NumFaceRFT)
{
	float ran_a,ran_b,aa,bb,cc,ca,cb,sa,sb,la,lb,lc;
	ran_a=asin(sqrt(random(0,1)));
	ran_b=random(0,PI*2);
	aa=cos(ran_b)*sin(ran_a);											//���㷴�����������//
	bb=sin(ran_a)*sin(ran_b);
	cc=cos(ran_a);
	ca=cos(Input.face1[NumFaceRFT][4]);
	sa=sin(Input.face1[NumFaceRFT][4]);
	cb=cos(Input.face1[NumFaceRFT][5]);
	sb=sin(Input.face1[NumFaceRFT][5]);
	la=sb*aa+cb*ca*bb+sa*cb*cc;
	lb=-cb*aa+ca*sb*bb+sa*sb*cc;
	lc=-sa*bb+ca*cc;
	if(abso(la)<0.00001)
		la=0;
	if(abso(lb)<0.00001)
		lb=0;
	if(abso(lc)<0.00001)
		lc=0;
	P_Ray->light_new[0]=la;
	P_Ray->light_new[1]=lb;
	P_Ray->light_new[2]=lc;
	P_Ray->light_new[3]=atan(sqrt(la*la+lb*lb)/lc);
	if(P_Ray->light_new[3]<0)
		P_Ray->light_new[3]=P_Ray->light_new[3]+PI;
	if(la>0)
		if(lb>0)
			P_Ray->light_new[4]=atan(lb/la);
		else
			P_Ray->light_new[4]=atan(lb/la)+PI*2;
	else if(la<0)
		P_Ray->light_new[4]=atan(lb/la)+PI;
	else
		if(lb>0)
			P_Ray->light_new[4]=PI/2;
		else
			P_Ray->light_new[4]=PI*3/2;
	
}



void LambertBottom(StructInput Input,StructRay *P_Ray, int NumFaceRFT)
{
	float ran_a,ran_b,aa,bb,cc,ca,cb,sa,sb,la,lb,lc;
	ran_a=asin(sqrt(random(0,1)));
	ran_b=random(0,PI*2);
	aa=cos(ran_b)*sin(ran_a);											//���㷴�����������//
	bb=sin(ran_a)*sin(ran_b);
	cc=cos(ran_a);
	ca=cos(Input.face1[NumFaceRFT][4]);
	sa=sin(Input.face1[NumFaceRFT][4]);
	cb=cos(Input.face1[NumFaceRFT][5]);
	sb=sin(Input.face1[NumFaceRFT][5]);
	la=sb*aa-cb*ca*bb-sa*cb*cc;
	lb=-cb*aa-ca*sb*bb-sa*sb*cc;
	lc=sa*bb-ca*cc;
	if(abso(la)<0.00001)
		la=0;
	if(abso(lb)<0.00001)
		lb=0;
	if(abso(lc)<0.00001)
		lc=0;
	P_Ray->light_new[0]=la;
	P_Ray->light_new[1]=lb;
	P_Ray->light_new[2]=lc;
	P_Ray->light_new[3]=atan(sqrt(la*la+lb*lb)/lc);
	if(P_Ray->light_new[3]<0)
		P_Ray->light_new[3]=P_Ray->light_new[3]+PI;
	if(la>0)
		if(lb>0)
			P_Ray->light_new[4]=atan(lb/la);
		else
			P_Ray->light_new[4]=atan(lb/la)+PI*2;
	else if(la<0)
		P_Ray->light_new[4]=atan(lb/la)+PI;
	else
		if(lb>0)
			P_Ray->light_new[4]=PI/2;
		else
			P_Ray->light_new[4]=PI*3/2;
}