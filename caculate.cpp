#include"calculate.h"



void cal_light(StructInput input,StructRay*P_Ray)				//计算直线向量表示形式，P_Ray->light//
{
	float di,in;
	int abdi,abin;
	in=input.light_ang[0]/(float)PI*2;
	abin=round(in);
	if(abso(abin-in)>0.00001)															//求非典型天顶角情况下，P_Ray->light状况//
	{
		di=input.light_ang[1]/(float)PI*2;
		abdi=round(di);
		if(abso(abdi-di)>0.00001)														//求非典型方位角情况下，P_Ray->light状况//
		{
			P_Ray->light[0]=-sin(input.light_ang[0])*cos(input.light_ang[1]);
			P_Ray->light[1]=-sin(input.light_ang[0])*sin(input.light_ang[1]);
			P_Ray->light[2]=cos(input.light_ang[0]);
			P_Ray->light[3]=input.light_ang[0];
			P_Ray->light[4]=input.light_ang[1]+PI;
			if(P_Ray->light[4]>2*PI)
				P_Ray->light[4]=P_Ray->light[4]-PI*2;
		}
		else
		{
			switch(abdi)																//求典型光源方位角（N,E,W,S）情况下，P_Ray->light状况//
			{
				case 0:																	//光源在x正向位置//
					P_Ray->light[0]=-sin(input.light_ang[0]);
					P_Ray->light[1]=0;
					P_Ray->light[2]=cos(input.light_ang[0]);
					P_Ray->light[3]=input.light_ang[0];
					P_Ray->light[4]=input.light_ang[1]+PI;
					if(P_Ray->light[4]>2*PI)
						P_Ray->light[4]=P_Ray->light[4]-PI*2;
					break;
				case 1:																	//光源在y正向位置//
					P_Ray->light[0]=0;
					P_Ray->light[1]=-sin(input.light_ang[0]);
					P_Ray->light[2]=cos(input.light_ang[0]);
					P_Ray->light[3]=input.light_ang[0];
					P_Ray->light[4]=input.light_ang[1]+PI;
					if(P_Ray->light[4]>2*PI)
						P_Ray->light[4]=P_Ray->light[4]-PI*2;
					break;
				case 2:																	//光源在x逆向位置//
					P_Ray->light[0]=sin(input.light_ang[0]);
					P_Ray->light[1]=0;
					P_Ray->light[2]=cos(input.light_ang[0]);
					P_Ray->light[3]=input.light_ang[0];
					P_Ray->light[4]=input.light_ang[1]+PI;
					if(P_Ray->light[4]>2*PI)
						P_Ray->light[4]=P_Ray->light[4]-PI*2;
					break;
				case 3:																	//光源在y逆向位置//
					P_Ray->light[0]=0;
					P_Ray->light[1]=sin(input.light_ang[0]);
					P_Ray->light[2]=cos(input.light_ang[0]);
					P_Ray->light[3]=input.light_ang[0];
					P_Ray->light[4]=input.light_ang[1]+PI;
					if(P_Ray->light[4]>2*PI)
						P_Ray->light[4]=P_Ray->light[4]-PI*2;
					break;
				default:break;
			}
		}
	}
	else
	{
		switch(abin)																	//求典型光源天顶角（0,90,180）情况下，P_Ray->light状况//
		{
			case 0:
				P_Ray->light[0]=0;
				P_Ray->light[1]=0;
				P_Ray->light[2]=1;
				P_Ray->light[3]=0;
				P_Ray->light[4]=180;
				break;
			case 1:
				P_Ray->light[0]=cos(input.light_ang[0]);
				P_Ray->light[1]=cos(PI/2-input.light_ang[0]);
				P_Ray->light[2]=0;
				P_Ray->light[3]=90;
				P_Ray->light[4]=input.light_ang[1]+PI;
				if(P_Ray->light[4]>2*PI)
					P_Ray->light[4]=P_Ray->light[4]-PI*2;
				break;
			case 2:
				P_Ray->light[0]=0;
				P_Ray->light[1]=0;
				P_Ray->light[2]=-1;
				P_Ray->light[3]=180;
				P_Ray->light[4]=180;
				break;
			default:break;
		}
	}
	for(int k=0;k<5;k++)
		P_Ray->light_new[k] = P_Ray->light[k];
}




float cal_angle(float a1,float b1,float c1,float a2,float b2,float c2)					//计算两直线向量间夹角//
{
	float angle,multi;
	multi=a1*a2+b1*b2+c1*c2;
	if(multi > 1) multi = 1;
	if(multi < -1) multi = -1;
	angle=acos(multi);
	return angle;
}