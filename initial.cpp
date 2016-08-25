#include"initial.h"
using namespace std;



void initia_f(StructInput*p_input, StructResult Result)					//三角面片初始化，生成face1//
{
	float x[6]={0};
	float a,b,c,d,e,f,total;
	p_input->face1 = (float**)calloc(p_input->num_f,sizeof(float*));						//声明p_input->face1数组变量//
	for(int i=0;i<p_input->num_f;i++)
	{
		p_input->face1[i] = (float*)calloc(6,sizeof(float));
	}
	for(int i=0;i<p_input->num_f;i++)
	{
		x[0]=p_input->face[i][3]-p_input->face[i][0];											//计算向量式//
		x[1]=p_input->face[i][4]-p_input->face[i][1];
		x[2]=p_input->face[i][5]-p_input->face[i][2];
		x[3]=p_input->face[i][6]-p_input->face[i][0];
		x[4]=p_input->face[i][7]-p_input->face[i][1];
		x[5]=p_input->face[i][8]-p_input->face[i][2];
		a=x[1]*x[5]-x[2]*x[4];												//计算面法向量//
		b=x[2]*x[3]-x[0]*x[5];
		c=x[0]*x[4]-x[1]*x[3];
		total=sqrt(a*a+b*b+c*c);											//面法向量归一化//
		a=a/total;
		b=b/total;
		c=c/total;
		d=p_input->face[i][0]*a+p_input->face[i][1]*b+p_input->face[i][2]*c;
		e=atan(sqrt(a*a+b*b)/c);											//计算p_input->face1表达//
		if(e==0&&c<0)
			e=PI;
		if(e<0)
			e=e+PI;
		if(a>0)
			if(b>0)
				f=atan(b/a);
			else
				f=atan(b/a)+PI*2;
		else if(a<0)
				f=atan(b/a)+PI;
		else
			if(b>0)
				f=PI/2;
			else if(b<0)
				f=PI*3/2;
			else
				f=0;
		p_input->face1[i][0]=a;
		p_input->face1[i][1]=b;
		p_input->face1[i][2]=c;
		p_input->face1[i][3]=d;
		p_input->face1[i][4]=e;
		p_input->face1[i][5]=f;

				//正向//
	}
}



void initia_p(StructInput *p_input, StructBox Box)		//生成光源面//
{
	float max_x=Box.BoxX[Box.BoxStruct[0]];
	float max_y=Box.BoxY[Box.BoxStruct[1]];
	float min_x=Box.BoxX[0];
	float min_y=Box.BoxY[0];
	p_input->height=Box.BoxZ[Box.BoxStruct[2]];
	p_input->plane[0]=min_x+0.00001;
	p_input->plane[1]=min_y+0.00001;
	p_input->plane[2]=p_input->height;
	p_input->plane[3]=max_x-0.00001;
	p_input->plane[4]=max_y-0.00001;
	p_input->plane[5]=p_input->height;	
}




