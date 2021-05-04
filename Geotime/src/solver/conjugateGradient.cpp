
#include <utils.h>
#include <conjugateGradient.h>

ConjugateGradient::ConjugateGradient()
{
	setNbIter(10);
	setSize(0);
	setCallback(nullptr);
	setDataFormat(FORMAT_FLOAT32);
	setProcessingMode(PROCESSING_MODE_CPU);
	pCG = nullptr;
}

ConjugateGradient::~ConjugateGradient()
{

}


int ConjugateGradient::setProcessingMode(int processingMode)
{
	this->processingMode = processingMode;
	return SUCCESS;
}

int ConjugateGradient::setDataFormat(int dataFormat)
{
	this->dataFormat = dataFormat;
	return SUCCESS;
}

int ConjugateGradient::setNbIter(long nbIter)
{
	this->nbIter = nbIter;
	return SUCCESS;
}

int ConjugateGradient::setSize(long size)
{
	this->size0 = size;
	return SUCCESS;
}


int ConjugateGradient::setRhs(void* rhs)
{
	this->rhs = rhs;
	return SUCCESS;
}

int ConjugateGradient::setX(void* x)
{
	this->x = x;
	return SUCCESS;
}

int ConjugateGradient::setCallback(ConjugateGradientCallBack* callBack)
{
	this->callBack = callBack;
	return SUCCESS;
}


int ConjugateGradient::initParamCPU()
{
	pCG = new ConjugateGradientCPU();
	((ConjugateGradientCPU*)pCG)->setDataFormat(dataFormat);
	((ConjugateGradientCPU*)pCG)->setNbIter(nbIter);
	((ConjugateGradientCPU*)pCG)->setRhs(rhs);
	((ConjugateGradientCPU*)pCG)->setSize(size0);
	((ConjugateGradientCPU*)pCG)->setX(x);
	((ConjugateGradientCPU*)pCG)->setCallback(callBack);
	return SUCCESS;
}

int ConjugateGradient::initParamGPU()
{
	return SUCCESS;
}


int ConjugateGradient::initParam()
{
	if (pCG != nullptr) return SUCCESS;
	if (processingMode == PROCESSING_MODE_CPU) return initParamCPU();
	if (processingMode == PROCESSING_MODE_GPU) return initParamGPU();
}

int ConjugateGradient::run()
{
	if (initParam() == FAIL) return FAIL;
	if (processingMode == PROCESSING_MODE_CPU) return ((ConjugateGradientCPU*)pCG)->run();

	return SUCCESS;
}

