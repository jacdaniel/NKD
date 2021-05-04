
#include <string.h>
#include <utils.h>
#include <poissonSolver.h>
#include <normalManager.h>
#include <OpNablaCPU.h>
#include <surfaceReconstructionCPU.h>

typedef struct _SURFACERECONSTRUCTIONCPUPARAM
{
	void** dip;
	void** dtau;
	void** residu;
	void** div;
	void* divT;
	void* divx;
	void* divz;
	void* deltaTau;
	void* poissonSolver;
	void* tau;
	void* taudx;
	void* taudz;
	void* outxy;
	void* outxz;
	void* residuy;
	void* residuz;
	int timeSliceSize[3];
	long timeSliceSize0;
}SURFACERECONSTRUCTIONCPUPARAM;

typedef struct _SURFACERECONSTRUCTIONCPU
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
	SURFACERECONSTRUCTIONCPUPARAM* param;
}SURFACERECONSTRUCTIONCPU;

static int surfaceReconstructionCPUParamCreate(SURFACERECONSTRUCTIONCPU* p, SURFACERECONSTRUCTIONCPUPARAM** param)
{
	if (p == nullptr || param == nullptr) return FAIL;
	if (callocSafe((void**)param, 1, sizeof(SURFACERECONSTRUCTIONCPUPARAM)) != CALLOC_SUCCESS) return FAIL;
	SURFACERECONSTRUCTIONCPUPARAM* pparam = *param;
	long size0 = p->size[0] * p->size[1] * p->size[2];
	long sizeof_ = p->dataFormat == FORMAT_FLOAT32 ? sizeof(float) : sizeof(double);
	pparam->timeSliceSize[0] = p->size[1];
	pparam->timeSliceSize[1] = p->size[2];
	pparam->timeSliceSize[2] = 1;
	pparam->timeSliceSize0 = (long)pparam->timeSliceSize[0] * pparam->timeSliceSize[1];
	int dims = getDimsFromSize(pparam->timeSliceSize);

	callocSafe((void**)&pparam->dip, 2, sizeof(void*));
	callocSafe((void**)&pparam->dtau, 2, sizeof(void*));
	callocSafe((void**)&pparam->residu, 2, sizeof(void*));
	callocSafe((void**)&pparam->div, 2, sizeof(void*));
	for (int i = 0; i < dims; i++)
	{
		callocSafe((void**)&pparam->dip[i], pparam->timeSliceSize0, sizeof_);
		callocSafe((void**)&pparam->dtau[i], pparam->timeSliceSize0, sizeof_);
		callocSafe((void**)&pparam->residu[i], pparam->timeSliceSize0, sizeof_);
		callocSafe((void**)&pparam->div[i], pparam->timeSliceSize0, sizeof_);
	}
	callocSafe((void**)&pparam->divT, pparam->timeSliceSize0, sizeof_);
	callocSafe((void**)&pparam->tau, pparam->timeSliceSize0, sizeof_);
	callocSafe((void**)&pparam->deltaTau, pparam->timeSliceSize0, sizeof_);
	

	poissonSolverInit(&pparam->poissonSolver);
	poissonSolverSetProcessingMode(pparam->poissonSolver, PROCESSING_MODE_CPU);
	poissonSolverSetMethode(pparam->poissonSolver, POISSON_SOLVER_METHODE_FOURIER);
	poissonSolverSetDataFormat(pparam->poissonSolver, p->dataFormat);
	poissonSolverSetSize(pparam->poissonSolver, p->param->timeSliceSize);
	poissonSolverSetDataIn(pparam->poissonSolver, pparam->divT);
	poissonSolverSetDataOut(pparam->poissonSolver, pparam->deltaTau);
	poissonSolverSetNbreThreads(pparam->poissonSolver, 1);

	return SUCCESS;
}


int surfaceReconstructionCPUInit(void** p)
{
	if (p == nullptr) return FAIL;
	if (callocSafe(p, 1, sizeof(SURFACERECONSTRUCTIONCPU)) != CALLOC_SUCCESS) return FAIL;

	surfaceReconstructionCPUSetDataFormat(*p, FORMAT_FLOAT32);
	surfaceReconstructionCPUSetSolverType(*p, POISSON_SOLVER_METHODE_FOURIER);
	surfaceReconstructionCPUSetNbIter(*p, 20);
	return SUCCESS;
}

int surfaceReconstructionCPURelease(void** p)
{
	return SUCCESS;
}

int surfaceReconstructionCPUSetDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTIONCPU*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

int surfaceReconstructionCPUSetSize(void* p, int* size)
{
	if (p == nullptr) return FAIL;
	for (int i = 0; i < 3; i++)
		((SURFACERECONSTRUCTIONCPU*)p)->size[i] = size[i];
	return SUCCESS;
}

