#include <string.h>
#include <math.h>
#include <vector>
#include <utils.h>
#include <xData.h>
#include <utilDisplay.h>
#include <OpDotCPU.h>
#include <OpMemCpyCPU.h>
#include <arithmetic.h>
#include <OpMalloc_CPU.h>
#include <conjugateGradientCPU.h>

ConjugateGradientCPU::PARAM::PARAM()
{

}


ConjugateGradientCPU::PARAM::~PARAM()
{

}


ConjugateGradientCPU::PARAM::PARAM(void *x, long size0, int dataFormat)
{
	OpMallocLike(&z, x, size0, dataFormat);
	tmp = z;
	OpMallocLike(&r, x, size0, dataFormat);
	OpMallocLike(&d, x, size0, dataFormat);
}


ConjugateGradientCPU::ConjugateGradientCPU()
{
	setDataFormat(FORMAT_FLOAT32);
	setNbIter(10);
	setSize(0);
	setRhs(nullptr);
	setX(nullptr);
	setCallback(nullptr);
	param = nullptr;
}

ConjugateGradientCPU::~ConjugateGradientCPU()
{
}

int ConjugateGradientCPU::setDataFormat(int dataFormat)
{
	this->dataFormat = dataFormat;
	return SUCCESS;
}

int ConjugateGradientCPU::setNbIter(long nbIter)
{
	this->nbIter = nbIter;
	return SUCCESS;
}

int ConjugateGradientCPU::setSize(long size)
{
	this->size0 = size;
	return SUCCESS;
}

int ConjugateGradientCPU::setRhs(void* rhs)
{
	this->rhs = rhs;
	return SUCCESS;
}

int ConjugateGradientCPU::setX(void* x)
{
	this->x = x;
	return SUCCESS;
}

int ConjugateGradientCPU::setCallback(ConjugateGradientCallBack* callBack)
{
	this->callBack = callBack;
	return SUCCESS;
}


double ConjugateGradientCPU::dot(void* f, size_t size, void* g)
{
	return OpDotCPU::dot(f, size, g, dataFormat);
	return 0.0;
}


int ConjugateGradientCPU::paramInit()
{
	param = new PARAM(x, size0, dataFormat);
	return SUCCESS;
}

int ConjugateGradientCPU::run()
{
	int cont = 1;
	int iter = 0;
	double db = 0.0;
	double rho0 = 0.0;
	double rho1 = 0.0;
	double beta = 0.0;
	double denom = 1.0;
	double alphax = 1.0;

	if (param == nullptr) paramInit();
	
	db = dot(rhs, size0, rhs);
	opMemCpy(param->r, rhs, size0, dataFormat);
	callBack->f(x, param->tmp);
	Arithmetic_YEqualsAMinusB(param->r, size0, param->r, param->tmp, dataFormat);
	callBack->Preconditionner(param->r, param->z);
	rho0 = dot(param->r, size0, param->z);

	while (cont)
	{
		if (iter == 0)
		{
			opMemCpy(param->d, param->z, size0, dataFormat);
		}
		else
		{
			beta = rho0 / rho1;
			Arithmetic_YEqualsXPlusAlphaTimesY_CPU(param->d, size0, param->z, beta, dataFormat);
		}

		callBack->f(param->d, param->tmp);
		denom = dot(param->d, size0, param->tmp);
		alphax = rho0 / denom;

		if (fabs(alphax) < 1e6 && fabs(alphax) > 1e-6)
		{
			Arithmetic_YPlusEqualsAlphaTimesX_CPU(x, size0, alphax, param->d, dataFormat);
			Arithmetic_YMinusEqualsAlphaTimesX_CPU(param->r, size0, alphax, param->tmp, dataFormat);
			callBack->Preconditionner(param->r, param->z);
			rho1 = rho0;
			rho0 = dot(param->r, size0, param->z);
		}
		else
		{
			cont = 0;
		}
		
		iter++;
		if (iter >= nbIter) cont = 0;
	}
	return SUCCESS;
}






