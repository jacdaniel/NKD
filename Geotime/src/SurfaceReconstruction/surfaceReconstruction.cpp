
#include <utils.h>
#include <poissonSolver.h>
#include <surfaceReconstructionCPU.h>
#include <surfaceReconstruction.h>

typedef struct _SURFACERECONSTRUCTIONPARAM
{
	void* p;
}SURFACERECONSTRUCTIONPARAM;

typedef struct _SURFACERECONSTRUCTION
{
	int processingMode;
	int dataFormat;
	int size[3];
	int solverType;
	int seedX, seedY, seedZ;
	int nbIter;
	void* surfaceInit;
	void* surfaceOut;
	void* normalManager;
	SURFACERECONSTRUCTIONPARAM* param;
}SURFACERECONSTRUCTION;

static int surfaceReconstructionParamCreate(SURFACERECONSTRUCTION* p, SURFACERECONSTRUCTIONPARAM** param)
{
	if (p == nullptr || param == nullptr) return FAIL;
	if (callocSafe((void**)param, 1, sizeof(SURFACERECONSTRUCTIONPARAM)) != CALLOC_SUCCESS) return FAIL;
	SURFACERECONSTRUCTIONPARAM* pparam = *param;
	if (p->processingMode == PROCESSING_MODE_CPU)
	{
		surfaceReconstructionCPUInit((void**)&pparam->p);
		surfaceReconstructionCPUSetDataFormat(pparam->p, p->dataFormat);
		surfaceReconstructionCPUSetSize(pparam->p, p->size);
		surfaceReconstructionCPUSetSolverType(pparam->p, p->solverType);
		surfaceReconstructionCPUSetSurfaceOut(pparam->p, p->surfaceOut);
		surfaceReconstructionCPUSetNbIter(p, p->nbIter);
		surfaceReconstructionCPUSetNormalManager(pparam->p, p->normalManager);
		surfaceReconstructionCPUSetSeed(pparam->p, p->seedX, p->seedY, p->seedZ);		
	}
	
	return SUCCESS;
}


int surfaceReconstructionInit(void** p)
{
	if (p == nullptr) return FAIL;
	if (callocSafe(p, 1, sizeof(SURFACERECONSTRUCTION)) != CALLOC_SUCCESS) return FAIL;
	
	surfaceReconstructionSetDataFormat(*p, FORMAT_FLOAT32);
	surfaceReconstructionSetProcessingMode(*p, PROCESSING_MODE_CPU);
	surfaceReconstructionSetSolverType(*p, POISSON_SOLVER_METHODE_FOURIER);
	surfaceReconstructionSetNbIter(*p, 20);
	return SUCCESS;
}

int surfaceReconstructionRelease(void** p)
{
	return SUCCESS;
}

int surfaceReconstructionSetProcessingMode(void* p, int processingMode)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTION*)p)->processingMode = processingMode;
	return SUCCESS;
}

int surfaceReconstructionSetDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTION*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

int surfaceReconstructionSetSize(void* p, int *size)
{
	if (p == nullptr) return FAIL;
	for (int i=0; i<3; i++)
	((SURFACERECONSTRUCTION*)p)->size[i] = size[i];
	return SUCCESS;
}

int surfaceReconstructionSetSurfaceOut(void* _p, void* surfaceOut)
{
	if (_p == nullptr) return FAIL;
	SURFACERECONSTRUCTION* p = (SURFACERECONSTRUCTION*)_p;
	p->surfaceOut = surfaceOut;
	if (p->param && p->param->p)
	{
		if (p->processingMode == PROCESSING_MODE_CPU) surfaceReconstructionCPUSetSurfaceOut(p->param->p, surfaceOut);
		else if (p->processingMode == PROCESSING_MODE_GPU);
	}
	return SUCCESS;
}

int surfaceReconstructionSetSolverType(void* _p, int solverType)
{
	if (_p == nullptr) return FAIL;
	SURFACERECONSTRUCTION* p = (SURFACERECONSTRUCTION*)_p;
	p->solverType = solverType;
	if (p->param && p->param->p)
	{
		if (p->processingMode == PROCESSING_MODE_CPU) surfaceReconstructionCPUSetSolverType(p->param->p, solverType);
		else if (p->processingMode == PROCESSING_MODE_GPU);
	}
	return SUCCESS;
}

int surfaceReconstructionSetNbIter(void* _p, int nbIter)
{
	if (_p == nullptr) return FAIL;
	SURFACERECONSTRUCTION* p = (SURFACERECONSTRUCTION*)_p;
	p->nbIter = nbIter;
	if (p->param && p->param->p)
	{
		if (p->processingMode == PROCESSING_MODE_CPU) surfaceReconstructionCPUSetNbIter(p->param->p, nbIter);
		else if (p->processingMode == PROCESSING_MODE_GPU);
	}
	return SUCCESS;
}

int surfaceReconstructionSetNormalManager(void* p, void *normalManager)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTION*)p)->normalManager = normalManager;
}

int surfaceReconstructionSetSeed(void* _p, int x0, int y0, int z0)
{
	if (_p == nullptr) return FAIL;
	SURFACERECONSTRUCTION* p = (SURFACERECONSTRUCTION*)_p;
	p->seedX = x0;
	p->seedY = y0;
	p->seedZ = z0;
	if (p->param && p->param->p)
	{
		if (p->processingMode == PROCESSING_MODE_CPU) surfaceReconstructionCPUSetSeed(p->param->p, x0, y0, z0);
		else if (p->processingMode == PROCESSING_MODE_GPU);
	}
	return SUCCESS;
}

int surfaceReconstructionRun(void* _p)
{
	SURFACERECONSTRUCTION *p = (SURFACERECONSTRUCTION*)_p;
	if (p == nullptr) return FAIL;
	if (p->param == nullptr) surfaceReconstructionParamCreate(p, &p->param);
	if (p->processingMode == PROCESSING_MODE_CPU)
	{
		surfaceReconstructionCPURun(p->param->p);
	}

}


