
#include <vector>
#include <utils.h>
#include <xData.h>
#include <arithmetic_YPlusEqualsAlphaTimesX_CPU.h>

template<typename T> int Arithmetic_YPlusEqualsAlphaTimesX_CPU(T* y, size_t size, double alpha, T* x)
{
	for (size_t i = 0; i < size; i++)
	{
		y[i] += alpha * x[i];
	}
	return SUCCESS;
}


int Arithmetic_YPlusEqualsAlphaTimesX_CPU(float* y, size_t size, double alpha, float* x)
{
	return Arithmetic_YPlusEqualsAlphaTimesX_CPU<float>((float*) y, size, alpha, (float*)x);
}

int Arithmetic_YPlusEqualsAlphaTimesX_CPU(double* y, size_t size, double alpha, double* x)
{
	return Arithmetic_YPlusEqualsAlphaTimesX_CPU<double>((double*)y, size, alpha, (double*)x);
}

int Arithmetic_YPlusEqualsAlphaTimesX_CPU(void* y, size_t size, double alpha, void* x, int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32:
		return Arithmetic_YPlusEqualsAlphaTimesX_CPU<float>((float*)y, size, alpha, (float*)x);
		break;
	case FORMAT_FLOAT64:
		return Arithmetic_YPlusEqualsAlphaTimesX_CPU<double>((double*)y, size, alpha, (double*)x);
		break;
	case FORMAT_XDATA:
		return Arithmetic_YPlusEqualsAlphaTimesX_CPU(xDataGetData(y), xDataGetSize0(y), alpha, xDataGetData(x), xDataGetDataFormat(y));
		break;
	case FORMAT_ARRAY_XDATA:
		std::vector<void*>* vy = (std::vector<void*>*)y;
		std::vector<void*>* vx = (std::vector<void*>*)x;
		for (int i = 0; i < vy->size(); i++)
		{
			Arithmetic_YPlusEqualsAlphaTimesX_CPU((*vy)[i], (size_t)xDataGetSize((*vy)[i]), alpha, (*vx)[i], FORMAT_XDATA);
		}
		return SUCCESS;
		break;
	}
	return FAIL;
}