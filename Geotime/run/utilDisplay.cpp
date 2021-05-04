
#include <stdio.h>
#include <utils.h>
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
	}
}