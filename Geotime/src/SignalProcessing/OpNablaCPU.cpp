
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>


#include "OpNablaCPU.h"

OpNablaCPU::OpNablaCPU()
{
	threadDataX = nullptr;
	threadDataY = nullptr;
	threadDataZ = nullptr;
	size = 0;
	setNbThread(2);
}

OpNablaCPU::~OpNablaCPU()
{

}

void OpNablaCPU::setNbThread(int n)
{
	this->nbThreads = n;
}

void OpNablaCPU::setSize(int *_size)
{
	this->size = _size;
}



template <typename T> int  OpNablaCPU::nablaX(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;
	omp_set_num_threads(12);
    #pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		int tid = omp_get_thread_num();
		fprintf(stderr, "--> %d %d\n", tid, omp_get_num_threads());
		for (long y = 0; y < size[1]; y++)
		{
			for (long x = 0; x < size[0] - 1; x++)
			{
				g[(long)size[0] * size[1] * z + (long)size[0] * y + x] = f[(long)size[0] * size[1] * (long)z + size[0] * y + x + 1] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			long x = size[0] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)0.0;
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablaY(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			for (long y = 0; y < size[1] - 1; y++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * z + size[0] * (y + 1) + x] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			long y = size[1] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)0.0;
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablaZ(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long y = 0; y < size[1]; y++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			for (long z = 0; z < size[2] - 1; z++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * (z + 1) + size[0] * y + x] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			long z = size[2] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)0.0;
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatX(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long y = 0; y < size[1]; y++)
		{
			long x = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = -f[(long)size[0] * size[1] * z + size[0] * y + x];
			for (x = 1; x < size[0] - 1; x++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * z + size[0] * y + x - 1] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			x = size[0] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * z + size[0] * y + x - 1];
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatY(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			long y = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = -f[(long)size[0] * size[1] * z + size[0] * y + x];
			for (y = 1; y < size[1] - 1; y++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * z + size[0] * (y - 1) + x] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			y = size[1] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * z + size[0] * (y - 1) + x];
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatZ(T* f, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long y = 0; y < size[1]; y++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			long z = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = -f[(long)size[0] * size[1] * z + size[0] * y + x];
			for (z = 1; z < size[2] - 1; z++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * (z - 1) + size[0] * y + x] - f[(long)size[0] * size[1] * z + size[0] * y + x];
			}
			z = size[2] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = f[(long)size[0] * size[1] * (z - 1) + size[0] * y + x];
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablaX(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long y = 0; y < size[1]; y++)
		{
			for (long x = 0; x < size[0] - 1; x++)
			{
				g[(long)size[0] * size[1] * z + (long)size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * (long)z + size[0] * y + x + 1] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			long x = size[0] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)0.0;
		}
	}
	return 0;
}





template <typename T> int OpNablaCPU::nablaY(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			for (long y = 0; y < size[1] - 1; y++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * z + size[0] * (y + 1) + x] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			long y = size[1] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = 0.0f;
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablaZ(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long x = 0; x < size[0]; x++)
	{
		for (long y = 0; y < size[1]; y++)
		{
			for (long z = 0; z < size[2] - 1; z++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * (z + 1) + size[0] * y + x] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			long z = size[2] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)0.0;
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatX(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long y = 0; y < size[1]; y++)
		{
			long x = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)(-(double)f[(long)size[0] * size[1] * z + size[0] * y + x]/norm);
			for (x = 1; x < size[0] - 1; x++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * z + size[0] * y + x - 1] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			x = size[0] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * z + size[0] * y + x - 1])/norm);
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatY(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long z = 0; z < size[2]; z++)
	{
		for (long x = 0; x < size[0]; x++)
		{
			long y = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = -(T)((double)(f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			for (y = 1; y < size[1] - 1; y++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * z + size[0] * (y - 1) + x] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			y = size[1] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * z + size[0] * (y - 1) + x])/norm);
		}
	}
	return 0;
}

template <typename T> int OpNablaCPU::nablatZ(short* f, double norm, int* size, T* g)
{
	if (f == NULL || size == NULL || g == NULL) return 1;

	omp_set_num_threads(30);
	#pragma omp parallel for
	for (long x = 0; x < size[0]; x++)
	{
		for (long y = 0; y < size[1]; y++)
		{
			long z = 0;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = -(T)((double)(f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			for (z = 1; z < size[2] - 1; z++)
			{
				g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * (z - 1) + size[0] * y + x] - f[(long)size[0] * size[1] * z + size[0] * y + x])/norm);
			}
			z = size[2] - 1;
			g[(long)size[0] * size[1] * z + size[0] * y + x] = (T)((double)(f[(long)size[0] * size[1] * (z - 1) + size[0] * y + x])/norm);
		}
	}
	return 0;
}



int OpNablaCPU::nablaX(float* f, int* size, float* g)
{
	return nablaX<float>(f, size, g);
}

int OpNablaCPU::nablaY(float* f, int* size, float* g)
{
	return nablaY<float>(f, size, g);
}

int OpNablaCPU::nablaZ(float* f, int* size, float* g)
{
	return nablaZ<float>(f, size, g);
}



int OpNablaCPU::nablaX(double* f, int* size, double* g)
{
	return nablaX<double>(f, size, g);
}

int OpNablaCPU::nablaY(double* f, int* size, double* g)
{
	return nablaY<double>(f, size, g);
}

int OpNablaCPU::nablaZ(double* f, int* size, double* g)
{
	return nablaZ<double>(f, size, g);
}



int OpNablaCPU::nablatX(float* f, int* size, float* g)
{
	return nablatX<float>(f, size,  g);
}

int OpNablaCPU::nablatY(float* f, int* size, float* g)
{
	return nablatY<float>(f, size, g);
}

int OpNablaCPU::nablatZ(float* f, int* size, float* g)
{
	return nablatZ<float>(f, size, g);
}


int OpNablaCPU::nablatX(double* f, int* size, double* g)
{
	return nablatX<double>(f, size, g);
}

int OpNablaCPU::nablatY(double* f, int* size, double* g)
{
	return nablatY<double>(f, size, g);
}

int OpNablaCPU::nablatZ(double* f, int* size, double* g)
{
	return nablatZ<double>(f, size, g);
}



int OpNablaCPU::nablaX(short* f, double norm, int* size, double* g)
{
	return nablaX<double>(f, norm, size, g);
}
int OpNablaCPU::nablaY(short* f, double norm, int* size, double* g)
{
	return nablaY<double>(f, norm, size, g);
}

int OpNablaCPU::nablaZ(short* f, double norm, int* size, double* g)
{
	return nablaZ<double>(f, norm, size, g);
}

int OpNablaCPU::nablatX(short* f, double norm, int* size, double* g)
{
	return nablatX<double>(f, norm, size, g);
}

int OpNablaCPU::nablatY(short* f, double norm, int* size, double* g)
{
	return nablatY<double>(f, norm, size, g);
}

int OpNablaCPU::nablatZ(short* f, double norm, int* size, double* g)
{
	return nablatZ<double>(f, norm, size, g);
}
