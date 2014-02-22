#pragma once
#include "LowpassFilter.h"
#include "TnsMemoryTracer.h" 


class CHighpassFilter : public CLowpassFilter
{
public:
	CHighpassFilter(void);
	
	virtual ~CHighpassFilter(void);


	virtual void SetValueAt(int i, int length, double value);


};


