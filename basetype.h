#ifndef basetype_h
#define basetype_h
#define NULL 0

typedef struct stct_result															//�������洢�ṹ��//
{
	float**result_t;
	float**result_sl;
	float**result_ss;
	float**result_m;
	float**brf_t;
	float**brf_sl;
	float**brf_ss;
	float**brf_m;
	int DecideCollect;
}StructResult;

typedef struct stct_input
{
	long photonum;
	int num_f;
	float height;
	float Energymax;
	float* rft;
	float *tms;
	float* rft_soil;
	float plane[6];						//��Դ������
	float** face;						//������������
	float** face1;
	float light_ang[2];
	int num_direct;
	int ViewDirectionNum;
	int SpectralNum;
	float** ViewAngle;
	float **LightCollect;															//���������
}StructInput;

typedef struct stct_ray															//�������洢�ṹ��//
{
	float point[3];
	float point_p[3];
	float light[5];
	float light_new[5];
	bool Direction;
	bool EverSpreadSingle;
	float *Energy;
	float *EnergyNew;
	int PointBox[3];
	int decide[6]; 
	int	FaceScatter;															//�ֱ���//
	int num_rft, num_tms, num_soil;
	float angle_f;
	float drt;																	//���ߵ�ǰ����//
	int Times;
}StructRay;


typedef struct stct_box
{
	int BoxStruct[3];
	float BoxSpace[3];
	float *BoxX;
	float *BoxY;
	float *BoxZ;
	float BoxOrigin[3];
	int *FaceNum;
	int **FaceBox;
}StructBox;

#endif