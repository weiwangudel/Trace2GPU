
#pragma once 

class CHighpassFilter;
class CLowpassFilter;


#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <time.h> 

#define TNS_MEM_LOG_FILENAME "TnsMemoryTracer.txt"

class TnsLoop
{
private:
	static int mNextId;

	int mLoopId;
public:
	TnsLoop(const char* fileName, long lineNumber);
	virtual ~TnsLoop(void);
};


class TnsIteration
{
private:
	static int mNextId;

	int mLoopId;
public:
	TnsIteration(const char* fileName, long lineNumber);
	virtual ~TnsIteration(void);
};


class TnsMemoryTracer
{
public:
	static TnsMemoryTracer* GetSingleton();

	TnsMemoryTracer(void);
	virtual ~TnsMemoryTracer(void);

	FILE * fid;

	void Write_MemoryCreate(unsigned int address);
	void Write_MemoryRead(unsigned int address);
	void Write_MemoryWrite(const char* fileName, long lineNumber, unsigned int address);

	void Write(const char* fileName, long lineNumber, const char* codeType);

	char* GetTime();

};




int& TnsCr( int& rData);
int& TnsRd( int& rData);
int& TnsWt( char* fileName, long lineNumber, int& rData);
double& TnsCr( double& rData);
double& TnsRd( double& rData);
double& TnsWt( char* fileName, long lineNumber, double& rData);
CLowpassFilter * TnsCr( CLowpassFilter * pData);
CLowpassFilter * TnsRd( CLowpassFilter * pData);

