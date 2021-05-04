
#ifndef __LAPLACIAN__
#define __LAPLACIAN__

#define LAPLACIAN_FOURIER_FULL 0
#define LAPLACIAN_FOURIER_HALF 1

int laplacianFourierCreate(void** filter, int* size, double lambda, double mu, int format, int processingMode);

#endif
