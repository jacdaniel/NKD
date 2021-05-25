
#include <utils.h>
#include <DataInsert.h>



template<typename Tbloc, typename Tdata> int DataInsert::insert(Tbloc* block, int* blockSize, Tdata* data, int* dataSize, int* x1)
{
	for (int z = 0; z < dataSize[2]; z++)
	{
		if (z + x1[2] < blockSize[2])
		{
			size_t offsetzBlock = (size_t)blockSize[0] * blockSize[1] * (z + x1[2]);
			size_t offsetzData = (size_t)dataSize[0] * dataSize[1] * z;
			for (int y = 0; y < dataSize[1]; y++)
			{
				if (y + x1[2] < blockSize[1])
				{
					size_t offsetyBlock = (size_t)blockSize[0] * (y + x1[1]);
					size_t offsetyData = (size_t)dataSize[0] * y;
					for (int x = 0; x < dataSize[0]; x++)
					{
						if (x + x1[0] < blockSize[0])
						{
							block[offsetzBlock + offsetyBlock + x + x1[0]] = (Tbloc)data[offsetzData + offsetyData + x];
						}
					}
				}
			}
		}
	}
	return SUCCESS;
}

int DataInsert::insert(void* block, int* blockSize, int blockDataFormat, void* data, int* dataSize, int dataFormat, int* x1)
{
	int ret = FAIL;

	switch (blockDataFormat)
	{
	case FORMAT_INT16:
		if (dataFormat == FORMAT_INT16) ret = insert<short, short>((short*)block, blockSize, (short*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT32) ret = insert<short, float>((short*)block, blockSize, (float*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT64) ret = insert<short, double>((short*)block, blockSize, (double*)data, dataSize, x1);
		break;
	case FORMAT_FLOAT32:
		if (dataFormat == FORMAT_INT16) ret = insert<float, short>((float*)block, blockSize, (short*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT32) ret = insert<float, float>((float*)block, blockSize, (float*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT64) ret = insert<float, double>((float*)block, blockSize, (double*)data, dataSize, x1);
		break;
	case FORMAT_FLOAT64:
		if (dataFormat == FORMAT_INT16) ret = insert<double, short>((double*)block, blockSize, (short*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT32) ret = insert<double, float>((double*)block, blockSize, (float*)data, dataSize, x1);
		else if (dataFormat == FORMAT_FLOAT64) ret = insert<double, double>((double*)block, blockSize, (double*)data, dataSize, x1);
		break;
	}
	return ret;
}