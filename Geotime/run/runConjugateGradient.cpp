
#include <stdio.h>
#include <string.h>
#include <utils.h>
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
private:
	size_t size0;
	int* size;
};
CALLBACK::CALLBACK(){}
CALLBACK::~CALLBACK() {}

int CALLBACK::f(void* in, void* out)
{
	double* pin = (double*)in;
	double* pout = (double*)out;

	pout[0] = 283.0 * pin[0] + 368.0 * pin[1] + 318.0 * pin[2];
	pout[1] = 368.0 * pin[0] + 480.0 * pin[1] + 412.0 * pin[2];
	pout[2] = 318.0 * pin[0] + 412.0 * pin[1] + 362.0 * pin[2];

	return SUCCESS;
}

int CALLBACK::Preconditionner(void* in, void* out)
{
	memcpy(out, in, size0 * sizeof(double));
	return SUCCESS;
}

void CALLBACK::setSize(size_t size)
{
	this->size0 = size;
}





int runConjugateGradient(int argc, char** argv)
{
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