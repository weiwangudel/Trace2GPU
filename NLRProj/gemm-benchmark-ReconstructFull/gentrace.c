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


