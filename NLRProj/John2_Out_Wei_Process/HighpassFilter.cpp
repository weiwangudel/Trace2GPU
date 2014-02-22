#include "StdAfx.h"
#include "TnsMemoryTracer.h" 
#include "HighpassFilter.h"

CHighpassFilter::CHighpassFilter(void)
{

}
	
CHighpassFilter::~CHighpassFilter(void)
{}

void CHighpassFilter::SetValueAt(int i, int length, double value)
{
	TnsWt(__FILE__,__LINE__,_x[TnsRd(i)]) =TnsRd(value);
	TnsWt(__FILE__,__LINE__,_y[TnsRd(i)]) =TnsRd(value);
	TnsWt(__FILE__,__LINE__,_z[TnsRd(i)]) =TnsRd(value);
	
}
