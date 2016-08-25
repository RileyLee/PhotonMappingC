#include"function.h"

float abso(float x)
{
	return x<0?-x:x;
}
double abso(double x)
{
	return x<0?-x:x;
}



float* equation(float** formula, float *answer,int num)								//����Ԫһ�η�����//
{
	if(formula[0][0]==0)															//��ȥ2��3ʽ�б���x//
	{
		if(formula[1][0]!=0)
		{
			formula=exchange(0,1,0,formula);
		}
		else
		{
			formula=exchange(0,2,0,formula);
		}
	}

	formula=minus(0,1,0,formula);
	formula=minus(0,2,0,formula);

	if(formula[1][1]==0)															//��ȥ3ʽ�б���y//
		formula=exchange(1,2,1,formula);
	formula=minus(1,2,1,formula);

	answer[2]=formula[2][3]/formula[2][2];											//���zֵ��С//

	formula=minus(2,1,2,formula);													//ת���ԽǾ���//
	formula=minus(1,0,1,formula);								
	formula=minus(2,0,2,formula);

	answer[1]=formula[1][3]/formula[1][1];
	answer[0]=formula[0][3]/formula[0][0];
	return answer;
}
float** exchange(int n1,int n2,int min,float** formula)								//�Ե����ӷ���//
{
	float m;
	for(int i=min;i<4;i++)
	{
		m=formula[n1][i];
		formula[n1][i]=formula[n2][i];
		formula[n2][i]=m;
	}
	return formula;
}
float** minus(int n1,int n2,int min, float** formula)								//�������໥��ȥ������n2-n1//
{
	if(formula[n2][min]!=0)
	{
		float k;
		k=formula[n2][min]/formula[n1][min];
		for(int i=min;i<4;i++)
		{
			formula[n2][i]=formula[n2][i]-formula[n1][i]*k;
		}
	}
	return formula;
}




double random(double a,double b)													//����a��b�������//
{
	double max,min;
	if(a>b)
	{
		max=a;
		min=b;
	}
	else
	{
	max=b;
	min=a;
	}
	double ran;
	unsigned long r;

		r = rand();
		r <<= 16;
		r += rand();
		ran=(double)r/2147483648;
		ran=(double)(ran*(max-min)+min);
	return(ran);
}



float** CalRoot(float* formula,float d,float**answer,StructRay *P_Ray)			//��һԪ���η��̣�������������ֱ��x��y��z//
{
	float roots[2];
	float y[2],z[2];
	roots[0]=(-formula[1]+sqrt(d))/2/formula[0];										//���xֵ//
	roots[1]=(-formula[1]-sqrt(d))/2/formula[0]; 
	y[0]=(-P_Ray->point[0]+roots[0])*P_Ray->light[1]/P_Ray->light[0]+P_Ray->point[1];								//����y����ֵ//
	y[1]=(-P_Ray->point[0]+roots[1])*P_Ray->light[1]/P_Ray->light[0]+P_Ray->point[1];								//����y����ֵ//
	z[0]=(-P_Ray->point[0]+roots[0])*P_Ray->light[2]/P_Ray->light[0]+P_Ray->point[2];
	z[1]=(-P_Ray->point[0]+roots[1])*P_Ray->light[2]/P_Ray->light[0]+P_Ray->point[2];								//����z����ֵ//
	answer[0][0]=roots[0];
	answer[0][1]=y[0];
	answer[0][2]=z[0];
	answer[1][0]=roots[1];
	answer[1][1]=y[1];
	answer[1][2]=z[1];
	return answer;
}



int round(float x)																		//�Ը�������������//
{
int in;
int answer;
in=(int)floor(x);
if((x-in)<0.5)
answer=in;
else
answer=in+1;
return(answer);
}

int RoundInt(float data)
{
	int result;
	result=(data>=0)?((int)(data)):((int)(data)-1);
	result=(result==data-1&&data<0)?(int)data:result;
	return result;
}


