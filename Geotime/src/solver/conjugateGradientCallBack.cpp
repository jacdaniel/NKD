
#include <utils.h>
#include <conjugateGradientCallBack.h>


ConjugateGradientCallBack::ConjugateGradientCallBack()
{
}


ConjugateGradientCallBack::~ConjugateGradientCallBack()
{
}


int ConjugateGradientCallBack::f(void* in, void* out)
{
	return SUCCESS;
}


int ConjugateGradientCallBack::Preconditionner(void* in, void* out)
{

	return SUCCESS;
}
