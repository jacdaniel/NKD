
#ifndef __POISSONSOLVER__
#define __POISSONSOLVER__


int poissonSolverInit(void **p);
int poissonSolverRelease(void **p);

int poissonSolverSetMethode(void *p, int methode);
int poissonSolverSetProcessingMode(void *p, int processingMode);
int poissonSolverSetDataFormat(void *p, int dataFormat);
int poissonSolverSetSize(void *p, int *size);
int poissonSolverSetDataIn(void *p, void *dataIn);
int poissonSolverSetDataOut(void *p, void *dataOut);
int poissonSolverSetNbreThreads(void *p, int nbThreads);
int poissonSolverGetWorkingMemory(long *cpuMem, long *gpuMem, int processingMode, int methode, int dataFormat);

int poissonSolverRun(void*p);



#endif
