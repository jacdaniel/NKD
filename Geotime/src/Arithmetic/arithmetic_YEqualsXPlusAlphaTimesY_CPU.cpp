
#include <utils.h>
#include <vector>
#include <XDATA.h>
#include <XData.h>
#include <arithmetic_YEqualsXPlusAlphaTimesY_CPU.h>


template<typename T> int Arithmetic_YEqualsXPlusAlphaTimesY_CPU(T* y, size_t size, T* x, double alpha)
{
	for (size_t i = 0; i < size; i++)
		y[i] = x[i] + alpha * y[i];
	return SUCCESS;
}

int Arithmetic_YEqualsXPlusAlphaTimesY_CPU(float* y, size_t size, float* x, double alpha)
{
	return Arithmetic_YEqualsXPlusAlphaTimesY_CPU<float>((float*)y, size, (float*)x, alpha);
}

int Arithmetic_YEqualsXPlusAlphaTimesY_CPU(double* y, size_t size, double* x, double alpha)
{
	return Arithmetic_YEqualsXPlusAlphaTimesY_CPU<double>((double*)y, size, (double*)x, alpha);
}

int Arithmetic_YEqualsXPlusAlphaTimesY_CPU(void* y, size_t size, void* x, double alpha, int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32:
		return Arithmetic_YEqualsXPlusAlphaTimesY_CPU<float>((float*)y, size, (float*)x, alpha);
		break;
	case FORMAT_FLOAT64:
		return Arithmetic_YEqualsXPlusAlphaTimesY_CPU<double>((double*)y, size, (double*)x, alpha);
		break;
	case FORMAT_XDATA:
		return Arithmetic_YEqualsXPlusAlphaTimesY_CPU(xDataGetData(y), xDataGetSize0(y), xDataGetData(x), alpha, xDataGetDataFormat(y));
		break;
	case FORMAT_ARRAY_XDATA:
		std::vector<void*>* vy = (std::vector<void*>*)y;
		std::vector<void*>* vx = (std::vector<void*>*)x;
		for (int i = 0; i < vy->size(); i++)
		{
			Arithmetic_YEqualsXPlusAlphaTimesY_CPU((*vy)[i], (size_t)xDataGetSize((*vy)[i]), (*vx)[i], alpha, FORMAT_XDATA);
		}
		return SUCCESS;
		break;
	}
	return FAIL;
}


