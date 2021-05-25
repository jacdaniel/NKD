
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

SurfaceStacking::PARAM::PARAM()
{

}

SurfaceStacking::PARAM::~PARAM()
{

}


SurfaceStacking::SurfaceStacking()
{
	param = nullptr;
}

SurfaceStacking::~SurfaceStacking()
{
}

int SurfaceStacking::setSize(int* size)
{	
	for (int i = 0; i < 3; i++)
		this->size[i] = size[i];
	return SUCCESS;
}

int SurfaceStacking::setStack(void* stack)
{
	this->stack = stack;
	return SUCCESS;
}

int SurfaceStacking::setStackFormat(int dataFormat)
{
	this->stackDataFormat = dataFormat;
	return SUCCESS;
}

int SurfaceStacking::setSurface(void* surface)
{
	this->surface = surface;
	return SUCCESS;
}

int SurfaceStacking::setSurfaceFormat(int dataFormat)
{
	this->surfaceDataFormat = dataFormat;
	return SUCCESS;
}


void SurfaceStacking::initParam()
{
	size_t size0 = size[1] * size[2];
	this->param = new PARAM();
	this->param->xc = new int[size0];
	switch (stackDataFormat)
	{
	case FORMAT_INT16: param->vc = new short[size0]; break;
	case FORMAT_FLOAT32: param->vc = new float[size0]; break;
	}
}


// TODO

template<typename Tsurface, typename Tstack> int SurfaceStacking::surfaceStackingInitValuesTemplate()
{
	for (int z = 0; z < size[2]; z++)
		for (int y = 0; y < size[1]; y++)
		{
			int x0 = (int)floor(((Tsurface*)surface)[z * size[1] + y] + .5);
			x0 = MIN(MAX(x0, 0), size[0] - 1);
			param->xc[z * size[1] + y] = x0;
			((Tstack*)param->vc)[z * size[1] + y] = ((Tstack*)stack)[z * size[0] * size[1] + y * size[0] + x0];
		}
	return SUCCESS;
}

int SurfaceStacking::surfaceStackingInitValues()
{
	switch (surfaceDataFormat)
	{
		case FORMAT_INT16: 
			if ( stackDataFormat == FORMAT_INT16 ) surfaceStackingInitValuesTemplate<short, short>();
			else if (stackDataFormat == FORMAT_FLOAT32 ) surfaceStackingInitValuesTemplate<short, float>();
			break;
		case FORMAT_FLOAT32: 
			if (stackDataFormat == FORMAT_INT16) surfaceStackingInitValuesTemplate<float, short>();
			else if (stackDataFormat == FORMAT_FLOAT32) surfaceStackingInitValuesTemplate<float, float>();
			break;
		case FORMAT_FLOAT64:
			if (stackDataFormat == FORMAT_INT16) surfaceStackingInitValuesTemplate<double, short>();
			else if (stackDataFormat == FORMAT_FLOAT32) surfaceStackingInitValuesTemplate<double, float>();
			break;
	}	
	return SUCCESS;
}

template<typename T> bool SurfaceStacking::isValidStackingTemplate(T vmax)
{
	size_t size00 = (size_t)size[1] * size[2];
	for (size_t add = 0; add < size00; add++)
	{
		if (((T*)param->vc)[add] >= vmax) return false;
	}
	return true;
}

bool SurfaceStacking::isValidStacking()
{
	switch (stackDataFormat)
	{
	case FORMAT_INT16: return isValidStackingTemplate<short>(32000); break;
	case FORMAT_FLOAT32:
	case FORMAT_FLOAT64:
		return true;
		break;
	}
	return false;
}

template<typename T> int SurfaceStacking::surfaceStakingSetValuesTemplate()
{
	for (int z = 0; z < size[2]; z++)
		for (int y = 0; y < size[1]; y++)
		{
			size_t add0 = z * size[1] + y;
			((T*)stack)[add0 * size[0] + param->xc[add0]] = ((T*)param->vc)[add0] + (T)1;
		}
	return SUCCESS;
}

int SurfaceStacking::surfaceStakingSetValues()
{
	switch (stackDataFormat)
	{
	case FORMAT_INT16:
		surfaceStakingSetValuesTemplate<short>();
		break;
	case FORMAT_FLOAT32:
		surfaceStakingSetValuesTemplate<float>();
		break;
	}
	return SUCCESS;
}



int SurfaceStacking::run()
{
	if (param == nullptr) initParam();
	surfaceStackingInitValues();
	if (isValidStacking())
	{
		surfaceStakingSetValues();
		return SUCCESS;
	}
	return FAIL;
}





