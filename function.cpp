#include"function.h"

float abso(float x)
{
	return x<0?-x:x;
}
double abso(double x)
{
	return x<0?-x:x;
}



float* equation(float** formula, float *answer,int num)								//解三元一次方程组//
{
	if(formula[0][0]==0)															//消去2、3式中变量x//
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

	if(formula[1][1]==0)															//消去3式中变量y//
		formula=exchange(1,2,1,formula);
	formula=minus(1,2,1,formula);

	answer[2]=formula[2][3]/formula[2][2];											//求得z值大小//

	formula=minus(2,1,2,formula);													//转化对角矩阵//
	formula=minus(1,0,1,formula);								
	formula=minus(2,0,2,formula);

	answer[1]=formula[1][3]/formula[1][1];
	answer[0]=formula[0][3]/formula[0][0];
	return answer;
}
float** exchange(int n1,int n2,int min,float** formula)								//对调两子方程//
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
float** minus(int n1,int n2,int min, float** formula)								//两方程相互消去，具体n2-n1//
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




double random(double a,double b)													//生成a与b间随机数//
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



float** CalRoot(float* formula,float d,float**answer,StructRay *P_Ray)			//解一元二次方程，并计算出两组解分别的x、y、z//
{
	float roots[2];
	float y[2],z[2];
	roots[0]=(-formula[1]+sqrt(d))/2/formula[0];										//求解x值//
	roots[1]=(-formula[1]-sqrt(d))/2/formula[0]; 
	y[0]=(-P_Ray->point[0]+roots[0])*P_Ray->light[1]/P_Ray->light[0]+P_Ray->point[1];								//解算y可能值//
	y[1]=(-P_Ray->point[0]+roots[1])*P_Ray->light[1]/P_Ray->light[0]+P_Ray->point[1];								//解算y可能值//
	z[0]=(-P_Ray->point[0]+roots[0])*P_Ray->light[2]/P_Ray->light[0]+P_Ray->point[2];
	z[1]=(-P_Ray->point[0]+roots[1])*P_Ray->light[2]/P_Ray->light[0]+P_Ray->point[2];								//解算z可能值//
	answer[0][0]=roots[0];
	answer[0][1]=y[0];
	answer[0][2]=z[0];
	answer[1][0]=roots[1];
	answer[1][1]=y[1];
	answer[1][2]=z[1];
	return answer;
}



int round(float x)																		//对浮点数四舍五入//
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


