
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



int getDimsFromSize(int* size)
{
	int dims = 0;
	for (int i = 0; i < 3; i++)
	{
		if (size[i] > 1) dims++;
	}
	return dims;
}