void datainput(StructInput *p_input, StructRay *P_Ray, char **argv)
{
	//cout<<"input the number of photons:";							/*键入光子数*/
	//cin>>p_input->photonum;

	//cout<<"input the reflectance of faces:";						/*键入反射率*/
	//cin>>p_input->rft;

	//cout<<"input the incident angle:";								/*键入反射率*/
	//cin>>p_input->light_ang[0];

	//cout<<"input the direction angle:";								/*键入反射率*/
	//cin>>p_input->light_ang[1];


	FILE *fp;
	fp=fopen(argv[1],"r");
	fscanf(fp,"%d",&p_input->num_f);
	p_input->face = (float**)calloc(p_input->num_f,sizeof(float*));               /*声明face数组变量*/
	for(int i=0;i<p_input->num_f;i++)
	{
		p_input->face[i] = (float*)calloc(9,sizeof(float));
	} 
	for(int i=0;i<p_input->num_f;i++)
	{
		for(int j=0;j<9;j++)
		{
			fscanf(fp,"%f",&p_input->face[i][j]);
		}
	}

	FILE *fp1;
	fp1 = fopen(argv[2],"r");
	fscanf(fp1,"%d",&p_input->photonum);
	fscanf(fp1,"%f",&p_input->light_ang[0]);
	fscanf(fp1,"%f",&p_input->light_ang[1]);
	fscanf(fp1,"%f",&p_input->Energymax);

	p_input->light_ang[0]=PI-p_input->light_ang[0]/180*PI;
	p_input->light_ang[1]=p_input->light_ang[1]/180*PI;



	FILE *fp2;
	fp2 = fopen(argv[3],"r");
	fscanf(fp2,"%d",&p_input->ViewDirectionNum);
	p_input->ViewAngle = (float**)calloc(2,sizeof(float*));
	for(int i=0; i<2; i++)
	{
		p_input->ViewAngle[i] = (float*)calloc(p_input->ViewDirectionNum,sizeof(float));
	}
	for(int i=0; i<p_input->ViewDirectionNum; i++)
	{
		fscanf(fp2,"%f",&p_input->ViewAngle[0][i]);
		fscanf(fp2,"%f",&p_input->ViewAngle[1][i]);
	}
	
	p_input->LightCollect = (float**)calloc(p_input->ViewDirectionNum , sizeof(float*));
	for(int i=0; i < p_input->ViewDirectionNum; i++)
		p_input->LightCollect[i] = (float*)calloc(3,sizeof(float));


	FILE *fp3;
	fp3 = fopen(argv[4],"r");
	fscanf(fp3,"%d",&p_input->SpectralNum);
	p_input->rft = (float*)calloc(p_input->SpectralNum,sizeof(float));
	p_input->tms = (float*)calloc(p_input->SpectralNum,sizeof(float));
	p_input->rft_soil = (float*)calloc(p_input->SpectralNum,sizeof(float));
	for(int i=0; i<p_input->ViewDirectionNum; i++)
	{
		fscanf(fp3,"%f",&p_input->rft[i]);
		fscanf(fp3,"%f",&p_input->tms[i]);
		fscanf(fp3,"%f",&p_input->rft_soil[i]);
	}
	P_Ray->Energy = (float*)calloc(p_input->SpectralNum,sizeof(float));
	P_Ray->EnergyNew = (float*)calloc(p_input->SpectralNum,sizeof(float));


}

void initia_ray(StructRay *P_Ray, StructInput input)
{
	P_Ray->num_rft=0;P_Ray->num_tms=0;P_Ray->num_soil=0;
	P_Ray->EverSpreadSingle = false;
	P_Ray->decide[2]=1;P_Ray->decide[1]=1;
	P_Ray->point[0]=(float)random(input.plane[0],input.plane[3]);
	P_Ray->point[1]=(float)random(input.plane[1],input.plane[4]);
	P_Ray->point[2]=input.height;
	cal_light(input,P_Ray);
	P_Ray->decide[5] = 0; 
	P_Ray->Times = 0;
	
	for(int i=0;i<input.SpectralNum;i++)
	{
		P_Ray->EnergyNew[i] = 1;
		P_Ray->Energy[i] = 1;
	}
		P_Ray->FaceScatter = -1;							////??????????
}


