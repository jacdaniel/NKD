#ifndef __NORMALMANAGERCPU__
#define __NORMALMANAGERCPU__


int normalManagerCPUInit(void** p);
int normalManagerCPURelease(void** p);

int normalManagerCPUSetSize(void* p, int* size);
int normalManagerCPUSetDip(void* p, void* dipxy, void* dipxz);
int normalManagerCPUSetDipInDataFormat(void* p, int dataFormat);
int normalManagerCPUSetDipOutDataFormat(void* p, int dataFormat);
int normalManagerCPUSetDataIn(void* p, void* dataIn);
int normalManagerCPUSetDataInFormat(void* p, int format);

int normalManagerCPURun(void* _p);





#endif