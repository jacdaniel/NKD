
#include <string.h>
#include <vector>
#include <utils.h>
#include <xData.h>
#include <OpMemCpyCPU.h>


template <typename T> int opMemCpyCHS(T* src, T* dst, size_t size)
{
	for (size_t add = 0; add < size; add++)
		dst[add] = -src[add];
	return SUCCESS;
}


// TODO
int opMemCpy(void* dst, void* src, size_t size, int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32:
		memcpy(dst, src, size * sizeof(float));
		return SUCCESS;
		break;
	case FORMAT_FLOAT64:
		memcpy(dst, src, size * sizeof(double));
		return SUCCESS;
		break;
	case FORMAT_XDATA:
		opMemCpy(xDataGetData(dst), xDataGetData(src), xDataGetSize0(dst), xDataGetDataFormat(src));
		return SUCCESS;
		break;
	case FORMAT_ARRAY_XDATA:
		std::vector<void*>* vsrc = (std::vector<void*>*)src;
		std::vector<void*>* vdst = (std::vector<void*>*)dst;
		for (int i = 0; i < vsrc->size(); i++)
		{
			opMemCpy((*vdst)[i], (*vsrc)[i], 0, FORMAT_XDATA);
		}
		return SUCCESS;
		break;
	}
	return FAIL;
}

int opMemCpyCHS(void* dst, void* src, size_t size, int dataFormat)
{
	switch (dataFormat)
	{
		case FORMAT_FLOAT32: opMemCpyCHS<float>((float*)dst, (float*)src, size); break;
		case FORMAT_FLOAT64: opMemCpyCHS<double>((double*)dst, (double*)src, size); break;
		case FORMAT_XDATA:
			opMemCpyCHS(xDataGetData(dst), xDataGetData(src), xDataGetSize0(src), xDataGetDataFormat(src));
			break;
		case FORMAT_ARRAY_XDATA:
			std::vector<void*>* vsrc = (std::vector<void*>*)src;
			std::vector<void*>* vdst = (std::vector<void*>*)dst;
			for (int i = 0; i < vsrc->size(); i++)
			{
				opMemCpyCHS((*vdst)[i], (*vsrc)[i], 0, FORMAT_XDATA);
			}
			return SUCCESS;
			break;
	}
	return SUCCESS;
}




