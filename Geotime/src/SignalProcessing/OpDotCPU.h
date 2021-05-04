#ifndef __OPDOTCPU__
#define __OPDOTCPU__

class OpDotCPU
{
public:
	OpDotCPU();
	virtual ~OpDotCPU();

private:
	template <typename T> static double dot(T* f, long size, T* g);

public:
	static double dot(float* f, long size, float* g);
	static double dot(double* f, long size, double* g);

	static double dot(void* f, long size, void* g, int dataFormat);

};


#endif