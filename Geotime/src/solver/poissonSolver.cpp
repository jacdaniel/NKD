
#include <malloc.h>

#include <utils.h>

#include <poissonFourier.h>
#include <poissonSolver.h>

typedef struct _POISSON_SOLVER_PARAM
{
	int x;
	void *p;
}POISSON_SOLVER_PARAM;

typedef struct _POISSON_SOLVER
{
	int methode;
	int processingMode;
	int dataFormat;
	int size[3];
	int nbThreads;
	void *dataIn;
	void*dataOut;
	POISSON_SOLVER_PARAM *param;
}POISSON_SOLVER;


int poissonSolverInit(void **p)
{
	if ( p == nullptr ) return FAIL;
	if ( callocSafe(p, 1, sizeof(POISSON_SOLVER)) != CALLOC_SUCCESS ) return FAIL;
	poissonSolverSetMethode(*p, POISSON_SOLVER_METHODE_FOURIER);
	poissonSolverSetProcessingMode(*p, PROCESSING_MODE_CPU);
	poissonSolverSetDataFormat(*p, FORMAT_FLOAT32);
	poissonSolverSetNbreThreads(*p, 1);
	return SUCCESS;
}

int poissonSolverRelease(void **p)
{
	if ( p == nullptr || *p == nullptr ) return FAIL;
	FREE(*p)
	return SUCCESS;
}

int poissonSolverSetMethode(void *p, int methode)
{
	if ( p == nullptr ) return FAIL;
	((POISSON_SOLVER*)p)->methode = methode;
	return SUCCESS;
}

int poissonSolverSetProcessingMode(void *p, int processingMode)
{
	if ( p ==nullptr ) return FAIL;
	((POISSON_SOLVER*)p)->processingMode = processingMode;
	return SUCCESS;
}


int poissonSolverSetDataFormat(void *p, int dataFormat)
{
	if( p == nullptr ) return FAIL;
	((POISSON_SOLVER*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

int poissonSolverSetSize(void *p, int *size)
{
	if ( p == nullptr ) return FAIL;
	for(int i=0; i<3; i++)
		((POISSON_SOLVER*)p)->size[i] = size[i];
	return SUCCESS;
}

int poissonSolverSetDataIn(void *p, void *dataIn)
{
	if ( p == nullptr) return FAIL;
	((POISSON_SOLVER*)p)->dataIn = dataIn;
	return SUCCESS;
}

int poissonSolverSetDataOut(void *p, void *dataOut)
{
	if ( p== nullptr) return FAIL;
	((POISSON_SOLVER*)p)->dataOut = dataOut;
	return SUCCESS;
}

int poissonSolverSetNbreThreads(void *p, int nbThreads)
{
	if(p == nullptr) return FAIL;
	((POISSON_SOLVER*)p)->nbThreads = nbThreads;
	return SUCCESS;
}



int poissonSolverParamInitFourier(POISSON_SOLVER *p, POISSON_SOLVER_PARAM *param)
{
	if ( p == nullptr || param == nullptr ) return FAIL;
	poissonFourierInit(&(param->p));
	poissonFourierSetProcessingMode(param->p, p->processingMode);
	poissonFourierSetDataFormat(param->p, p->dataFormat);
	poissonFourierSetSize(param->p, p->size);
	poissonFourierSetDataIn(param->p, p->dataIn);
	poissonFourierSetDataOut(param->p, p->dataOut);
	poissonFourierSetNbreThreads(param->p, p->nbThreads);
	return SUCCESS;
}

int poissonSolverGetWorkingMemory(long *cpuMem, long *gpuMem, int processingMode, int methode, int dataFormat)
{

	return SUCCESS;
}



int poissonSolverParamInit(POISSON_SOLVER *p, POISSON_SOLVER_PARAM **param)
{
	if ( param == nullptr ) return FAIL;
	if ( callocSafe((void**)param, 1, sizeof(POISSON_SOLVER_PARAM)) != CALLOC_SUCCESS ) return FAIL;
	if ( p->methode == POISSON_SOLVER_METHODE_FOURIER )
	{
		poissonSolverParamInitFourier(p, *param);
	}
	else if ( p->methode == POISSON_SOLVER_METHODE_CG )
	{

	}
	/*
	if ( p == nullptr ) return FAIL;
	(*param) = (POISSON_SOLVER_PARAM*)calloc(1, sizeof(POISSON_SOLVER_PARAM));
	if ( p->methode == POISSON_SOLVER_METHODE_FOURIER )
	{
		(*param)->p = poissonFourierInit();
		poissonFourier

	}
	else if ( p->methode == POISSON_SOLVER_METHODE_CG )
	{

	}
	else
	{
		return FAIL;
	}
	*/
	return SUCCESS;
}


int poissonSolverRun(void *_p)
{
	if ( _p == nullptr ) return FAIL;
	POISSON_SOLVER *p = (POISSON_SOLVER*)_p;
	if ( p->param == nullptr )
	{
		poissonSolverParamInit(p, &p->param);
	}

	int ret = SUCCESS;
	if( p->methode == POISSON_SOLVER_METHODE_FOURIER )
	{
		ret = poissonFourierRun(p->param->p);
	}
	else if ( p->methode == POISSON_SOLVER_METHODE_CG )
	{

	}

	return ret;
}


