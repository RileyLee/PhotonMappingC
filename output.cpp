#include"output.h"

void output(StructResult *P_Result, StructInput input, char **argv)
{
	fstream ofile;
	ofile.open(argv[6],ios::out|ios::trunc);
	ofile<<"result of simulation\n";
	ofile<<"photon number:"<<input.photonum<<'\n';
	ofile<<"incident angle:"<<(input.light_ang[0]/PI*180)<<'\n';
	ofile<<"reflectance of faces:"<<input.rft<<'\n';
	ofile<<"Spectral Number"<<input.SpectralNum;
	ofile<<"Viewing Direction Number:"<<input.ViewDirectionNum;
	ofile.close();

	cal_result(P_Result,input);


	fstream ofile1;
	ofile1.open(argv[7],ios::out|ios::trunc);

	for(int i=0;i<input.ViewDirectionNum;i++)
	{
		for(int j=0;j<input.SpectralNum;j++)
		{
			ofile1<<setiosflags(ios::fixed)<<setprecision(5)<<setfill('0')<<P_Result->brf_t[i][j]<<'\t';
		}
		ofile1<<'\n';
	}

	ofile1.close();

	fstream ofile2;
	ofile2.open(argv[8],ios::out|ios::trunc);

	for(int i=0;i<input.ViewDirectionNum;i++)
	{
		for(int j=0;j<input.SpectralNum;j++)
		{
			ofile2<<setiosflags(ios::fixed)<<setprecision(5)<<setfill('0')<<P_Result->brf_sl[i][j]<<'\t';
		}
		ofile2<<'\n';
	}

	ofile2.close();

	fstream ofile3;
	ofile3.open(argv[9],ios::out|ios::trunc);

	for(int i=0;i<input.ViewDirectionNum;i++)
	{
		for(int j=0;j<input.SpectralNum;j++)
		{
			ofile3<<setiosflags(ios::fixed)<<setprecision(5)<<setfill('0')<<P_Result->brf_ss[i][j]<<'\t';
		}
		ofile3<<'\n';
	}

	ofile3.close();
}