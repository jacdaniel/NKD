
#include <fftw3.h>

#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>
#include <laplacian.h>
#include <poissonFourierCPU.h>

typedef struct _POISSONFOURIERCPUPARAM
{
	void* fftPlanDirect;
	void* fftPlanInverse;
	void * spatialIn;
	void * spatialOut;
	void * freq;
	double fftwNorm;
	long size0;
	long memSize0;
	int dim;

	double hx, hz;
	long transform;

	void * invLap;

	long width_poisson;
	long depth_poisson;
	long size_poisson;
	int threads_nbre;
}POISSONFOURIERCPUPARAM;

typedef struct _POISSONFOURIERCPU
{
	int type;
	int size[3];
	double mu, lambda;
	void *dataIn, *dataOut, *u, *boundary;
	int processingMode;
	int dataFormat;
	int nbThreads;
	POISSONFOURIERCPUPARAM *param;

}POISSONFOURIERCPU;


// P A R A M
int poissonFourierCPUParamFloat32Create(POISSONFOURIERCPU* data, POISSONFOURIERCPUPARAM **param)
{
	if (data == nullptr) return FAIL;
	*param = (POISSONFOURIERCPUPARAM*)calloc(1, sizeof(POISSONFOURIERCPUPARAM));
	POISSONFOURIERCPUPARAM* pparam = *param;

	pparam->dim = getDimsFromSize(data->size);
	pparam->size0 = (long)data->size[0] * data->size[1] * data->size[2];
	pparam->memSize0 = pparam->size0 *sizeof(float);

	laplacianFourierCreate((void**)&(pparam->invLap), data->size, data->lambda, data->mu, data->dataFormat, PROCESSING_MODE_CPU);

	// 1 D
	if ( pparam->dim == _1D )
	{
		pparam->spatialIn = fftwf_malloc(pparam->memSize0);
		pparam->spatialOut = fftwf_malloc(pparam->memSize0);
		pparam->freq = fftwf_malloc(pparam->memSize0);

		pparam->fftPlanDirect = fftwf_plan_r2r_1d(data->size[0], (float*)pparam->spatialIn, (float*)pparam->freq, FFTW_REDFT10, FFTW_ESTIMATE);
		pparam->fftPlanInverse = fftwf_plan_r2r_1d(data->size[0], (float*)pparam->freq, (float*)pparam->spatialOut, FFTW_REDFT01, FFTW_ESTIMATE);
		pparam->fftwNorm = 2.0 * data->size[0];
	}


	return SUCCESS;
}

int poissonFourierCPUParamFloat64Create(POISSONFOURIERCPU* data, POISSONFOURIERCPUPARAM** param)
{
	if (data == nullptr) return FAIL;
	*param = (POISSONFOURIERCPUPARAM*)calloc(1, sizeof(POISSONFOURIERCPUPARAM));
	POISSONFOURIERCPUPARAM* pparam = *param;

	pparam->dim = getDimsFromSize(data->size);
	pparam->size0 = (long)data->size[0] * data->size[1] * data->size[2];
	pparam->memSize0 = pparam->size0 * sizeof(double);

	laplacianFourierCreate((void**)&(pparam->invLap), data->size, data->lambda, data->mu, data->dataFormat, PROCESSING_MODE_CPU);

	// 1D
	if (pparam->dim == _1D)
	{
		pparam->spatialIn = fftw_malloc(pparam->memSize0);
		pparam->spatialOut = fftw_malloc(pparam->memSize0);
		pparam->freq = fftw_malloc(pparam->memSize0);

		pparam->fftPlanDirect = fftw_plan_r2r_1d(data->size[0], (double*)pparam->spatialIn, (double*)pparam->freq, FFTW_REDFT10, FFTW_ESTIMATE);
		pparam->fftPlanInverse = fftw_plan_r2r_1d(data->size[0], (double*)pparam->freq, (double*)pparam->spatialOut, FFTW_REDFT01, FFTW_ESTIMATE);
		pparam->fftwNorm = 2.0 * data->size[0];
	}


	return SUCCESS;
}

