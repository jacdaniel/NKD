
#include <stdio.h>
#include <string.h>
<<<<<<< HEAD
#include <utils.h>
=======
#include <vector>
#include <utils.h>
#include <xData.h>
>>>>>>> e381ca5... Initial commit
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
<<<<<<< HEAD
private:
	size_t size0;
	int* size;
=======
	void setDataFormat(int dataFormat);

private:
	size_t size0;
	int* size;
	int dataFormat;
>>>>>>> e381ca5... Initial commit
};
CALLBACK::CALLBACK(){}
CALLBACK::~CALLBACK() {}

<<<<<<< HEAD
int CALLBACK::f(void* in, void* out)
{
	double* pin = (double*)in;
	double* pout = (double*)out;

	pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
	pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
	pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];

=======
static void genericCallBack(void* f, int size, void* g, int dataFormat)
{
	if (dataFormat == FORMAT_FLOAT32)
	{
		float* pin = (float*)f;
		float* pout = (float*)g;
		pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
		pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
		pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];
	}
	else
	{
		double* pin = (double*)f;
		double* pout = (double*)g;
		pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
		pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
		pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];
	}
}

static void genericPreconditionner(void* f, int size, void* g, int dataFormat)
{
	if (dataFormat == FORMAT_FLOAT32)
	{
		memcpy(g, f, size * sizeof(float));
	}
	else
	{
		memcpy(g, f, size * sizeof(double));
	}
}


int CALLBACK::f(void* in, void* out)
{
	if (dataFormat == FORMAT_FLOAT32 || dataFormat == FORMAT_FLOAT64)
	{
		genericCallBack(in, size0, out, dataFormat);
	}
	else if (dataFormat == FORMAT_XDATA)
	{
		void* f = xDataGetData(in);
		void* g = xDataGetData(out);
		int size = xDataGetSize0(in);
		int dataFormat = xDataGetDataFormat(in);
		genericCallBack(f, size, g, dataFormat);
	}
>>>>>>> e381ca5... Initial commit
	return SUCCESS;
}

int CALLBACK::Preconditionner(void* in, void* out)
{
<<<<<<< HEAD
	memcpy(out, in, size0 * sizeof(double));
=======
	if (dataFormat == FORMAT_FLOAT32 || dataFormat == FORMAT_FLOAT64)
	{
		genericPreconditionner(in, size0, out, dataFormat);
	}
	else if (dataFormat == FORMAT_XDATA)
	{
		void* f = xDataGetData(in);
		void* g = xDataGetData(out);
		int size = xDataGetSize0(in);
		int dataFormat = xDataGetDataFormat(in);
		genericPreconditionner(f, size, g, dataFormat);
	}
>>>>>>> e381ca5... Initial commit
	return SUCCESS;
}

void CALLBACK::setSize(size_t size)
{
	this->size0 = size;
}

<<<<<<< HEAD




int runConjugateGradient(int argc, char** argv)
{
=======
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


	double rhs[] = { 4880.0, 6344.0, 5504.0 };
	double x[] = { 0.0, 0.0, 0.0 };
	int size[] = { 3, 1, 1 };
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



int runConjugateGradient0(int argc, char** argv)
{
>>>>>>> e381ca5... Initial commit
	double rhs[] = { 4880.0, 6344.0, 5504.0 };
	double x[] = { 0.0, 0.0, 0.0 };
	int dataFormat = FORMAT_FLOAT64;

	CALLBACK* callBack = new CALLBACK();
	callBack->setSize(3);

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