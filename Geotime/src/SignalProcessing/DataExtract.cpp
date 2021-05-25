
#include <utils.h>
#include <DataExtract.h>

template<typename Tin, typename Tout>int DataExtract::extract(Tin* dataIn, int* size, int* x1, int* x2, Tout* dataOut)
{
	int dimx = x2[0] - x1[0] + 1;
	int dimy = x2[1] - x1[1] + 1;
	int dimz = x2[2] - x1[2] + 1;
	for (int z=0; z<dimz; z++)
	{
		if ( z+x1[2] < size[2] )
		{
			for (int y=0; y<dimy; y++)
			{
				if ( y + x1[2] < size[1])
				{
					for (int x = 0; x < dimx; x++)
					{
						if ( x+x1[0] < size[0])
						{
							dataOut[(size_t)dimx * dimy * z + (size_t)dimx * y + x] = (Tout)dataIn[(size_t)size[0] * size[1] * (z + x1[2]) + (size_t)size[0] * (y + x1[1]) + x + x1[0]];
						}
					}
				}
			}
		}
	}
	return SUCCESS;
}

template<typename Tin, typename Tout>int DataExtract::extract(Tin* dataIn, int* size, int x1, int y1, int z1, int x2, int y2, int z2, Tout* dataOut)
{
	int dimx = x2 - x1 + 1;
	int dimy = x2 - x1 + 1;
	int dimz = x2 - x1 + 1;
	for (int z = 0; z < dimz; z++)
	{
		if (z + z1 < size[2])
		{
			for (int y = 0; y < dimy; y++)
			{
				if (y + y1 < size[1])
				{
					for (int x = 0; x < dimx; x++)
					{
						if (x + x1 < size[0])
						{
							dataOut[(size_t)dimx * dimy * z + (size_t)dimx * y + x] = (Tout)dataIn[(size_t)size[0] * size[1] * (z + z1) + (size_t)size[0] * (y + y1) + x + x1];
						}
					}
				}
			}
		}
	}
	return SUCCESS;
}


int DataExtract::extract(void* dataIn, int* size, int dataInFormat, int *x1, int* x2, void* dataOut, int dataOutFormat)
{
	int ret = FAIL;
	switch (dataInFormat)
	{
		case FORMAT_INT16:
			if (dataOutFormat == FORMAT_INT16) ret = extract<short, short>((short*)dataIn, size, x1, x2, (short*)dataOut); 
			else if ( dataOutFormat == FORMAT_FLOAT32 ) ret = extract<short, float>((short*)dataIn, size, x1, x2, (float*)dataOut); 
			else if ( dataOutFormat == FORMAT_FLOAT64) ret = extract<short, double>((short*)dataIn, size, x1, x2, (double*)dataOut);
			break;
		case FORMAT_FLOAT32:
			if (dataOutFormat == FORMAT_INT16) ret = extract<float, short>((float*)dataIn, size, x1, x2, (short*)dataOut);
			else if (dataOutFormat == FORMAT_FLOAT32) ret = extract<float, float>((float*)dataIn, size, x1, x2, (float*)dataOut);
			else if (dataOutFormat == FORMAT_FLOAT64) ret = extract<float, double>((float*)dataIn, size, x1, x2, (double*)dataOut);
			break;
		case FORMAT_FLOAT64:
			if (dataOutFormat == FORMAT_INT16) ret = extract<double, short>((double*)dataIn, size, x1, x2, (short*)dataOut);
			else if (dataOutFormat == FORMAT_FLOAT32) ret = extract<double, float>((double*)dataIn, size, x1, x2, (float*)dataOut);
			else if (dataOutFormat == FORMAT_FLOAT64) ret = extract<double, double>((double*)dataIn, size, x1, x2, (double*)dataOut);
			break;
	}

	return ret;
}
	
int DataExtract::extract(void* dataIn, int* size, int dataInFormat, int x1, int y1, int z1, int x2, int y2, int z2, void* dataOut, int dataOutFormat)
{
	int ret = FAIL;
	switch (dataInFormat)
	{
	case FORMAT_INT16:
		if (dataOutFormat == FORMAT_INT16) ret = extract<short, short>((short*)dataIn, size, x1, y1, z1, x2, y2, z2, (short*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT32) ret = extract<short, float>((short*)dataIn, size, x1, y1, z1, x2, y2, z2, (float*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT64) ret = extract<short, double>((short*)dataIn, size, x1, y1, z1, x2, y2, z2, (double*)dataOut);
		break;
	case FORMAT_FLOAT32:
		if (dataOutFormat == FORMAT_INT16) ret = extract<float, short>((float*)dataIn, size, x1, y1, z1, x2, y2, z2, (short*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT32) ret = extract<float, float>((float*)dataIn, size, x1, y1, z1, x2, y2, z2, (float*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT64) ret = extract<float, double>((float*)dataIn, size, x1, y1, z1, x2, y2, z2, (double*)dataOut);
		break;
	case FORMAT_FLOAT64:
		if (dataOutFormat == FORMAT_INT16) ret = extract<double, short>((double*)dataIn, size, x1, y1, z1, x2, y2, z2, (short*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT32) ret = extract<double, float>((double*)dataIn, size, x1, y1, z1, x2, y2, z2, (float*)dataOut);
		else if (dataOutFormat == FORMAT_FLOAT64) ret = extract<double, double>((double*)dataIn, size, x1, y1, z1, x2, y2, z2, (double*)dataOut);
		break;
	}
	return ret;
}


