
#include <stdio.h>
#include <vector>
#include <utils.h>
#include <xData.h>
#include <utilDisplay.h>


void UtilDisplay::display(double* data, long nbre)
{
	for (long i = 0; i < nbre; i++)
	{
		fprintf(stderr, "[%d] %f\n", i, data[i]);
	}
}

void UtilDisplay::display(float* data, long nbre)
{
	for (long i = 0; i < nbre; i++)
	{
		fprintf(stderr, "[%d] %f\n", i, data[i]);
	}
}


void UtilDisplay::display(void* data, long size, int dataFormat)
{
	switch (dataFormat)
	{
		case FORMAT_FLOAT32:  display((float*)data, size); break;
		case FORMAT_FLOAT64:  display((double*)data, size); break;
		case FORMAT_XDATA:   display((void*)xDataGetData(data), xDataGetSize0(data), xDataGetDataFormat(data)); break;
		case FORMAT_ARRAY_XDATA:
		{
			std::vector<void*>* p0 = (std::vector<void*>*)data;
			for (int i = 0; i < p0->size(); i++)
			{
				fprintf(stderr, "============= array [%d] =============\n", i);
				display((*p0)[i], 0, FORMAT_XDATA);
			}
		}
		break;
	}
}