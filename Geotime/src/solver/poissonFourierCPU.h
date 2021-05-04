
#ifndef __POISSONFOURIERCPU__
#define __POISSONFOURIERPCU__

#define POISSONFOURIER_TYPE_DCTII 0
#define POISSONFOURIER_TYPE_DSTI  1

int poissonFourierCPUInit(void **p);
int poissonFourierCPURelease(void **p);

int poissonFourierCPUSetDataFormat(void *p, int dataFormat);
int poissonFourierCPUSetSize(void *p, int *size);
int poissonFourierCPUSetDataIn(void *p, void *dataIn);
int poissonFourierCPUSetDataOut(void *p, void *dataOut);
int poissonFourierCPUSetNbreThreads(void *p, int nbThreads);

int poissonFourierCPURun(void* _p);





#endif
