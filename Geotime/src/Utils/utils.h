
#ifndef __UTILS__
#define __UTILS__

int mallocSafe_(void**ptr, long size, char *file, long line);
#define MALLOC_SUCCESS         			   0
#define MALLOC_FAIL_CANNOT_ALLOCATE_MEMORY 1
#define MALLOC_FAIL_NULLPTR    			   2
#ifndef MALLOC_SAFE
#define mallocSafe(ptr, size) mallocSafe_(ptr, size, __FILE__, __LINE__)
#endif

int callocSafe_(void**ptr, long size, long sizeof_, char *file, long line);
#define CALLOC_SUCCESS         			   0
#define CALLOC_FAIL_CANNOT_ALLOCATE_MEMORY 1
#define CALLOC_FAIL_NULLPTR    			   2
#ifndef CALLOC_SAFE
#define callocSafe(ptr, size, sizeof_) callocSafe_(ptr, size, sizeof_, __FILE__, __LINE__)
#endif

#define ERROR_MESSAGE(msg) fprintf(stderr, "[ERROR] %s [ %s - %d ]\n", __FILE__, __LINE__);


#define DIP_SHORT_NORM 1000.0

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifndef FREE
#define FREE(x){ \
if ( x != nullptr ) { \
        free(x); \
        x = nullptr;} }
#endif


#define PI 3.1415926535


#define SUCCESS 0
#define FAIL    1

#define _1D 1
#define _2D 2
#define _3D 3

#define PROCESSING_MODE_CPU 0
#define PROCESSING_MODE_GPU 1

#define POISSON_SOLVER_METHODE_FOURIER 0
#define POISSON_SOLVER_METHODE_CG      1

#define POISSON_SOLVER_BOUNDARY_NEUMANN   0
#define POISSON_SOLVER_BOUNDARY_DIRICHLET 1

#define FORMAT_ERROR       -1
#define FORMAT_UCHAR        0
#define FORMAT_CHAR         1
#define FORMAT_INT16        2
#define FORMAT_UINT16       3
#define FORMAT_INT32        4
#define FORMAT_UINT32       5
#define FORMAT_INT64        6
#define FORMAT_UINT64       7
#define FORMAT_FLOAT32      8
#define FORMAT_FLOAT64      9
#define FORMAT_XDATA       10
#define FORMAT_ARRAY_XDATA 11

int getDimsFromSize(int* size);



#endif
