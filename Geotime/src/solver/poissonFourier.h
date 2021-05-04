
#ifndef __POISSONFOURIER__
#define __POISSONFOURIER__

#define POISSONFOURIER_TYPE_DCTII 0
#define POISSONFOURIER_TYPE_DSTI  1

int poissonFourierInit(void **_p);
int poissonFourierRelease(void **_p);
int poissonFourierSetProcessingMode(void *p, int processingMode);
int poissonFourierSetDataFormat(void *p, int dataFormat);
int poissonFourierSetSize(void *p, int *size);
int poissonFourierSetDataIn(void *p, void *dataIn);
int poissonFourierSetDataOut(void *p, void *dataOut);
int poissonFourierSetNbreThreads(void *p, int nbThreads);

int poissonFourierRun(void*p);




#endif
