
#include <utils.h>
#include <vector>
#include <xData.h>

typedef struct _XDATA
{
	void* x;
	int dataFormat;
	int size[3];
	size_t size0;
}XDATA;

int xDataInit(void **p)
{
	if (p == nullptr) return FAIL;
	callocSafe(p, 1, sizeof(XDATA));
	return SUCCESS;
}

int xDataRelease(void** p)
{

	return SUCCESS;
}

int xDataSetSize(void* p, int* size)
{
	if (p == nullptr) return FAIL;
	((XDATA*)p)->size0 = 1;
	for (int i = 0; i < 3; i++)
	{
		((XDATA*)p)->size[i] = size[i];
		((XDATA*)p)->size0 *= size[i];
	}
	return SUCCESS;
}

int xDataSetData(void* p, void* data)
{
	if (p == nullptr) return FAIL;
	((XDATA*)p)->x = data;
	return SUCCESS;
}

int xDataSetDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((XDATA*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

//int xDataSetDataVector(void* p, std::vector<void*>* d)
//{
//	if (p == nullptr) return FAIL;
//	((XDATA*)p)->x = d;
//	((XDATA*)p)->dataFormat = FORMAT_ARRAY_XDATA;
//	return SUCCESS;
//}

int xDataDataMalloc(void* p)
{

	return SUCCESS;
}

int xDataDataFree(void* p)
{

	return SUCCESS;
}

int xDataClear(void* p)
{

	return SUCCESS;
}

void* xDataGetData(void* p)
{
	if (p == nullptr) return nullptr;
	return ((XDATA*)p)->x;
}

int* xDataGetSize(void* p)
{
	if (p == nullptr) return nullptr;
	return ((XDATA*)p)->size;
}

size_t xDataGetSize0(void* p) {
	if (p == nullptr) return 0;
	return ((XDATA*)p)->size0;
}

int xDataGetDataFormat(void* p)
{
	if (p == nullptr) return FORMAT_ERROR;
	return ((XDATA*)p)->dataFormat;
}

// V E C T O R

//
//int xDataGetLength(void* _p)
//{
//	if (_p == nullptr) return 0;
//	XDATA* p = (XDATA*)_p;
//	if (p->dataFormat == FORMAT_XDATA) return 1;
//	if (p->dataFormat == FORMAT_ARRAY_XDATA)
//	{
//		std::vector<void*>* pp = (std::vector<void*> *)p;
//		return pp->size();
//	}
//	return 0;
//}


