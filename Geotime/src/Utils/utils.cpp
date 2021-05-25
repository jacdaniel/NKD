
#include <stdio.h>
#include <malloc.h>

#include <utils.h>


int mallocSafe_(void**ptr, long size, char *file, long line)
{
	if ( ptr == nullptr )
	{
		fprintf(stderr, "mallocSafe nullptr: %s - %d\n", file, line);
		return MALLOC_FAIL_NULLPTR;
	}
	(*ptr) = malloc(size);
	if ( (*ptr) == nullptr )
	{
		fprintf(stderr, "mallocSafe cannot allocate memory: %s - %d\n", file, line);
		return MALLOC_FAIL_CANNOT_ALLOCATE_MEMORY;
	}
	return MALLOC_SUCCESS;
}


int callocSafe_(void**ptr, long size, long _sizeof, char *file, long line)
{
	if ( ptr == nullptr )
	{
		fprintf(stderr, "callocSafe nullptr: %s - %d\n", file, line);
		return CALLOC_FAIL_NULLPTR;
	}
	(*ptr) = calloc(size, _sizeof);
	if ( (*ptr) == nullptr )
	{
		fprintf(stderr, "callocSafe cannot allocate memory: %s - %d\n", file, line);
		return CALLOC_FAIL_CANNOT_ALLOCATE_MEMORY;
	}
	return CALLOC_SUCCESS;
}


int callocSafeFormat_(void** ptr, size_t size, int dataFormat, char *file, long line)
{
	return callocSafe_(ptr, size, getSizeofFromFormat(dataFormat), file, line);
}


size_t getSizeofFromFormat(int dataFormat)
{
	switch (dataFormat)
	{
	case FORMAT_CHAR: return sizeof(char); break;
	case FORMAT_UCHAR: return sizeof(unsigned char); break;
	case FORMAT_INT16: return sizeof(short); break;
	case FORMAT_UINT16: return sizeof(unsigned short); break;
	case FORMAT_INT32: return sizeof(int); break;
	case FORMAT_UINT32: return sizeof(unsigned int); break;
	case FORMAT_INT64: return sizeof(long); break;
	case FORMAT_UINT64: return sizeof(unsigned long); break;
	case FORMAT_FLOAT32: return sizeof(float); break;
	case FORMAT_FLOAT64: return sizeof(double); break;
	default: return 0; break;
	}

}

int getDimsFromSize(int* size)
{
	int dims = 0;
	for (int i = 0; i < 3; i++)
	{
		if (size[i] > 1) dims++;
	}
	return dims;
}