#include"determine.h"


int deter_in(float*answer, int i, StructInput input)						//�ж�һ���Ƿ���������Ƭ�ڲ�//
{
	int result=-1;

	if(input.face1[i][2]!=0)																//���һ����������ƬͶӰ��x��y����//
	{
		result=function_xy(input.face,0,3,6,i,answer);									

		if(result!=0)
			result=function_xy(input.face,0,6,3,i,answer);

		if(result!=0)
			result=function_xy(input.face,3,6,0,i,answer);
	}
	else if(input.face1[i][0]!=0)															//���������������ƬͶӰ��y��z����//
	{
		result=function_yz(input.face,0,3,6,i,answer);

		if(result!=0)
			result=function_yz(input.face,0,6,3,i,answer);

		if(result!=0)
			result=function_yz(input.face,3,6,0,i,answer);
	}
	else																			//���������������ƬͶӰ��x��z����//
	{
		result=function_xz(input.face,0,3,6,i,answer);

		if(result!=0)
			result=function_xz(input.face,0,6,3,i,answer);

		if(result!=0)
			result=function_xz(input.face,3,6,0,i,answer);
	}

	return result;
}
int function_xy(float**face,int a,int b,int c,int i,float*answer)					//��������xy��ͶӰ��ֱ�߷��̣����ж�������������ķֲ�//
{
	int result;
	float value[2]={0};
	float func[3]={0};
	if(face[i][a]!=face[i][b])
	{
		func[0]=(face[i][a+1]-face[i][b+1])/(face[i][a]-face[i][b]);
		func[1]=-1;
		func[2]=face[i][a+1]-func[0]*face[i][a];
	}
	else
	{
	func[0]=1;
	func[1]=0;
	func[2]=-face[i][a];
	}
	value[0]=func[0]*face[i][c]+func[1]*face[i][c+1]+func[2];
	value[1]=func[0]*answer[0]+func[1]*answer[1]+func[2];
	result=(value[0]*value[1]>=0)?1:0;
	return result;
}
int function_yz(float**face,int a,int b,int c,int i,float*answer)					//��������yz��ͶӰ��ֱ�߷��̣����ж�������������ķֲ�//
{
	int result;
	float value[2]={0};
	float func[3]={0};
	if(face[i][a+1]!=face[i][b+1])
	{
		func[0]=(face[i][a+2]-face[i][b+2])/(face[i][a+1]-face[i][b+1]);
		func[1]=-1;
		func[2]=face[i][a+2]-func[0]*face[i][a+1];
	}
	else
	{
		func[0]=1;
		func[1]=0;
		func[2]=-face[i][a+1];
	}
	value[0]=func[0]*face[i][c+1]+func[1]*face[i][c+2]+func[2];
	value[1]=func[0]*answer[1]+func[1]*answer[2]+func[2];
	result=(value[0]*value[1]>=0)?1:0;
	return result;
}
int function_xz(float**face,int a,int b,int c,int i,float*answer)					//��������xz��ͶӰ��ֱ�߷��̣����ж�������������ķֲ�//
{
	int result;
	float value[2]={0};
	float func[3]={0};
	if(face[i][a]!=face[i][b])
	{
		func[0]=(face[i][a+2]-face[i][b+2])/(face[i][a]-face[i][b]);
		func[1]=-1;
		func[2]=face[i][a+2]-func[0]*face[i][a];
	}
	else
	{
		func[0]=1;
		func[1]=0;
		func[2]=-face[i][a];
	}
	value[0]=func[0]*face[i][c]+func[1]*face[i][c+2]+func[2];
	value[1]=func[0]*answer[0]+func[1]*answer[2]+func[2];
	result=(value[0]*value[1]>=0)?1:0;
	return result;
}




int deter_dir(float * answer,int i,StructRay*P_Ray)							//�ж�������λ���Ƿ���ֱ�ߴ���������ͬ//
{
int result=-1;
if(abso(P_Ray->point[0]-answer[0])>0.00001)											
{
	if(P_Ray->light[4]<PI/2*3&&P_Ray->light[4]>PI/2)												//��������x��
	{
		if(P_Ray->point[0]>answer[0])
			result=1;
		else
			result=0;
	}	
	else																			//��������x��
	{
		if(P_Ray->point[0]<answer[0])
			result=1;
		else
			result=0;
	}
}
else if(abso(P_Ray->point[2]-answer[2])>0.00001)
{
	if(P_Ray->light[3]>PI/2)																//��������y��
	{
		if(P_Ray->point[2]>answer[2])
			result=1;
		else
			result=0;
	}
	else
	{
		if(P_Ray->point[2]<answer[2])
			result=1;
		else
			result=0;
	}
}
else if(abso(P_Ray->point[1]-answer[1])>0.00001)
{
	if(P_Ray->light[4]>PI)																	//��������y��
	{
		if(P_Ray->point[1]>answer[1])
			result=1;
		else
			result=0;
	}
	else
	{
		if(P_Ray->point[1]<answer[1])
			result=1;
		else
			result=0;
	}
}
else
	P_Ray->decide[1]=0;

return result;
}


