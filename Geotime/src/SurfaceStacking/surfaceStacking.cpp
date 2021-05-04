
#include <math.h>

#include <utils.h>
#include <surfaceStacking.h>

typedef struct _SURFACESTACKINGPARAM
{
	int* xc;
	short* v;
}SURFACESTACKINGPARAM;


typedef struct _SURFACESTACKING
{
	void* stack;
	void* surface;
	int size[3];
	int stackFormat;
	int surfaceFormat;
	SURFACESTACKINGPARAM *param;
}SURFACESTACKING;

int surfaceStackingInit(void** p)
{
	if (p == nullptr) return FAIL;
	if (callocSafe(p, 1, sizeof(SURFACESTACKING)) != CALLOC_SUCCESS) return FAIL;

	return SUCCESS;
}

int surfaceStackingRelease(void** _p)
{
	return SUCCESS;
}

int surfaceStackingSetSize(void* _p, int* size)
{
	if (_p == nullptr) return FAIL;
	for (int i = 0; i < 3; i++)
		((SURFACESTACKING*)_p)->size[i] = size[i];
	return SUCCESS;
}

int surfaceStackingSetStack(void* p, void* stack)
{
	if (p == nullptr) return FAIL;
	((SURFACESTACKING*)p)->stack = stack;
	return SUCCESS;
}

int surfaceStackingSetStackFormat(void* _p, int format)
{
	if (_p == nullptr) return FAIL;
	((SURFACESTACKING*)_p)->stackFormat = format;
	return SUCCESS;
}

int surfaceStackingSetSurface(void* _p, void* surface)
{
	if (_p == nullptr) return FAIL;
	((SURFACESTACKING*)_p)->surface = surface;
	return SUCCESS;
}

int surfaceStackingSetSurfaceFormat(void* _p, int format)
{
	if (_p == nullptr) return FAIL;
	((SURFACESTACKING*)_p)->surfaceFormat = format;
	return SUCCESS;
}


static int surfaceStackingGetXc(short *stack, float *surface, int* size, int * xc, short *v)
{
	for (int z=0; z<size[2]; z++)
		for (int y = 0; y < size[1]; y++)
		{
			int x0 = surface[z * size[1] + y];
			x0 = MIN(MAX(x0, 0), size[0] - 1);
			xc[z * size[1] + y] = x0;
			v[z * size[1] + y] = stack[z * size[0] * size[1] + y * size[0] + x0];
		}
	return SUCCESS;
}

static int surfaceStackValidStacking(short* v, long size0)
{
	for (long add = 0; add < size0; add++)
	{
		if (v[add] > 32000) return FAIL;
		v[add] += 1;
	}
	return SUCCESS;
}

static int surfaceStackingSetXc(short* stack, int* size, int* xc, short* v)
{
	for (int z = 0; z < size[2]; z++)
		for (int y = 0; y < size[1]; y++)
			stack[z * size[0] * size[1] + y * size[0] + xc[z * size[1] + y]] = v[z * size[1] + y];
	return SUCCESS;
}

static int surfaceStackingStack(short* stack, int* size, float* surface, int *xc, short *v)
{
	surfaceStackingGetXc(stack, surface, size, xc, v);
	if (surfaceStackValidStacking(v, size[1] * size[2]) == FAIL)
		return FAIL;
	surfaceStackingSetXc(stack, size, xc, v);
	return SUCCESS;
}



int surfaceStackingRun(void* _p)
{
	SURFACESTACKING* p = (SURFACESTACKING*)_p;
	return surfaceStackingStack((short*)p->stack, p->size, (float*)p->surface, p->param->xc, p->param->v);
}


