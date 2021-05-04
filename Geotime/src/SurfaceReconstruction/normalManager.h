#ifndef __NORMALMANAGER__
#define __NORMALMANAGER__


int normalManagerInit(void** p);
int normalManagerRelease(void** p);

int normalManagerSetSize(void *p, int *size);
int normalManagerSetDip(void* p, void* dipxy, void* dipxz);
int normalManagerSetDipInDataFormat(void* p, int dataFormat);
int normalManagerSetDipOutDataFormat(void* p, int dataFormat);
int normalManagerSetDataIn(void* p, void* dataIn);
int normalManagerSetDataOut(void* p, void* outxy, void* outxz);
int normalManagerSetProcessingMode(void* p, int processingMode);
int normalManagerSetDataInFormat(void* p, int format);

int normalManagerRun(void* _p);


#endif