int surfaceReconstructionCPUSetSurfaceOut(void* p, void* surfaceOut)
{
	if (p == nullptr) return FAIL;
	for (int i = 0; i < 3; i++)
		((SURFACERECONSTRUCTIONCPU*)p)->surfaceOut = surfaceOut;
	return SUCCESS;
}

int surfaceReconstructionCPUSetSolverType(void* p, int solverType)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTIONCPU*)p)->solverType = solverType;
	return SUCCESS;
}

int surfaceReconstructionCPUSetNbIter(void* p, int nbIter)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTIONCPU*)p)->nbIter = nbIter;
	return SUCCESS;
}

int surfaceReconstructionCPUSetNormalManager(void* p, void* normalManager)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTIONCPU*)p)->normalManager = normalManager;
}

int surfaceReconstructionCPUSetSeed(void* p, int x0, int y0, int z0)
{
	if (p == nullptr) return FAIL;
	((SURFACERECONSTRUCTIONCPU*)p)->seedX = x0;
	((SURFACERECONSTRUCTIONCPU*)p)->seedY = y0;
	((SURFACERECONSTRUCTIONCPU*)p)->seedZ = z0;
}



template <typename T> void updateTau(T* tau, long size, T* deltaTau)
{
	for (long add = 0; add < size; add++)
		tau[add] += deltaTau[add];
}

template <typename T> void centerTau(T* tau, int *size, int x0, int y0, int z0)
{
	long size0 = size[0] * size[1] * size[2];
	T xc = tau[size[0] * z0 + y0];
	for (long add = 0; add < size0; add++)
		tau[add] -= xc;
	for (long add = 0; add < size0; add++)
		tau[add] += x0;
}

template <typename T, typename Tdip> void residuCompute(T* dtau, long size, Tdip* dip, T* residu)
{
	if (sizeof(Tdip) == 2) // int16
		for (long add = 0; add < size; add++)
			residu[add] = dtau[add] - (T)dip[add] / (T)DIP_SHORT_NORM;
	else
		for (long add = 0; add < size; add++)
			residu[add] = dtau[add] - (T)dip[add];
}

template <typename T, typename Tdip> int surfaceReconstructionCPURunX(void* _p)
{
	SURFACERECONSTRUCTIONCPU* p = (SURFACERECONSTRUCTIONCPU*)_p;
	if (p == nullptr) return FAIL;
	if (p->param == nullptr) surfaceReconstructionCPUParamCreate(p, &p->param);
	int cont = 1;
	int iter = 0;

	normalManagerSetDataIn(p->normalManager, p->param->tau);
	normalManagerSetDataOut(p->normalManager, p->param->dip[0], p->param->dip[1]);
	if (p->surfaceInit != nullptr) memcpy(p->param->tau, p->surfaceInit, p->param->timeSliceSize0 * sizeof(T));
	else memset(p->param->tau, 0, p->param->timeSliceSize0 * sizeof(T));
	centerTau<T>((T*)p->param->tau, p->param->timeSliceSize, p->seedX, p->seedY, p->seedZ);
	while (cont)
	{
		normalManagerRun(p->normalManager);
		OpNablaCPU::nablaX((T*)p->param->tau, p->param->timeSliceSize, (T*)p->param->dtau[0]);
		residuCompute<T, Tdip>((T*)p->param->dtau[0], p->param->timeSliceSize0, (Tdip*)p->param->dip[0], (T*)p->param->residu[0]);
		OpNablaCPU::nablatX((T*)p->param->residu[0], p->param->timeSliceSize, (T*)p->param->div[0]);

		memcpy(p->param->divT, p->param->div[0], p->param->timeSliceSize0 * sizeof(T));
		poissonSolverRun(p->param->poissonSolver);

		updateTau<T>((T*)p->param->tau, p->param->timeSliceSize0, (T*)p->param->deltaTau);
		centerTau<T>((T*)p->param->tau, p->param->timeSliceSize, p->seedX, p->seedY, p->seedZ);

		iter++;
		if (iter > p->nbIter) cont = 0;
	}

	memcpy(p->surfaceOut, p->param->tau, p->param->timeSliceSize0 * sizeof(T));
}



int surfaceReconstructionCPURun(void* _p)
{
	SURFACERECONSTRUCTIONCPU* p = (SURFACERECONSTRUCTIONCPU*)_p;
	if (p == nullptr) return FAIL;
	if (p->param == nullptr) surfaceReconstructionCPUParamCreate(p, &p->param);
	if (p->dataFormat == FORMAT_FLOAT32) surfaceReconstructionCPURunX<float, short>(p);
	if (p->dataFormat == FORMAT_FLOAT64) surfaceReconstructionCPURunX<double, short>(p);


}


