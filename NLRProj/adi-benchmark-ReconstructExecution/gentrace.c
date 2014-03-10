#include <stdio.h>
//TNS MEM
FILE* fid = NULL;

double* TnsMem(double* pData)
{
	fprintf(fid, "Read 0x%X " , (unsigned int)pData);
	return pData;
}

double& TnsMem(double& rData)
{
	fprintf(fid, "Read 0x%X " , (unsigned int)&rData);
	return rData;
}

int& TnsMem(int& rData)
{
	fprintf(fid, "Read 0x%X " , (unsigned int)&rData);
	return rData;
}

double& TnsMemWr(double& rData)
{
	fprintf(fid, "Write 0x%X " , (unsigned int)&rData);
	return rData;
}

double TnsMemC(double rConstant)
{
	fprintf(fid, "Read Constant%f " , rConstant);
	return rConstant;
}

int TnsMemC(int rConstant)
{
	fprintf(fid, "Read Constant%d " , rConstant);
	return rConstant;
}

int TnsMemIter(int i)
{
	fprintf(fid, "Read %d ", i);
	return i;
}


