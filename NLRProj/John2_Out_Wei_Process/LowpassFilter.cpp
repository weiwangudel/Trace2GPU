#include "StdAfx.h"
#include "TnsMemoryTracer.h" 
#include "LowpassFilter.h"

CLowpassFilter::CLowpassFilter(void)
{
	//parallel
	for (int i=0; i<DATA_LEN; i++)
	{
		TnsWt(__FILE__,__LINE__,_x[TnsRd(i)]) =0;
		TnsWt(__FILE__,__LINE__,_y[TnsRd(i)]) =0;
		TnsWt(__FILE__,__LINE__,_z[TnsRd(i)]) =0;
	}

}


CLowpassFilter::~CLowpassFilter(void)
{
}

void CLowpassFilter::SetValueAt(int i, int length, double value)
{
	int sum = 0;

	int end = TnsRd(i)+TnsRd(length);
	for (int i=0; i<end; i++)
	{
		TnsWt(__FILE__,__LINE__,_x[TnsRd(i)]) =TnsRd(value);
		TnsWt(__FILE__,__LINE__,_y[TnsRd(i)]) =TnsRd(value);
		TnsWt(__FILE__,__LINE__,_z[TnsRd(i)]) =TnsRd(value);

		TnsWt(__FILE__,__LINE__,sum) = TnsRd(_x[TnsRd(i)])+TnsRd(_y[TnsRd(i)])+TnsRd(_z[TnsRd(i)]); //for cross iteration test 
	}

}


double CLowpassFilter::GetFilteredValue(int ix,int iy,int iz)
{	
	return TnsRd(_x[TnsRd(ix)]);
}
