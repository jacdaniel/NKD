#ifndef __SURFACESTACKING__
#define __SURFACESTACKING__

class SurfaceStacking
{
	class PARAM
	{
	public:
		PARAM();
		~PARAM();
		int* xc;
		void* vc;
	};

private:
	int size[3];
	int stackDataFormat;
	int surfaceDataFormat;
	void* stack;
	void* surface;
	PARAM* param;

	void initParam();
	int surfaceStackingInitValues();
	bool isValidStacking();
	int surfaceStakingSetValues();

	template<typename Tsurface, typename Tstack> int surfaceStackingInitValuesTemplate();
	template<typename T> bool isValidStackingTemplate(T vmax);
	template<typename T> int surfaceStakingSetValuesTemplate();




public:
	SurfaceStacking();
	~SurfaceStacking();
	int setSize(int* size);
	int setStack(void *stack);
	int setStackFormat(int dataFormat);
	int setSurface(void* surface);
	int setSurfaceFormat(int dataFormat);
	int run();
};



#endif