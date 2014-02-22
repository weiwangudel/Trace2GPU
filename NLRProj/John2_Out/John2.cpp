// John2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TnsMemoryTracer.h" 
#include "LowpassFilter.h"
#include "HighpassFilter.h"

int _tmain(int argc, _TCHAR* argv[])
{

	CLowpassFilter filter;

	//parallel
	for (int i=0; i<DATA_LEN; i++)
	{
		TnsWt(__FILE__,__LINE__,filter._x[TnsRd(i)]) =TnsRd(i);
		TnsWt(__FILE__,__LINE__,filter._y[TnsRd(i)]) =TnsRd(filter._x[TnsRd(i)])*2;
		TnsWt(__FILE__,__LINE__,filter._z[TnsRd(i)]) =TnsRd(filter._y[TnsRd(i)])*3;
	}

	int blockLen = 4;

	//parallel
	for (int i=0; i<DATA_LEN; i+=blockLen)
	{
		filter.SetValueAt(i, blockLen,3);
	}

	//non-parallel , data overlap
	for (int i=0; i<DATA_LEN-blockLen/2; i+=blockLen/2)
	{
		filter.SetValueAt(i, blockLen,5);
	}


	CLowpassFilter* pFilter = new CHighpassFilter();
	//parallel 
	for (int i=0; i<DATA_LEN-blockLen/2; i+=blockLen/2)
	{
		pFilter->SetValueAt(i, blockLen,5);
	}

	//non-parallel 
	for (int i=0; i<DATA_LEN-blockLen/2; i+=blockLen/2)
	{
		pFilter->SetValueAt(i, blockLen,5);

		if (i>0)
		{
			TnsWt(__FILE__,__LINE__,pFilter->_x[TnsRd(i)-1]) = TnsRd(pFilter->_x[TnsRd(i)-1]) + TnsRd(pFilter->_x[TnsRd(i)]);
		}
	}


	return 0;
}

