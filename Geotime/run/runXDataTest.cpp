
#include <malloc.h>
#include <stdio.h>
#include <utils.h>
#include <xData.h>
#include <vector>
#include <utilDisplay.h>
#include <OpDotCPU.h>
#include <runXDataTest.h>



int runXDataTest(int argc, char** argv)
{
	// D 1
	int size1[] = { 20, 20, 1 };
	float* data1 = (float*)calloc(size1[0] * size1[1] * size1[2], sizeof(float));
	for (int i = 0; i < size1[0] * size1[1] * size1[2]; i++) data1[i] = 10;
	void* d1 = nullptr;
	xDataInit(&d1);
	xDataSetData(d1, data1);
	xDataSetSize(d1, size1);
	xDataSetDataFormat(d1, FORMAT_FLOAT32);

	// D 2
	int size2[] = { 20, 20, 1 };
	float* data2 = (float*)calloc(size2[0] * size2[1] * size2[2], sizeof(float));
	for (int i = 0; i < size2[0] * size2[1] * size2[2]; i++) data2[i] = 10;
	void* d2 = nullptr;
	xDataInit(&d2);
	xDataSetData(d2, data2);
	xDataSetSize(d2, size2);
	xDataSetDataFormat(d2, FORMAT_FLOAT32);

	// D 3
	int size3[] = { 20, 20, 1 };
	double* data3 = (double*)calloc(size3[0] * size3[1] * size3[2], sizeof(double));
	for (int i = 0; i < size3[0] * size3[1] * size3[2]; i++) data3[i] = 1000 + i;
	void* d3 = nullptr;
	xDataInit(&d3);
	xDataSetData(d3, data3);
	xDataSetSize(d3, size3);
	xDataSetDataFormat(d3, FORMAT_FLOAT64);

	std::vector<void*> d;
	d.push_back(d1);
	d.push_back(d2);
	// d.push_back(d3);

	std::vector<void*> d0;
	d0.push_back(d1);
	d0.push_back(d2);

	void* vd = nullptr;
	xDataInit(&vd);
	xDataSetData(vd, (void*)&d);
	xDataSetDataFormat(vd, FORMAT_ARRAY_XDATA);




	void* pp = d[0];
	float* dd1 = (float*)xDataGetData(pp);

	for (int i = 0; i<d.size(); i++)
	{
		void* p0 = d[i];
		void* data = xDataGetData(p0);
		int* size = xDataGetSize(p0);
		int dataFormat = xDataGetDataFormat(p0);
		UtilDisplay::display(data, 20, dataFormat);
	}

	double r = OpDotCPU::dot((void*)&d, 0, (void*)&d0, FORMAT_ARRAY_XDATA);







	fprintf(stderr, "runXDataTest [%d] OK\n", (int)d.size());
	return 0;
}


