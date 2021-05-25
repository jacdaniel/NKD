
#include <stdio.h>

#include <utils.h>
#include <BlockProc.h>

class CallBack : public BlocProc::CallBack 
{
public:
	virtual int Process();
	virtual int dataRead(int* x1, int* x2, void* data);
	virtual int dataWrite(int* x1, int* x2, void* data);
};

int CallBack::Process()
{ 
	
	return SUCCESS;
}

int CallBack::dataRead(int* x1, int* x2, void* data)
{ 
	fprintf(stderr, "data read [%d %d] - [%d %d] - [%d %d]\n", x1[0], x2[0], x1[1], x2[1], x1[2], x2[2]);
	return SUCCESS;
}

int CallBack::dataWrite(int* x1, int* x2, void* data)
{ 
	
	return SUCCESS;
}

int runBlockProc(int argc, char** argv)
{
	CallBack* c = nullptr;
	newSafe((void**)&c, CallBack);

	BlocProc* b = nullptr;
	newSafe((void**)&b, BlocProc);

	int dataSize[] = { 350, 2000, 400 };
	int borderSize[] = { 5, 5, 5 };
	int blockSize[] = { 128, 128, 128 };


	b->setCallBack(c);
	b->setDataSize(dataSize);
	b->setBorderSize(borderSize);
	b->setBlockSize(blockSize);
	b->setBorderType(0);
	b->setDataInFormat(FORMAT_INT16);
	b->setDataOutFormat(FORMAT_INT16);
	b->setCallBack(c);
	b->run();



	fprintf(stderr, "runBlockProc OK\n");
	return SUCCESS;
}