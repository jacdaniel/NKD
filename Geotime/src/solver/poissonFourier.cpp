
#include <fftw3.h>

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>
#include <poissonFourierCPU.h>
#include <poissonFourier.h>

typedef struct _POISSONFOURIERPARAM
{
	int x;
	void *p;
}POISSONFOURIERPARAM;

typedef struct _PoissonFourier
{
	int processingMode;
	int dataFormat;
	int size[3];
	int nbThreads;
	void *dataIn;
	void *dataOut;
	POISSONFOURIERPARAM *param;

}POISSONFOURIER;


int poissonFourierInit(void **p)
{
	if ( p == nullptr ) return FAIL;
	if ( callocSafe(p, 1, sizeof(POISSONFOURIER)) != CALLOC_SUCCESS ) return FAIL;

	return SUCCESS;
}


int poissonFourierRelease(void **_p)
{
	return SUCCESS;
}

int poissonFourierSetProcessingMode(void *p, int processingMode)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIER*)p)->processingMode = processingMode;
	return SUCCESS;
}

int poissonFourierSetDataFormat(void *p, int dataFormat)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIER*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

int poissonFourierSetSize(void *p, int *size)
{
	if ( p == nullptr ) return FAIL;
	for (int i=0; i<3; i++)
		((POISSONFOURIER*)p)->size[i] = size[i];
	return SUCCESS;
}

int poissonFourierSetDataIn(void *p, void *dataIn)
{
	if(p == nullptr ) return FAIL;
	((POISSONFOURIER*)p)->dataIn = dataIn;
	return SUCCESS;
}

int poissonFourierSetDataOut(void *p, void *dataOut)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIER*)p)->dataOut = dataOut;
	return SUCCESS;
}

int poissonFourierSetNbreThreads(void *p, int nbThreads)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIER*)p)->nbThreads = nbThreads;
	return SUCCESS;
}


int poissonFourierParamCPUInit(POISSONFOURIER*p,  POISSONFOURIERPARAM *param)
{
	poissonFourierCPUInit(&param->p);
	poissonFourierCPUSetDataFormat(param->p, p->dataFormat);
	poissonFourierCPUSetSize(param->p, p->size);
	poissonFourierCPUSetDataIn(param->p, p->dataIn);
	poissonFourierCPUSetDataOut(param->p, p->dataOut);
	poissonFourierCPUSetNbreThreads(param->p, p->nbThreads);
	return SUCCESS;
}

int poissonFourierParamInit(POISSONFOURIER *p, POISSONFOURIERPARAM **param)
{
	if( p == nullptr && param == nullptr ) return FAIL;
	callocSafe((void**)param, 1, sizeof(POISSONFOURIERPARAM));
	if ( p->processingMode == PROCESSING_MODE_CPU )
	{
		poissonFourierParamCPUInit(p, *param);
		return SUCCESS;
	}
	else if ( p->processingMode == PROCESSING_MODE_GPU )
	{

		return SUCCESS;
	}
	return FAIL;
}

int poissonFourierRun(void *_p)
{
	if ( _p == nullptr ) return FAIL;
	POISSONFOURIER *p = (POISSONFOURIER*)_p;
	if ( p->param == nullptr )
	{
		poissonFourierParamInit(p, &p->param);
	}

	if (p->processingMode == PROCESSING_MODE_CPU)
	{
		poissonFourierCPURun(p->param->p);
	}

	return SUCCESS;
}
