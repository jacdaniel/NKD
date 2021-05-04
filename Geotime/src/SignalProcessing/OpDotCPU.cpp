

#include <utils.h>
#include <xData.h>
#include <vector>
#include <OpDotCPU.h>

OpDotCPU::OpDotCPU()
{
}

OpDotCPU::~OpDotCPU()
{
}

template <typename T> double  OpDotCPU::dot(T* f, long size, T* g)
{
	double s = 0.0;
	for (long add = 0; add < size; add++)
		s += (double)f[add] * (double)g[add];
	return s;
}


double OpDotCPU::dot(float* f, long size, float* g)
{
	return dot<float>(f, size, g);
}

double OpDotCPU::dot(double* f, long size, double* g)
{
	return dot<double>(f, size, g);
}

double OpDotCPU::dot(void* f, long size, void* g, int dataFormat)
{
	switch (dataFormat)
	{
		case FORMAT_FLOAT32: return dot((float*)f, size, (float*)g); break;
		case FORMAT_FLOAT64: return dot((double*)f, size, (double*)g); break;
		case FORMAT_XDATA: return dot(xDataGetData(f), xDataGetSize0(f), xDataGetData(g), xDataGetDataFormat(f)); break;
		case FORMAT_ARRAY_XDATA:
			std::vector<void*> *vf = (std::vector<void*>*)f;
			std::vector<void*>*vg = (std::vector<void*> *)g;
			double val = 0.0;
			for (int i = 0; i < vf->size(); i++)
			{				
				val += dot((*vf)[i], 0, (*vg)[i], FORMAT_XDATA);
			}
			return val;
			break;
	}
	return FAIL;
}




