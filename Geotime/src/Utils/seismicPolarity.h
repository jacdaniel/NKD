
#ifndef __SEISMICPOLARITY__
#define __SEISMICPOLARITY__


#include <stdlib.h>

size_t c_btesti_NK(char* a, size_t k);
void c_bset_NK(char* a, size_t k);
int getTraceType(char* pol, int ir, int dimx);
int SnapMinMax(char* pol, int ir, int dimx, int type);

int seismisPolarityTraceFill(short* trace, int dimx, char* pol, int polarityDimx);


#endif