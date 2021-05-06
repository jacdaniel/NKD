
#include <vector>
#include <utils.h>
#include <xData.h>
#include <arithmetic_YEqualsAMinusB.h>



template<typename T> int Arithmetic_YEqualsAMinusB(T* y, size_t size, T* A, T* B)
{
	for (size_t add = 0; add < size; add++)
	{
		y[add] = A[add] - B[add];
	}
	return SUCCESS;
}


int Arithmetic_YEqualsAMinusB(float* y, size_t size, float* A, float* B)
{
	return Arithmetic_YEqualsAMinusB<float>((float*)y, size, (float*)A, (float*)B);
}

int Arithmetic_YEqualsAMinusB(double* y, size_t size, double* A, double* B)
{
	return Arithmetic_YEqualsAMinusB<double>(y, size, A, B);
}


int Arithmetic_YEqualsAMinusB(void* y, size_t size, void* A, void* B, int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32: return Arithmetic_YEqualsAMinusB<float>((float*)y, size, (float*)A, (float*)B); break;
	case FORMAT_FLOAT64: return Arithmetic_YEqualsAMinusB<double>((double*)y, size, (double*)A, (double*)B); break;
	case FORMAT_XDATA:
		return Arithmetic_YEqualsAMinusB((void*)xDataGetData(y), xDataGetSize0(y), (void*)xDataGetData(A), (void*)xDataGetData(B), xDataGetDataFormat(y));
		break;
	case FORMAT_ARRAY_XDATA:
	{
		std::vector<void*>* py = (std::vector<void*>*)y;
		std::vector<void*>* pA = (std::vector<void*>*)A;
		std::vector<void*>* pB = (std::vector<void*>*)B;
		for (int i = 0; i < py->size(); i++)
		{
			Arithmetic_YEqualsAMinusB(xDataGetData((*py)[i]), 0, xDataGetData((*pA)[i]), xDataGetData((*pB)[i]), FORMAT_XDATA);
		}
	}
	return SUCCESS;
	break;
	}
}

