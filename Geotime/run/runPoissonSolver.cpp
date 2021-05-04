
#include <stdio.h>
#include <utils.h>
#include <utilDisplay.h>
#include <laplacian.h>
#include <poissonSolver.h>
#include <OpNablaCPU.h>
#include <runPoissonSolver.h>

static int runPoissonSolver1D()
{
	void* p = nullptr;
	int size[] = { 100, 1, 1 };
	void* dataIn = nullptr, * dataOut = nullptr;
	long size0 = size[0] * size[1] * size[2];

	callocSafe(&dataIn, size0, sizeof(double));
	callocSafe(&dataOut, size0, sizeof(double));

	for (int i = 0; i < size0; i++)
		((double*)dataIn)[i] = i;

	poissonSolverInit(&p);
	poissonSolverSetProcessingMode(p, PROCESSING_MODE_CPU);
	poissonSolverSetMethode(p, POISSON_SOLVER_METHODE_FOURIER);
	poissonSolverSetDataFormat(p, FORMAT_FLOAT64);
	poissonSolverSetSize(p, size);
	poissonSolverSetDataIn(p, dataIn);
	poissonSolverSetDataOut(p, dataOut);
	poissonSolverSetNbreThreads(p, 1);

	poissonSolverRun(p);
	UtilDisplay::display((double*)dataOut, 100);

	fprintf(stderr, "poissonFourier1D ok\n");
	return 0;
}



static int runPoissonSolver1D_v2()
{
	void* p = nullptr, *dip, *r, *dr, *div;
	int size[] = { 100, 1, 1 };
	void* dataIn = nullptr, * dataOut = nullptr;
	long size0 = size[0] * size[1] * size[2];

	callocSafe(&dataIn, size0, sizeof(double));
	callocSafe(&dataOut, size0, sizeof(double));

	callocSafe(&dip, size0, sizeof(double));
	callocSafe(&r, size0, sizeof(double));
	callocSafe(&div, size0, sizeof(double));
	callocSafe(&dr, size0, sizeof(double));

	for (int i = 0; i < size0; i++)
		((double*)dip)[i] = 2.0;
	for (int i = 0; i < 100; i++)
		((double*)r)[i] = 0.0 - ((double*)dip)[i];

	OpNablaCPU::nablatX((double*)r, size, (double*)div);

	for (int i = 0; i < size0; i++)
		((double*)dataIn)[i] = ((double*)div)[i];

	poissonSolverInit(&p);
	poissonSolverSetProcessingMode(p, PROCESSING_MODE_CPU);
	poissonSolverSetMethode(p, POISSON_SOLVER_METHODE_FOURIER);
	poissonSolverSetDataFormat(p, FORMAT_FLOAT64);
	poissonSolverSetSize(p, size);
	poissonSolverSetDataIn(p, dataIn);
	poissonSolverSetDataOut(p, dataOut);
	poissonSolverSetNbreThreads(p, 1);

	poissonSolverRun(p);
	UtilDisplay::display((double*)dataOut, 100);

	fprintf(stderr, "poissonFourier1D ok\n");
	return 0;
}

static int runPoissonSolver2D()
{

}


int runPoissonSolver(int argc, char** argv)
{
	runPoissonSolver1D_v2();
	return 0;


	void* p = nullptr;
	int size[] = { 100, 100, 1 };
	void* dataIn = nullptr, * dataOut = nullptr;
	long size0 = size[0] * size[1];

	callocSafe(&dataIn, size0, sizeof(float));
	callocSafe(&dataOut, size0, sizeof(float));


	poissonSolverInit(&p);
	poissonSolverSetProcessingMode(p, PROCESSING_MODE_CPU);
	poissonSolverSetMethode(p, POISSON_SOLVER_METHODE_FOURIER);
	poissonSolverSetDataFormat(p, FORMAT_FLOAT32);
	poissonSolverSetSize(p, size);
	poissonSolverSetDataIn(p, dataIn);
	poissonSolverSetDataOut(p, dataOut);
	poissonSolverSetNbreThreads(p, 1);
	// poissonSolverGetWorkingMemory(long* cpuMem, long* gpuMem, int processingMode, int methode, int dataFormat);


	// poissonSolverRun(p);
	// poissonSolverRelease(&p);


	void* lap = nullptr;
	laplacianFourierCreate((void**)&lap, size, 0.0, 0.0, FORMAT_FLOAT64, PROCESSING_MODE_CPU);


	fprintf(stderr, "%s ok\n", __FILE__);
	return 0;
}

