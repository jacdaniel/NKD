
#ifndef __CONJUGATEGRADIENTCALLBACK__
#define __CONJUGATEGRADIENTCALLBACK__

class ConjugateGradientCallBack
{
public:
	ConjugateGradientCallBack();
	~ConjugateGradientCallBack();

	virtual int f(void* in, void* out);
	virtual int Preconditionner(void* in, void* out);
	
};



#endif