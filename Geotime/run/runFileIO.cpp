
#include <stdio.h>
#include <malloc.h>
#include <utils.h>
#include <utilDisplay.h>
#include <fileio2.h>
#include <seismicPolarity.h>
#include <runFileIO.h>

int runFileIO(int argc, char** argv)
{
	char* seismisFilename = "D:\\JACK2\\DATA\\SEISMIC\\seismic.xt";
	char* seismisFilename2 = "D:\\JACK2\\DATA\\SEISMIC\\seismic_tmp.xt";

	short* data = nullptr;

	FILEIO2* p = new FILEIO2();
	p->openForRead(seismisFilename);
	int dimx = p->get_dimx();
	int dimy = p->get_dimy();
	int dimz = p->get_dimz();
	data = (short*)calloc(dimx * dimy, sizeof(short));
	p->inlineRead(0, data);
	delete p;


	int polarityDimx = (dimx - 1) / 8 + 1;
	char* pol = (char*)calloc(polarityDimx, sizeof(char));
	
	seismisPolarityTraceFill(data, dimx, pol, polarityDimx);

	for (int x = 0; x < 10; x++)
	{
		int p = c_btesti_NK(pol, x);
		int type = getTraceType(pol, x, dimx);
		fprintf(stderr, "%d --> %d\n", x, type);
	}
	UtilDisplay::display(data, 10, FORMAT_INT16);

	short* data1 = (short*)calloc((size_t)dimx * dimy*dimz, sizeof(short));
	for (size_t add = 0; add < (size_t)dimx * dimy * dimz; add++)
		data1[add] = add%100;

	FILEIO2* p2 = new FILEIO2();
	p2->createNew(seismisFilename, seismisFilename2, dimx, dimy, dimz, 2);
	delete p2;
	p2 = new FILEIO2();
	p2->openForWrite(seismisFilename2);
	p2->inlineManyWrite(0, dimz, data1);
	delete p2;





	FREE(data)
	FREE(data1)
	FREE(pol)
	fprintf(stderr, "run fileio ok\n");
	return 0;
}