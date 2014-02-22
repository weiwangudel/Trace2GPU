
#include "StdAfx.h"
#include "TnsMemoryTracer.h"


int TnsLoop::mNextId = 1;
TnsLoop::TnsLoop(const char* fileName, long lineNumber)
{
	mLoopId = mNextId++;
	FILE* fid = TnsMemoryTracer::GetSingleton()->fid;

	fprintf(fid,"(File@)%s (Line@)%d (LoopId@)%d ", fileName, lineNumber, mLoopId);
	fprintf(fid,"\n");	
	fflush(fid);
	
	printf("(File@)%s (Line@)%d (LoopId@)%d ", fileName, lineNumber, mLoopId);
	printf("\n");
}
TnsLoop::~TnsLoop(void)
{	
	FILE* fid = TnsMemoryTracer::GetSingleton()->fid;

	fprintf(fid,"(LoopEndId@)%d ", mLoopId);
	fprintf(fid,"\n");	
	fflush(fid);
	
	printf("(LoopEndId@)%d ", mLoopId);
	printf("\n");
}


int TnsIteration::mNextId = 1;
TnsIteration::TnsIteration(const char* fileName, long lineNumber)
{
	mLoopId = mNextId++;
	FILE* fid = TnsMemoryTracer::GetSingleton()->fid;

	fprintf(fid,"(File@)%s (Line@)%d (IterationId@)%d ", fileName, lineNumber, mLoopId);
	fprintf(fid,"\n");	
	fflush(fid);
	
	printf("(File@)%s (Line@)%d (IterationId@)%d ", fileName, lineNumber, mLoopId);
	printf("\n");
}
TnsIteration::~TnsIteration(void)
{	
	FILE* fid = TnsMemoryTracer::GetSingleton()->fid;

	fprintf(fid,"(IterationIdEndId@)%d ", mLoopId);
	fprintf(fid,"\n");	
	fflush(fid);
	
	printf("(IterationIdEndId@)%d ", mLoopId);
	printf("\n");
}


TnsMemoryTracer::TnsMemoryTracer(void)
{
	fid = fopen(TNS_MEM_LOG_FILENAME,"w");
}

TnsMemoryTracer::~TnsMemoryTracer(void)
{
	if(fid != NULL){

		fclose(fid);
	}
}


TnsMemoryTracer* TnsMemoryTracer::GetSingleton()
{
	static TnsMemoryTracer aLog;
	return &aLog;
}

char* TnsMemoryTracer::GetTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	static char timeString[256];

	size_t sizeLength = strftime (timeString, 256, "%H:%M:%S", timeinfo );
	return timeString;
}

void TnsMemoryTracer::Write_MemoryCreate(unsigned int address)
{

	//fixed
	fprintf(fid,"(Create@)%X ", address);
	printf("(Create@)%X ", address);

	//fprintf(fid,"\n");
	//printf("\n");

	fflush(fid);

}

void TnsMemoryTracer::Write_MemoryRead(unsigned int address)
{

	//fixed
	fprintf(fid,"(Read@)%X ", address);
	printf("(Read@)%X ", address);

	//fprintf(fid,"\n");
	//printf("\n");

	fflush(fid);

}

void TnsMemoryTracer::Write_MemoryWrite(const char* fileName, long lineNumber, unsigned int address)
{
	//fixed
	fprintf(fid,"(File@)%s (Line@)%d (Write@)%X ", 
	fileName, lineNumber, address);

	printf("(File@)%s (Line@)%d (Write@)%X ", 
	fileName, lineNumber, address);

	fprintf(fid,"\n");
	printf("\n");

	fflush(fid);

//	//fixed
//	_ftprintf(fid,"(Write@)%X "), address);
//	_tprintf("(Write@)%X "), address);
//
//	_ftprintf(fid,"\n"));
//	_tprintf("\n"));
//
//	fflush(fid);
}

void TnsMemoryTracer::Write(const char* fileName, long lineNumber, const char* codeType)
{
	char* timeStamp = TnsMemoryTracer::GetTime();
		
	//fixed
	fprintf(fid,"(File@)%s (Line@)%d (Time@)%s (Codetype@)%s ", 
	fileName, lineNumber, timeStamp, codeType);

	printf("(File@)%s (Line@)%d (Time@)%s (Codetype@)%s ", 
	fileName, lineNumber, timeStamp, codeType);

	fprintf(fid,"\n");
	printf("\n");

	fflush(fid);
}



int& TnsCr( int& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryCreate( (unsigned int) &rData); return rData; }

int& TnsRd( int& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryRead( (unsigned int) &rData); return rData; }

int& TnsWt( char* fileName, long lineNumber, int& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryWrite( fileName, lineNumber, (unsigned int) &rData); return rData; }

double& TnsCr( double& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryCreate( (unsigned int) &rData); return rData; }

double& TnsRd( double& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryRead( (unsigned int) &rData); return rData; }

double& TnsWt( char* fileName, long lineNumber, double& rData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryWrite( fileName, lineNumber, (unsigned int) &rData); return rData; }

CLowpassFilter * TnsCr( CLowpassFilter * pData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryCreate( (unsigned int) pData); return pData; }

CLowpassFilter * TnsRd( CLowpassFilter * pData)
{	TnsMemoryTracer::GetSingleton()->Write_MemoryRead( (unsigned int) pData); return pData; }


