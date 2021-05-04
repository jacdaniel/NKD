#ifndef __CONJUGATEGRADIENT__
#define __CONJUGATEGRADIENT__

#include <conjugateGradientCPU.h>
#include <conjugateGradientCallBack.h>

class ConjugateGradient
{
public:
	ConjugateGradient();
	~ConjugateGradient();

	int setProcessingMode(int processingMode);
	int setDataFormat(int dataFormat);
	int setNbIter(long nbIter);
	int setSize(long size);
	int setRhs(void* rhs);
	int setX(void* x);
	int setCallback(ConjugateGradientCallBack* callBack);
	int run();


private:
	int processingMode;
	int dataFormat;
	long nbIter, size0;
	void* rhs, * x;
	ConjugateGradientCallBack* callBack;
	void* pCG;

	int initParam();
	int initParamCPU();
	int initParamGPU();




	/*

private:
	long nbIter, size0;
	double* rhs, * x;

	std::vector<XData> Xrhs;
	std::vector<XData> Xx;

	CALLBACK* callback;
	double dot(double* v1, long size, double* v2);
	double dot(float* v1, long size, float* v2);
public:
	ConjugateGradient();
	~ConjugateGradient();

	void setCallback(void* callback, void* data);
	void setPreconditionner(void* f, void* data);
	void setSize(int size);
	void setRhs(void* rhs);
	void setX(void* x);
	void setNbiter(int nbiter);
	int run();
	int run2();

	double dot(std::vector<XData> v1, std::vector<XData> v2);
	void setRhs(std::vector<XData> rhs);
	void setX(std::vector<XData> x);

	PARAM* param;
	*/

};


#endif