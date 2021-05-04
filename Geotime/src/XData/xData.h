
#ifndef __XDATA__
#define __XDATA__

int xDataInit(void** p);
int xDataRelease(void** p);
int xDataSetSize(void* p, int* size);
int xDataSetData(void* p, void* data);
int xDataSetDataFormat(void* p, int dataFormat);
int xDataDataMalloc(void* p);
int xDataDataFree(void* p);
int xDataClear(void* p);
void* xDataGetData(void* p);
int* xDataGetSize(void* p);
size_t xDataGetSize0(void* p);
int xDataGetDataFormat(void* p);

//int xDataGetLength(void* p);

#endif