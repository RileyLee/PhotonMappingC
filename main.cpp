#include "basetype.h"
#include<stdlib.h>
#include<iostream>
#include"initial.h"
#include"box.h"
#include"intersection.h"
#include"result.h"
#include"output.h"
#include<windows.h>
#include "type_memory.h"
#include<time.h>
#include<cmath>
#include"raytracing.h"

STMemoryPool* mp;
int num_view = 0;


void main(int argc, char **argv)
{
	int count = 0;
	mp = new STMemoryPool();

	

	StructInput input;
	StructInput *p_input;
	p_input=&input;

	StructRay ray;
	StructRay *P_Ray;
	P_Ray=&ray;

	StructBox Box = initial_box(input, argv);
	StructBox *P_Box;
	P_Box=&Box;

	srand(time(0));
	datainput(p_input, P_Ray, argv);

	StructResult Result = initia_r(input);
	StructResult *P_Result;
	P_Result = &Result;

	initia_f(p_input, Result);
	initia_p(p_input, Box);
	
	LARGE_INTEGER BeginTime;
	LARGE_INTEGER EndTime;
	LARGE_INTEGER Freq;

	float **UpOrDown = (float**)mp->Allocate(input.ViewDirectionNum,sizeof(float*));
	for (int i=0;i<input.ViewDirectionNum;i++)	UpOrDown[i] = (float*)mp->Allocate(input.num_f,sizeof(float));
	InitialUpOrDown(UpOrDown, p_input, Result);

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&BeginTime);
	

	for(int num_photon=0; num_photon<input.photonum; num_photon++)
	{
		initia_ray(P_Ray, input);

		raytracing(P_Ray, input, Box, P_Result, UpOrDown);
		if(count != (int)(num_photon/100))
		{
			count = (int)(num_photon/100);
			cout<<"PART "<<count<<"                                 COMPLETE\n";
		}
	}
	
	output(P_Result, input, argv); 

	QueryPerformanceCounter(&EndTime);
	cout<<"\nTime:"<<(EndTime.QuadPart-BeginTime.QuadPart)/Freq.QuadPart*1000<<"ms"<<endl;
	system("pause");


	mp->Dispose(UpOrDown, input.num_f, sizeof(float));
	mp->Dispose(UpOrDown, input.ViewDirectionNum, sizeof(float*));


}