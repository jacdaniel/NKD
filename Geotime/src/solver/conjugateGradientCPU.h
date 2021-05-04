#ifndef __CONJUGATEGRADIENTCPU__
#define __CONJUGATEGRADIENTCPU__

#include <conjugateGradientCallBack.h>

class ConjugateGradientCPU
{
public:
	ConjugateGradientCPU();
	~ConjugateGradientCPU();

	int setDataFormat(int dataFormat);
	int setNbIter(long nbIter);
	int setSize(long size);
	int setRhs(void* rhs);
	int setX(void* x);
	int setCallback(ConjugateGradientCallBack* callBack);
	int run();

private:

	class PARAM
	{
	public:
		PARAM();
		~PARAM();
		PARAM(void *x, long size0, int dataFormat);
		void* z;
		void* tmp;
		void* r;
		void* d;
	};


	int dataFormat;
	long nbIter, size0;
	void* rhs, * x;
	ConjugateGradientCallBack* callBack;
	double dot(void* f, long size, void* g);
	PARAM* param;

	int paramInit();

};



#endif