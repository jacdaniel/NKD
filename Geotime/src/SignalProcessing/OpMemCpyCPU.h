#ifndef __OPMEMCPYCPU__
#define __OPMEMCPYCPU__


int opMemCpy(void* dst, void* src, size_t size, int dataFormat);
int opMemCpyCHS(void* dst, void* src, size_t size, int dataFormat);

#endif