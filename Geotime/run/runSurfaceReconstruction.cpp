

#include <stdio.h>
#include <malloc.h>
#include <OpNablaCPU.h>
#include <utils.h>
#include <utilDisplay.h>
#include <normalManager.h>
#include <surfaceReconstruction.h>


static void dipInit(void* dipxy, void* dipxz, int* size)
{
	long size0 = (long)size[0] * size[1] * size[2];
	for (long i = 0; i < size0; i++)
	{
		((short*)dipxy)[i] = 1000;
		((short*)dipxz)[i] = -1000;
	}
}


static void tauInit(void* tau, int* size)
{
	long size0 = (long)size[0] * size[1] * size[2];
	for (long i = 0; i < size0; i++)
	{
		((float*)tau)[i] = 10;

	}
}

int runSurfaceReconstruction()
{
	int size[] = { 100, 100, 1 };
	void* dipxy, * dipxz, *tau, *outxy;

	int processingMode = PROCESSING_MODE_CPU;
	int dataFormat = FORMAT_FLOAT32;
	long size0 = (long)size[0] * size[1] * size[2];

	callocSafe((void**)&dipxy, size0, sizeof(short));
	callocSafe((void**)&dipxz, size0, sizeof(short));
	callocSafe((void**)&tau, size0, sizeof(double));
	callocSafe((void**)&outxy, size0, sizeof(short));

	tauInit(tau, size);
	dipInit(dipxy, dipxz, size);

	void* normalManager = nullptr;
	normalManagerInit((void**)&normalManager);
	normalManagerSetSize(normalManager, size);
	normalManagerSetDip(normalManager, dipxy, nullptr);
	normalManagerSetDipInDataFormat(normalManager, FORMAT_INT16);
	normalManagerSetDipOutDataFormat(normalManager, FORMAT_INT16);
	normalManagerSetDataIn(normalManager, tau);
	normalManagerSetDataOut(normalManager, outxy, nullptr);
	normalManagerSetProcessingMode(normalManager, processingMode);
	normalManagerSetDataInFormat(normalManager, dataFormat);

	void* s = nullptr;
	surfaceReconstructionInit((void**)&s);
	surfaceReconstructionSetProcessingMode(s, processingMode);
	surfaceReconstructionSetDataFormat(s, dataFormat);
	surfaceReconstructionSetSize(s, size);
	surfaceReconstructionSetSolverType(s, POISSON_SOLVER_METHODE_FOURIER);
	surfaceReconstructionSetNbIter(s, 20);
	surfaceReconstructionSetNormalManager(s, normalManager);
	surfaceReconstructionSetSeed(s, 10, 50, 0);
	surfaceReconstructionSetSurfaceOut(s, tau);
	surfaceReconstructionRun(s);

	UtilDisplay::display(tau, 100, dataFormat);



	// normalManagerRun(normalManager);

	return SUCCESS;
}