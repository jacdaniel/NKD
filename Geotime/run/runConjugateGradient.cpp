
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <vector>
#include <utils.h>
#include <xData.h>
#include <utilDisplay.h>
#include <conjugateGradient.h>
#include <runConjugateGradient.h>



class CALLBACK : public ConjugateGradientCallBack
{
public:
	CALLBACK();
	~CALLBACK();
	virtual int f(void* in, void* out);
	virtual int Preconditionner(void* in, void* out);
	void setSize(size_t size);
	void setDataFormat(int dataFormat);

private:
	size_t size0;
	int* size;
	int dataFormat;
};

CALLBACK::CALLBACK(){}
CALLBACK::~CALLBACK() {}


static void genericCallBack(void* f, int size, void* g, int dataFormat)
{
	switch (dataFormat)
	{
	 case FORMAT_FLOAT32:
	 {
		 float* pin = (float*)f;
		 float* pout = (float*)g;
		 pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
		 pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
		 pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];
	 }
	 break;
	 case FORMAT_FLOAT64:
	 {
		 double* pin = (double*)f;
		 double* pout = (double*)g;
		 pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
		 pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
		 pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];
	 }
	 break;
	 case FORMAT_XDATA:
		 genericCallBack(xDataGetData(f), xDataGetSize0(f), xDataGetData(g), xDataGetDataFormat(f));
		 break;
	 case FORMAT_ARRAY_XDATA:
	 {
		 std::vector<void*>* pin = (std::vector<void*>*)f;
		 std::vector<void*>* pout = (std::vector<void*>*)g;
		 for (int i = 0; i < pin->size(); i++)
		 {
			 genericCallBack((*pin)[i], 0, (*pout)[i], FORMAT_XDATA);
		 }
		 break;
	 }
	}
}

static void genericPreconditionner(void* f, int size, void* g, int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32: memcpy(g, f, size * sizeof(float)); break;
	case FORMAT_FLOAT64: memcpy(g, f, size * sizeof(double)); break;
	case FORMAT_XDATA:
	{
		void* in = xDataGetData(f);
		void* out = xDataGetData(g);
		int size = xDataGetSize0(f);
		int dataFormat = xDataGetDataFormat(f);
		genericPreconditionner(in, size, out, dataFormat);
	}
	break;
	case FORMAT_ARRAY_XDATA:
	{
		std::vector<void*>* pin = (std::vector<void*>*)f;
		std::vector<void*>* pout = (std::vector<void*>*)g;
		for (int i = 0; i < pin->size(); i++)
		{
			genericPreconditionner((*pin)[i], 0, (*pout)[i], FORMAT_XDATA);
		}
	}
	break;
	}
}



int CALLBACK::f(void* in, void* out)
{
	genericCallBack(in, size0, out, dataFormat);
	return SUCCESS;
}

int CALLBACK::Preconditionner(void* in, void* out)
{
	genericPreconditionner(in, size0, out, dataFormat);
	return SUCCESS;
}

void CALLBACK::setSize(size_t size)
{
	this->size0 = size;
}


void CALLBACK::setDataFormat(int dataFormat)
{
	this->dataFormat = dataFormat;
}


// xdata
int runConjugateGradient_xdata(int argc, char** argv)
{
	std::vector<void*> vrhs;
	std::vector<void*> vx;


	double rhs[] = { 4880.0, 6344.0, 5504.0 };
	double x[] = { 0.0, 0.0, 0.0 };
	int size[] = { 3, 1, 1};
	int dataFormat = FORMAT_FLOAT64;

	void* x_rhs = nullptr;
	xDataInit((void**)&x_rhs);
	xDataSetData(x_rhs, rhs);
	xDataSetSize(x_rhs, size);
	xDataSetDataFormat(x_rhs, dataFormat);

	void* x_x = nullptr;
	xDataInit((void**)&x_x);
	xDataSetData(x_x, x);
	xDataSetSize(x_x, size);
	xDataSetDataFormat(x_x, dataFormat);

	vrhs.push_back(x_rhs);
	vx.push_back(x_x);

	CALLBACK* callBack = new CALLBACK();
	callBack->setSize(3);
	callBack->setDataFormat(FORMAT_XDATA);

	ConjugateGradient* cg = new ConjugateGradient();
	cg->setProcessingMode(PROCESSING_MODE_CPU);
	cg->setDataFormat(FORMAT_XDATA);
	cg->setNbIter(100);
	cg->setSize(3);
	cg->setRhs((void*)x_rhs);
	cg->setX((void*)x_x);
	cg->setCallback(callBack);
	cg->run();

	UtilDisplay::display((void*)x_x, 3, FORMAT_XDATA);
	fprintf(stderr, "conjugate gradient ok\n");
	return 0;
}

