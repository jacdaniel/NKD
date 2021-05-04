
#include <math.h>
#include <utils.h>

#include <normalManager.h>

typedef struct _NORMALMANAGERCPUPARAM
{
	void* p;
}NORMALMANAGERCPUPARAM;


typedef struct _NORMALMANAGERCPU
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
	NORMALMANAGERCPUPARAM* param;
}NORMALMANAGERCPU;


static int normalManagerCPUParamCreate(NORMALMANAGERCPU* p, _NORMALMANAGERCPUPARAM** param)
{
	if (p == nullptr || param == nullptr)return FAIL;
	if (callocSafe((void**)param, 1, sizeof(NORMALMANAGERCPUPARAM)) != CALLOC_SUCCESS) return FAIL;
	return SUCCESS;
}

int normalManagerCPUInit(void** p)
{
	if (callocSafe((void**)p, 1, sizeof(NORMALMANAGERCPU)) != CALLOC_SUCCESS) return FAIL;

	return SUCCESS;
}

int normalManagerCPURelease(void** p)
{
	return SUCCESS;
}

int normalManagerCPUSetSize(void* p, int* size)
{
	if (p == nullptr) return FAIL;
	for (int i = 0; i < 3; i++)
		((NORMALMANAGERCPU*)p)->size[i] = size[i];
	return SUCCESS;
}

int normalManagerCPUSetDip(void* p, void* dipxy, void* dipxz)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->dipxy = dipxy;
	((NORMALMANAGERCPU*)p)->dipxz = dipxz;
	return SUCCESS;
}

int normalManagerCPUSetDipInDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->dipInFormat = dataFormat;
	return SUCCESS;
}

int normalManagerCPUSetDipOutDataFormat(void* p, int dataFormat)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->dipOutFormat = dataFormat;
	return SUCCESS;
}

int normalManagerCPUSetDataIn(void* p, void* dataIn)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->dataIn = dataIn;
	return SUCCESS;
}

int normalManagerCPUSetDataOut(void* p, void* outxy, void* outxz)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->outxy = outxy;
	((NORMALMANAGERCPU*)p)->outxz = outxz;
	return SUCCESS;
}

int normalManagerCPUSetDataInFormat(void* p, int format)
{
	if (p == nullptr) return FAIL;
	((NORMALMANAGERCPU*)p)->dataInFormat = format;
}


template <typename Tdip, typename Ttau, typename Tout> static int normalManagerGetDips(Tdip *dip, int *size, Ttau *tau, Tout *out, double norm)
{
	for (int y = 0; y < size[1]; y++)
	{
		for (int z = 0; z < size[2]; z++)
		{
			long xf = (long)floor(tau[(long)size[1] * z + y]);
			xf = MIN(MAX(xf, 0), size[0]-1);
			out[(long)size[1] * z + y] = (Tout)dip[(long)z * size[0] * size[1] + y * size[0] + xf];
		}
	}
	if (norm != 1.0)
	{
		for (long add = 0; add < (long)size[1] * size[2]; add++)
		{
			out[add] /= norm;
		}
	}
	return SUCCESS;
}



int normalManagerCPURun(void* _p)
{
	if (_p == nullptr) return FAIL;
	NORMALMANAGERCPU* p = (NORMALMANAGERCPU*)_p;
	if (p->param == nullptr) normalManagerCPUParamCreate(p, &p->param);

	normalManagerGetDips<short, float, short>((short*)p->dipxy, p->size, (float*)p->dataIn, (short*)p->outxy, 1.0);



	return SUCCESS;
}