StructResult initia_r(StructInput Input)
{
	StructResult result;
	result.result_t = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.result_sl = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.result_ss = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.result_m = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.brf_t = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.brf_sl = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.brf_ss = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));
	result.brf_m = (float**)calloc(Input.ViewDirectionNum,sizeof(float*));

	for(int i=0; i<Input.ViewDirectionNum; i++)
	{	
		result.result_t[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.result_sl[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.result_ss[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.result_m[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.brf_t[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.brf_sl[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.brf_ss[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		result.brf_m[i] = (float*)calloc(Input.SpectralNum,sizeof(float));
		for(int j=0;j<Input.SpectralNum;j++)
		{
			result.result_t[i][j] = 0;
			result.result_sl[i][j] = 0;
			result.result_ss[i][j] = 0;
			result.result_m[i][j] = 0;
			result.brf_t[i][j] = 0;
			result.brf_sl[i][j] = 0;
			result.brf_ss[i][j] = 0;
			result.brf_m[i][j] = 0;
		}
	}

	


	return result;
}

StructInput initia_input()
{
	StructInput input;
	input.photonum= 2000;
	input.num_f=2;
	input.light_ang[0]=40;
	input.light_ang[1]=140;
	input.Energymax = 0.002;
	input.face= NULL;
	input.face1= NULL;
	input.height=2;
	input.num_direct=1;
	return input;
}


StructBox initial_box(StructInput Input, char **argv)
{
	int BoxNum;
	StructBox Box;

	
	
	FILE *fp;
	fp=fopen(argv[5],"r");
	fscanf(fp,"%d",&Box.BoxStruct[0]);
	fscanf(fp,"%d",&Box.BoxStruct[1]);
	fscanf(fp,"%d",&Box.BoxStruct[2]);
	fscanf(fp,"%f",&Box.BoxOrigin[0]);
	fscanf(fp,"%f",&Box.BoxOrigin[1]);
	fscanf(fp,"%f",&Box.BoxOrigin[2]);
	fscanf(fp,"%f",&Box.BoxSpace[0]);
	fscanf(fp,"%f",&Box.BoxSpace[1]);
	fscanf(fp,"%f",&Box.BoxSpace[2]);

	BoxNum=Box.BoxStruct[0]*Box.BoxStruct[1]*Box.BoxStruct[2];
	Box.FaceNum=(int*)calloc(BoxNum,sizeof(int));
	Box.FaceBox=(int **)calloc(BoxNum,sizeof(int*));

	for(int i=0;i<BoxNum;i++)
	{
		fscanf(fp,"%d",&Box.FaceNum[i]);
		Box.FaceBox[i] = (int*)calloc(Box.FaceNum[i],sizeof(int));               /*声明FaceBox数组变量*/

		for(int j=0;j<Box.FaceNum[i];j++)
			fscanf(fp,"%d",&Box.FaceBox[i][j]);
	}

	
	

	Box.BoxX=(float*)calloc((Box.BoxStruct[0]+1),sizeof(float));
	Box.BoxY=(float*)calloc((Box.BoxStruct[1]+1),sizeof(float));
	Box.BoxZ=(float*)calloc((Box.BoxStruct[2]+1),sizeof(float));

	for(int i=0;i<=Box.BoxStruct[0];i++)
	{
		Box.BoxX[i]=Box.BoxOrigin[0]+i*Box.BoxSpace[0];
	}

	for(int j=0;j<=Box.BoxStruct[1];j++)
	{
		Box.BoxY[j]=Box.BoxOrigin[1]+j*Box.BoxSpace[1];
	}

	for(int k=0;k<=Box.BoxStruct[2];k++)
	{
		Box.BoxZ[k]=Box.BoxOrigin[2]+k*Box.BoxSpace[2];
	}

	return Box;
}

void InitialUpOrDown(float **UpOrDown, StructInput* P_Input, StructResult Result)
{
	for(int i=0;i<P_Input->ViewDirectionNum;i++)
	{
		P_Input->LightCollect[i][0] = sin(P_Input->ViewAngle[0][i] / 180 * PI) * cos(P_Input->ViewAngle[1][i] / 180 * PI);
		P_Input->LightCollect[i][1] = sin(P_Input->ViewAngle[0][i] / 180 * PI) * sin(P_Input->ViewAngle[1][i] / 180 * PI);
		P_Input->LightCollect[i][2] = cos(P_Input->ViewAngle[0][i] / 180 * PI);
		P_Input->LightCollect[i][0] = (abs(P_Input->LightCollect[i][0])<0.00001)?0:P_Input->LightCollect[i][0];
		P_Input->LightCollect[i][1] = (abs(P_Input->LightCollect[i][1])<0.00001)?0:P_Input->LightCollect[i][1];
		P_Input->LightCollect[i][2] = (abs(P_Input->LightCollect[i][2])<0.00001)?0:P_Input->LightCollect[i][2];
		for(int j=0;j<P_Input->num_f;j++)
		{
			UpOrDown[i][j] = cal_angle(P_Input->face1[j][0],P_Input->face1[j][1],P_Input->face1[j][2],P_Input->LightCollect[i][0],P_Input->LightCollect[i][1],P_Input->LightCollect[i][2]);
		}
	}
}



