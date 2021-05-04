#ifndef __UTILDISPLAY__
#define __UTILDISPLAY__

class UtilDisplay {
public:
	static void display(double* data, long nbre);
	static void display(float* data, long nbre);
	static void display(void* data, long size, int dataFormat);
};


#endif