// vector xdata
int runConjugateGradient(int argc, char** argv)
{
	std::vector<void*> vrhs;
	std::vector<void*> vx;

	double rhs_1[] = { 4880.0, 6344.0, 5504.0 };
	double x_1[] = { 0.0, 0.0, 0.0 };
	int size_1[] = { 3, 1, 1 };
	int dataFormat_1 = FORMAT_FLOAT64;

	double rhs_2[] = { 7787, 10124, 8780 };
	double x_2[] = { 0.0, 0.0, 0.0 };
	int size_2[] = { 3, 1, 1 };
	int dataFormat_2 = FORMAT_FLOAT64;

	float rhs_3[] = { 10694, 13904, 12056 };
	float x_3[] = { 0.0, 0.0, 0.0 };
	int size_3[] = { 3, 1, 1 };
	int dataFormat_3 = FORMAT_FLOAT32;


	void* x_rhs_1 = nullptr;
	xDataInit((void**)&x_rhs_1);
	xDataSetData(x_rhs_1, rhs_1);
	xDataSetSize(x_rhs_1, size_1);
	xDataSetDataFormat(x_rhs_1, dataFormat_1);

	void* x_x_1 = nullptr;
	xDataInit((void**)&x_x_1);
	xDataSetData(x_x_1, x_1);
	xDataSetSize(x_x_1, size_1);
	xDataSetDataFormat(x_x_1, dataFormat_1);

	void* x_rhs_2 = nullptr;
	xDataInit((void**)&x_rhs_2);
	xDataSetData(x_rhs_2, rhs_2);
	xDataSetSize(x_rhs_2, size_2);
	xDataSetDataFormat(x_rhs_2, dataFormat_2);

	void* x_x_2 = nullptr;
	xDataInit((void**)&x_x_2);
	xDataSetData(x_x_2, x_2);
	xDataSetSize(x_x_2, size_2);
	xDataSetDataFormat(x_x_2, dataFormat_2);

	void* x_rhs_3 = nullptr;
	xDataInit((void**)&x_rhs_3);
	xDataSetData(x_rhs_3, rhs_3);
	xDataSetSize(x_rhs_3, size_3);
	xDataSetDataFormat(x_rhs_3, dataFormat_3);

	void* x_x_3 = nullptr;
	xDataInit((void**)&x_x_3);
	xDataSetData(x_x_3, x_3);
	xDataSetSize(x_x_3, size_3);
	xDataSetDataFormat(x_x_3, dataFormat_3);

	vrhs.push_back(x_rhs_1);
	vrhs.push_back(x_rhs_2);
	vrhs.push_back(x_rhs_3);
	vx.push_back(x_x_1);
	vx.push_back(x_x_2);
	vx.push_back(x_x_3);


	CALLBACK* callBack = new CALLBACK();
	callBack->setSize(3);
	callBack->setDataFormat(FORMAT_ARRAY_XDATA);

	ConjugateGradient* cg = new ConjugateGradient();
	cg->setProcessingMode(PROCESSING_MODE_CPU);
	cg->setDataFormat(FORMAT_ARRAY_XDATA);
	cg->setNbIter(100);
	cg->setSize(3);
	cg->setRhs((void*)&vrhs);
	cg->setX((void*)&vx);
	cg->setCallback(callBack);
	cg->run();

	UtilDisplay::display((void*)&vx, 3, FORMAT_ARRAY_XDATA);
	fprintf(stderr, "conjugate gradient ok\n");
	return 0;
}



int runConjugateGradient_1(int argc, char** argv)
{
	double rhs[] = { 4880.0, 6344.0, 5504.0 };
	double x[] = { 0.0, 0.0, 0.0 };
	int dataFormat = FORMAT_FLOAT64;

	CALLBACK* callBack = new CALLBACK();
	callBack->setSize(3);
	callBack->setDataFormat(dataFormat);

	ConjugateGradient* cg = new ConjugateGradient();
	cg->setProcessingMode(PROCESSING_MODE_CPU);
	cg->setDataFormat(dataFormat);
	cg->setNbIter(100);
	cg->setSize(3);
	cg->setRhs((void*)rhs);
	cg->setX((void*)x);
	cg->setCallback(callBack);
	cg->run();

	UtilDisplay::display((void*)x, 3, dataFormat);
	fprintf(stderr, "conjugate gradient ok\n");
	return 0;
}