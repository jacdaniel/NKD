
#include <utils.h>
#include <BlockProc.h>


BlocProc::BlocProc()
{
	param = nullptr;
	setComputeType(COMPUTE_TYPE_SAME);
	setSizeTpe(SIZE_TYPE_CONSTANT);
	setMirrorType(MIRROR_TYPE_I);
}

BlocProc::~BlocProc()
{

}

int BlocProc::setDataSize(int* size)
{
	for (int i = 0; i < 3; i++)
		dataSize[i] = size[i];
	return SUCCESS;
}

int BlocProc::setBorderSize(int* size)
{
	for (int i = 0; i < 3; i++)
		borderSize[i] = size[i];
	return SUCCESS;
}


int BlocProc::setBlockSize(int* size)
{
	for (int i = 0; i < 3; i++)
		blocSize[i] = size[i];
	return SUCCESS;
}

int BlocProc::setBorderType(int type)
{
	borderType = type;
	return SUCCESS;
}


int BlocProc::setDataInFormat(int format)
{
	dataInFormat = format;
	return SUCCESS;
}

int BlocProc::setDataOutFormat(int format)
{
	dataOutFormat = format;
	return SUCCESS;
}

int BlocProc::setCallBack(CallBack* callback)
{
	this->callback = callback;
	return SUCCESS;
}

int BlocProc::setComputeType(int type)
{
	computeType = type;
	return SUCCESS;
}

int BlocProc::setSizeTpe(int type)
{
	sizeType = type;
	return SUCCESS;
}

int BlocProc::setMirrorType(int type)
{
	mirrorType = type;
	return SUCCESS;
}


int BlocProc::paramInit()
{
	int ret = newSafe((void**)& param, PARAM);
	param->blocSize0 = blocSize[0] * blocSize[1] * blocSize[2];
	for (int i = 0; i < 3; i++)
	{
		param->blockNbre[i] = (this->dataSize[i] - 1) / this->blocSize[i] + 1;
	}
	param->blockNbre0 = (size_t)param->blockNbre[0] * param->blockNbre[1] * param->blockNbre[2];
	callocSafe((void**)&param->x1, param->blockNbre0, sizeof(int*));
	callocSafe((void**)&param->x2, param->blockNbre0, sizeof(int*));
	callocSafe((void**)&param->xRead1, param->blockNbre0, sizeof(int*));
	callocSafe((void**)&param->xRead2, param->blockNbre0, sizeof(int*));
	for (int i = 0; i < param->blockNbre0; i++)
	{
		callocSafe((void**)&param->x1[i], 3, sizeof(int));
		callocSafe((void**)&param->x2[i], 3, sizeof(int));
		callocSafe((void**)&param->xRead1[i], 3, sizeof(int));
		callocSafe((void**)&param->xRead2[i], 3, sizeof(int));
	}

	size_t idx = 0;
	for (int z=0; z<this->param->blockNbre[2]; z++)
		for(int y=0; y< param->blockNbre[1]; y++)
			for (int x = 0; x < param->blockNbre[0]; x++)
			{
				xLimitInit(dataSize[0], x * blocSize[0], blocSize[0], &param->x1[idx][0], &param->x2[idx][0]);
				xLimitInit(dataSize[1], y * blocSize[1], blocSize[1], &param->x1[idx][1], &param->x2[idx][1]);
				xLimitInit(dataSize[2], z * blocSize[2], blocSize[2], &param->x1[idx][2], &param->x2[idx][2]);

				for (int j=0; j<3; j++)
					xReadinit(param->x1[idx][j], param->x2[idx][j], dataSize[j], borderSize[j], &param->xRead1[idx][j], &param->xRead2[idx][j]);
				idx++;
			}

	callocSafeFormat((void**)&param->dataIn, param->blocSize0, dataInFormat);
	callocSafeFormat((void**)&param->dataInBloc, param->blocSize0, dataInFormat);
	callocSafeFormat((void**)&param->dataOut, param->blocSize0, dataOutFormat);

	return SUCCESS;
}

int BlocProc::xLimitInit(long size, long x0, long Nu, int* x1, int* x2)
{
	*x1 = MIN(x0, size - 1);
	*x2 = MIN(x0 + Nu - 1, size - 1);
	return SUCCESS;
}

int BlocProc::xReadinit(int x1, int x2, int size, int border, int* xRead1, int* xRead2)
{
	*xRead1 = MAX(x1 - border, 0);
	*xRead2 = MIN(x2 + border, size - 1);
	return SUCCESS;
}


int BlocProc::run()
{
	if (param == nullptr) paramInit();
	debugPrintParam();
	int cont = 1;
	size_t idx = 0;
	int *x1, *x2, *xRead1, *xRead2;
	while (cont)
	{
		x1 = param->x1[idx];
		x2 = param->x2[idx];
		xRead1 = param->xRead1[idx];
		xRead2 = param->xRead2[idx];
		callback->dataRead(x1, x2, param->dataIn);


		idx++;
		if (idx >= param->blockNbre0) cont = 0;
	}





	return SUCCESS;
}


int BlocProc::debugPrintParam()
{
	for (int i = 0; i < param->blockNbre0; i++)
	{
		// fprintf(stderr, "%d: [%d %d] - [%d %d] - [%d %d] >>> ", i, param->x1[i][0], param->x2[i][0], param->x1[i][1], param->x2[i][1], param->x1[i][2], param->x2[i][2]);
		// fprintf(stderr, "[%d %d] - [%d %d] - [%d %d]\n", param->xRead1[i][0], param->xRead2[i][0], param->xRead1[i][1], param->xRead2[i][1], param->xRead1[i][2], param->xRead2[i][2]);

		callback->dataRead(param->x1[i], param->x2[i], param->dataIn);
	}
	return SUCCESS;
}


/*
param->nread_block[i] = (param->size_scale_max[i] - 1) / normal->block_size[i] + 1;

block_dim_cut_param2((long)normal->source_size[0], x0, (long)normal->block_size[0], (long)param->conv_border, &x1, &x2, &xread1, &xread2);

void block_dim_cut_param2(long size, long x0, long Nu, long border, long* x1, long* x2, long* xe1, long* xe2)
{
	*x1 = MIN(x0, size - 1);
	*x2 = MIN(x0 + Nu - 1, size - 1);
	*xe1 = MAX(*x1 - border, 0);
	*xe2 = MIN(*x2 + border, size - 1);
}

#define MODULOX2(x, m, res) {\
  res = x; \
  if ( res < 0 ) res = -res; \
  res = (res) % (2*(m)); \
  if ( res >= m ) res = 2 * (m) - res - 1; }

void block_data_mirror2(short* I0, int width0, int height0, int depth0,
	long xoffset, long yoffset, long zoffset,
	short* I1, int width1, int height1, int depth1)
{
	int xx, yy, zz;
	for (int z = 0; z < depth1; z++)
	{
		MODULOX2(z - zoffset, depth0, zz);
		for (int y = 0; y < height1; y++)
		{
			MODULOX2(y - yoffset, height0, yy);
			for (int x = 0; x < width1; x++)
			{
				MODULOX2(x - xoffset, width0, xx);
				I1[width1 * height1 * z + width1 * y + x] = I0[width0 * height0 * zz + width0 * yy + xx];
			}
		}
	}
}

*/