
#include <vector>
#include <utils.h>
#include <xData.h>

#include <OpMalloc_CPU.h>



int OpMallocLike(void** ptr, void* model, size_t size0,  int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_FLOAT32:
		callocSafe(ptr, size0, sizeof(float));
		return SUCCESS;
		break;
	case FORMAT_FLOAT64:
		callocSafe(ptr, size0, sizeof(double));
		return SUCCESS;
		break;

	case FORMAT_XDATA:
	{
		void* data;
		int* size = xDataGetSize(model);
		xDataInit(ptr);
		OpMallocLike(&data, nullptr, (size_t)size[0] * size[1] * size[2], xDataGetDataFormat(model));
		xDataSetData(*ptr, data);
		xDataSetDataFormat(*ptr, xDataGetDataFormat(model));
		xDataSetSize(*ptr, size);
		return SUCCESS;
		break;
	}
	case FORMAT_ARRAY_XDATA:
	{
		*ptr = new std::vector<void*>();
		std::vector<void*>* vmodel = (std::vector<void*>*)model;
		for (int i = 0; i < vmodel->size(); i++)
		{
			void* data = nullptr;
			OpMallocLike(&data, (*vmodel)[i], 0, FORMAT_XDATA);
			((std::vector<void*>*)(*ptr))->push_back(data);
		}
		return SUCCESS;
		break;
	}
	}

}


