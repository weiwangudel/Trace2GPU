#pragma once

#define DATA_LEN 12

class CLowpassFilter
{
public:
	CLowpassFilter(void);
	virtual ~CLowpassFilter(void);

	double GetFilteredValue(int ix,int iy,int iz);

	virtual void SetValueAt(int i, int length, double value);


	double _x[DATA_LEN];	
	double _y[DATA_LEN];	
	double _z[DATA_LEN];
};
