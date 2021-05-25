#ifndef __BLOCPROC__
#define __BLOCPROC__


class BlocProc
{
public:
	enum { COMPUTE_TYPE_SAME = 0, COMPUTE_TYPE_NO_SAME };
	enum { SIZE_TYPE_CONSTANT = 0, SIZE_TYPE_VARIABLE };
	enum { MIRROR_TYPE_I = 0 };

	class CallBack
	{
	public:
		CallBack() {}
		~CallBack() {}
		virtual int Process() { return FAIL; }
		virtual int dataRead(int* x1, int* x2, void* data) { return FAIL; }
		virtual int dataWrite(int *x1, int *x2, void* data) { return FAIL; }
	};

	class PARAM
	{
	public:
		int **x1;
		int **x2;
		int **xRead1;
		int **xRead2;
		int blockNbre[3];
		int border[3];
		size_t blockNbre0;
		size_t blocSize0;
		void* dataIn;
		void* dataInBloc;
		void* dataOut;
		void* cacheDataIn;
		int cacheDataInSize[3];
	};


	BlocProc();
	~BlocProc();

	int setDataSize(int* size);
	int setBorderSize(int* size);
	int setBlockSize(int* size);
	int setBorderType(int type);
	int setDataInFormat(int format);
	int setDataOutFormat(int format);
	int setCallBack(CallBack* callback);
	int setComputeType(int type);
	int setSizeTpe(int type);
	int setMirrorType(int type);

	int run();
	

private:
	int dataSize[3];
	int blocSize[3];
	int borderSize[3];
	int borderType;
	void* dataIn;
	void* dataOut;
	int dataInFormat;
	int dataOutFormat;
	int computeType;
	int sizeType;
	int mirrorType;
	CallBack *callback;
	PARAM* param;
	int paramInit();

	int xLimitInit(long size, long x0, long Nu, int* x1, int* x2);
	int xReadinit(int x1, int x2, int size, int border, int* xRead1, int* xRead2);

	int debugPrintParam();

};



#endif