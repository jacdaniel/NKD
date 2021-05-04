#ifndef __SURFACERECONSTRUCTION__
#define __SURFACERECONSTRUCTION__


int surfaceReconstructionInit(void** p);
int surfaceReconstructionRelease(void** p);

int surfaceReconstructionSetProcessingMode(void* p, int processingMode);
int surfaceReconstructionSetDataFormat(void* p, int dataFormat);
int surfaceReconstructionSetSize(void* p, int* size);
int surfaceReconstructionSetSolverType(void* p, int solverType);
int surfaceReconstructionSetSurfaceOut(void* p, void* surfaceOut);

int surfaceReconstructionSetNbIter(void* p, int nbIter);
int surfaceReconstructionSetNormalManager(void* p, void* normalManager);
int surfaceReconstructionSetSeed(void* p, int x0, int y0, int z0);
int surfaceReconstructionRun(void* _p);





#endif