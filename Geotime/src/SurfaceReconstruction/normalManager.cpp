
#include <utils.h>

#include <normalManagerCPU.h>
#include <normalManager.h>

typedef struct _NORMALMANAGERPARAM
{
	void* p;
}NORMALMANAGERPARAM;


typedef struct _NORMALMANAGER
{
	void* dipxy;
	void* dipxz;
	int dipInFormat;
	int dipOutFormat;
	int dataInFormat;
	int size[3];
	void* dataIn;
	void* outxy;
	void* outxz;
	int processingMode;
	NORMALMANAGERPARAM* param;
}NORMALMANAGER;


static int normalManagerParamCreate(NORMALMANAGER* p, NORMALMANAGERPARAM** param)
{
	if (p == nullptr || param == nullptr)return FAIL;
	if (callocSafe((void**)param, 1, sizeof(NORMALMANAGERPARAM)) != CALLOC_SUCCESS) return FAIL;
	if (p->processingMode == PROCESSING_MODE_CPU)
	{
		normalManagerCPUInit(&p->param->p);
		normalManagerCPUSetSize(p->param->p, p->size);
		normalManagerCPUSetDip(p->param->p, p->dipxy, p->dipxz);
		normalManagerCPUSetDipInDataFormat(p->param->p, p->dipInFormat);
		normalManagerCPUSetDipOutDataFormat(p->param->p, p->dipOutFormat);
		normalManagerCPUSetDataIn(p->param->p, p->dataIn);
		normalManagerSetDataOut(p->param->p, p->outxy, p->outxz);
	}
	else
	{
	}
}

int normalManagerInit(void** p)
{
	if (callocSafe((void**)p, 1, sizeof(NORMALMANAGER)) != CALLOC_SUCCESS) return FAIL;

	return SUCCESS;
}

int normalManagerRelease(void** p)
{
	return SUCCESS;
}

int normalManagerSetSize(void* p, int* size)
{
	if (p == nullptr) return FAIL;
	for (int i = 0; i < 3; i++)
		((NORMALMANAGER*)p)->size[i] = size[i];
	return SUCCESS;
}

int normalManagerSetProcessingMode(void* p, int processingMode)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->processingMode = processingMode;
	return SUCCESS;
}


int normalManagerSetDip(void* p, void* dipxy, void* dipxz)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->dipxy = dipxy;
	((NORMALMANAGER*)p)->dipxz = dipxz;
	return SUCCESS;
}

int normalManagerSetDipInDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->dipInFormat = dataFormat;
	return SUCCESS;
}

int normalManagerSetDipOutDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->dipOutFormat = dataFormat;
	return SUCCESS;
}

int normalManagerSetDataIn(void* p, void* dataIn)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->dataIn = dataIn;
	return SUCCESS;
}

int normalManagerSetDataOut(void* p, void* outxy, void *outxz)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->outxy = outxy;
	((NORMALMANAGER*)p)->outxz = outxz;
	return SUCCESS;
}

int normalManagerSetDataInFormat(void* p, int format)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGER*)p)->dataInFormat = format;
}

int normalManagerRun(void* _p)
{
	if (_p == nullptr) return FAIL;
	NORMALMANAGER* p = (NORMALMANAGER*)_p;
	if (p->param == nullptr)  normalManagerParamCreate(p, &p->param);
	if (p->processingMode == PROCESSING_MODE_CPU)
	{
		normalManagerCPURun(p->param->p);
	}
	else
	{
	}

	return SUCCESS;
}
