
#include <math.h>
#include <malloc.h>
#include <utils.h>

#include <cuda.h>
#include <cuda_runtime.h>

#include <laplacian.h>

#define VALUE_INVERSE_LAPLACIAN_1D(x, dx, lambda, mu, value) { \
    if ( x == 0 ) { value = 0.0 ; } \
    else { \
        double wx = 2.0 * PI * (double)x / (double)(2.0*dx); \
        double l1 = -2.0 + 2.0 * cos(wx); \
        double l2 = 6.0 - 8.0 * cos(wx) + 2.0 * cos(2.0 * wx); \
        value = 1.0 / (l1 - mu * l2 - lambda); \
    }\
}

#define VALUE_INVERSE_LAPLACIAN_2D(x, y, dx, dy, lambda, mu, value) { \
    if ( x == 0 && y == 0 ) { value = 0.0; } \
    else {\
        double wx = 2.0 * PI * (double)x / (double)(2.0*dx); \
        double wz = 2.0 * PI * (double)y / (double)(2.0*dy); \
        double l1 = -4.0 + 2.0 * cos(wx) + 2.0 * cos(wz); \
        double l2 = 20.0 - 16.0 * cos(wx) - 16.0 * cos(wz) + 8.0 * cos(wx) * cos(wz) + 2.0 * cos(2.0 * wx) + 2.0 * cos(2.0 * wz); \
        double val = l1 - mu * l2 - lambda; \
        value = 1.0 / val; \
    }\
}


template<typename T>__global__ void laplacian1DFourierGPU_kernel(T* filter, long dx, double lambda, double mu)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    if (x < dx)
    {
        double v;
        VALUE_INVERSE_LAPLACIAN_1D(x, dx, lambda, mu, v)
            filter[x] = (T)v;
    }
}

template<typename T>__global__ void laplacian2DFourierGPU_kernel(T* filter, long dx, long dy, double lambda, double mu)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;
    if ( x < dx && y < dy )
    {
        double v;
        VALUE_INVERSE_LAPLACIAN_2D(x, y, dx, dy, lambda, mu, v)
        filter[y * dx + x] = (T)v;
    }
}



template <typename T> int laplacian1DFourierCPUCreate(T** filter, long dx, double lambda, double mu)
{
    long x;

    callocSafe((void**)filter, dx , sizeof(double));
    double v;
    for (x = 0; x < dx; x++)
    {
        VALUE_INVERSE_LAPLACIAN_1D(x, dx, lambda, mu, v)
            (*filter)[x] = (T)v;
    }
  return SUCCESS;
}


template <typename T> int laplacian2DFourierCPUCreate(T **filter, long dx, long dy, double lambda, double mu)
{
  long x, y;
  
  callocSafe((void**)filter, dx*dy, sizeof(double));
  double v;
  for (y = 0; y< dy; y++)
      for (x = 0; x < dx; x++)
      {
          VALUE_INVERSE_LAPLACIAN_2D(x, y, dx, dy, lambda, mu, v)
          (*filter)[y * dx + x] = (T)v;
      }
  return SUCCESS;
}








template<typename T> int laplacianFourierCPUcreate(T** filter, int *size, double lambda, double mu)
{
    callocSafe((void**)filter, size[0]*size[1]*size[2], sizeof(T));
    if (size[1] == 1 && size[2] == 1)
    {
        return laplacian1DFourierCPUCreate<T>(filter, size[0], lambda, mu);
    }
    if (size[2] == 1)
    {
        return laplacian2DFourierCPUCreate<T>(filter, size[0], size[1], lambda, mu);
    }
    else
    {
    }
    return SUCCESS;
}

template<typename T> int laplacianFourierGPUcreate(T** filter, int* size, double lambda, double mu)
{
    cudaMalloc((void**)filter, size[0]*size[1]*size[2] * sizeof(T));
    if (size[1] == 1 && size[2] == 1)
    {
        dim3 block(10);
        dim3 grid((size[0] - 1) / block.x + 1);
        laplacian1DFourierGPU_kernel<T> << <grid, block >> > ((T*)(&filter), size[0], lambda, mu);
        return SUCCESS;
    }
    if (size[2] == 1)
    {
        dim3 block(10, 10);
        dim3 grid((size[0] - 1) / block.x + 1, (size[1] - 1) / block.y + 1);
        laplacian2DFourierGPU_kernel<T> << <grid, block >> > ((T*)&filter, size[0], size[1], lambda, mu);
        return SUCCESS;
    }
    else
    {
    }
    return SUCCESS;
}





static int laplacianFourierCPUFormatCreate(void** filter, int* size, double lambda, double mu, int format)
{
    if (format == FORMAT_FLOAT32)
    {
        return laplacianFourierCPUcreate<float>((float**)filter, size, lambda, mu);
    }
    else
    {
        return laplacianFourierCPUcreate<double>((double**)filter, size, lambda, mu);
    }
}

static int laplacianFourierGPUFormatCreate(void** filter, int* size, double lambda, double mu, int format)
{
    if (format == FORMAT_FLOAT32)
    {
        return laplacianFourierGPUcreate<float>((float**)filter, size, lambda, mu);
    }
    else
    {
        return laplacianFourierGPUcreate<double>((double**)filter, size, lambda, mu);
    }
}

int laplacianFourierCreate(void** filter, int* size, double lambda, double mu, int format, int processingMode)
{
    if (filter == nullptr) return FAIL;
    if (processingMode == PROCESSING_MODE_CPU)
    {
        return laplacianFourierCPUFormatCreate(filter, size, lambda, mu, format);
    }
    else
    {
        return laplacianFourierGPUFormatCreate(filter, size, lambda, mu, format);
    }
}