int poissonFourierCPUParamCreate(POISSONFOURIERCPU* data, POISSONFOURIERCPUPARAM** param)
{
	if (data->dataFormat == FORMAT_FLOAT32) return poissonFourierCPUParamFloat32Create(data, param);
	else if ( data->dataFormat == FORMAT_FLOAT64) return poissonFourierCPUParamFloat64Create(data, param);
}



int poissonFourierCPUInit(void **p)
{
	if ( p == nullptr ) return FAIL;
	callocSafe(p, 1, sizeof(POISSONFOURIERCPU));
	return SUCCESS;
}


int poissonFourierCPURelease(void **_p)
{
	if( _p == nullptr || *_p == nullptr ) return FAIL;
	POISSONFOURIERCPU *p = (POISSONFOURIERCPU*)(*_p);

	FREE(p)
	return SUCCESS;
}

int poissonFourierCPUSetDataFormat(void *p, int dataFormat)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIERCPU*)p)->dataFormat = dataFormat;
	return SUCCESS;
}

int poissonFourierCPUSetSize(void *p, int *size)
{
	if ( p == nullptr ) return FAIL;
	for (int i=0; i<3; i++)
	  ((POISSONFOURIERCPU*)p)->size[i] = size[i];
	return SUCCESS;
}

int poissonFourierCPUSetDataIn(void *p, void *dataIn)
{
	if( p == nullptr ) return FAIL;
	((POISSONFOURIERCPU*)p)->dataIn = dataIn;
	return SUCCESS;
}

int poissonFourierCPUSetDataOut(void *p, void *dataOut)
{
	if ( p == nullptr ) return FAIL;
	((POISSONFOURIERCPU*)p)->dataOut = dataOut;
	return SUCCESS;
}

int poissonFourierCPUSetNbreThreads(void *p, int nbThreads)
{
	if( p == nullptr ) return FAIL;
	((POISSONFOURIERCPU*)p)->nbThreads = nbThreads;
	return SUCCESS;
}



//
static int poissonFourierCPULaplacianApply(POISSONFOURIERCPU* p)
{
	if (p == nullptr) return FAIL;
	void* freq = p->param->freq;
	void* invLap = p->param->invLap;
	if (p->dataFormat == FORMAT_FLOAT32)
	{
		for (long i = 0; i < p->param->size0; i++)
		{
			((float*)freq)[i] *= ((float*)invLap)[i];
		}
	}
	else
	{
		for (long i = 0; i < p->param->size0; i++)
		{
			((double*)freq)[i] *= ((double*)invLap)[i];
		}
	}
}

static int poissonFourierCPUFourierNormalization(POISSONFOURIERCPU* p)
{
	if (p == nullptr) return FAIL;
	if (p->dataFormat == FORMAT_FLOAT32)
	{
		for (int i = 0; i < p->param->size0; i++)
			((float*)p->param->spatialOut)[i] /= (float)p->param->fftwNorm;
	}
	else
	{
		for (int i = 0; i < p->param->size0; i++)
			((double*)p->param->spatialOut)[i] /= p->param->fftwNorm;
	}
}



int poissonFourierCPURun(void* _p)
{
	POISSONFOURIERCPU* p = (POISSONFOURIERCPU*)_p;
	if (p == nullptr) return FAIL;
	if (p->param == nullptr) poissonFourierCPUParamCreate(p, &p->param);	
	memcpy(p->param->spatialIn, p->dataIn, p->param->memSize0);
	
	if (p->dataFormat == FORMAT_FLOAT32)
		fftwf_execute((fftwf_plan)p->param->fftPlanDirect);
	else
		fftw_execute((fftw_plan)p->param->fftPlanDirect);
	
	poissonFourierCPULaplacianApply(p);
	
	if (p->dataFormat == FORMAT_FLOAT32)
		fftwf_execute((fftwf_plan)p->param->fftPlanInverse);
	else
		fftw_execute((fftw_plan)p->param->fftPlanInverse);

	poissonFourierCPUFourierNormalization(p);
	memcpy(p->dataOut, p->param->spatialOut, p->param->memSize0);
	return SUCCESS;
}

