#ifndef __SURFACERECONSTRUCTIONCPU__
#define __SURFACERECONSTRUCTIONCPU__


int surfaceReconstructionCPUInit(void** p);
int surfaceReconstructionCPURelease(void** p);

int surfaceReconstructionCPUSetDataFormat(void* p, int dataFormat);
int surfaceReconstructionCPUSetSize(void* p, int* size);
int surfaceReconstructionCPUSetSolverType(void* p, int solverType);
int surfaceReconstructionCPUSetSurfaceOut(void* p, void* surfaceOut);

int surfaceReconstructionCPUSetNbIter(void* p, int nbIter);
int surfaceReconstructionCPUSetNormalManager(void* p, void* normalManager);
int surfaceReconstructionCPUSetSeed(void* p, int x0, int y0, int z0);
int surfaceReconstructionCPURun(void* _p);


#endif