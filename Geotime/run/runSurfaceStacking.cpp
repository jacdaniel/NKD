
#include <stdio.h>
#include <malloc.h>

#include <utils.h>
#include <fileio2.h>
#include <surfaceStacking.h>
#include <runSurfaceStacking.h>


int runSurfaceStacking(int argc, char** argv)
{
	char* seismisFilename = "D:\\JACK2\\DATA\\SEISMIC\\seismic.xt";
	char* seismisFilename2 = "D:\\JACK2\\DATA\\SEISMIC\\stack.xt";
	int size[3];

	float* surface = nullptr;

	FILEIO2* p = nullptr;
	newSafe((void**)&p, FILEIO2);
	p->openForRead(seismisFilename);
	size[0] = p->get_dimx();
	size[1] = p->get_dimy();
	size[2] = p->get_dimz();
	delete p;

	surface = (float*)calloc(size[1] * size[2], sizeof(float));
	short* stack = (short*)calloc((size_t)size[0] * size[1] * size[2], sizeof(short));
	SurfaceStacking* s = nullptr;
	newSafe((void**)&s, SurfaceStacking);
	s->setSize(size);
	s->setStack(stack);
	s->setStackFormat(FORMAT_INT16);	
	s->setSurfaceFormat(FORMAT_FLOAT32);
	
	for (int add = 0; add < (size_t)size[1] * size[2]; add++)
		surface[add] = (int)(100.0f + (float)add / 20.0f)%(150);
	s->setSurface(surface);
	for (int i = 0; i < 100; i++)
	s->run();


	FILEIO2* p2 = new FILEIO2();
	p2->createNew(seismisFilename, seismisFilename2, size[0], size[1], size[2], 2);
	delete p2;
	p2 = new FILEIO2();
	p2->openForWrite(seismisFilename2);
	p2->inlineManyWrite(0, size[2], stack);
	delete p2;


	fprintf(stderr, "runSurfaceStacking ok\n");
	return 0;
}