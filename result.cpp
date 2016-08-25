#include "result.h"
#include "type_memory.h"

extern STMemoryPool* mp;

void cal_result(StructResult* P_Result,StructInput Input)
{
	float k=0;
	for(int i=0 ; i<Input.ViewDirectionNum ; i++)
	{
		for(int j=0;j<Input.SpectralNum;j++)
		{
			P_Result->result_m[i][j] = P_Result->result_t[i][j] - P_Result->result_sl[i][j]-P_Result->result_ss[i][j];
			
			k=cos(Input.ViewAngle[0][i]/180*PI);

			P_Result->brf_t[i][j] = abs(P_Result->result_t[i][j] / (double)Input.photonum / k);
			P_Result->brf_sl[i][j] = abs(P_Result->result_sl[i][j] / (double)Input.photonum / k);
			P_Result->brf_ss[i][j] = abs(P_Result->result_ss[i][j] / (double)Input.photonum / k);
			P_Result->brf_m[i][j] = abs(P_Result->result_m[i][j] / (double)Input.photonum / k); 
		}
	}
}



