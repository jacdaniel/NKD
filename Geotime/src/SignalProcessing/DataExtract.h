
#ifndef __DATAEXTRACT__
#define __DATAEXTRACT__

class DataExtract
{

public:
	template<typename Tin, typename Tout>int extract(Tin* dataIn, int* size, int* x1, int* x2, Tout* dataOut);
	template<typename Tin, typename Tout>int extract(Tin* dataIn, int* size, int x1, int y1, int z1, int x2, int y2, int z2, Tout* dataOut);
	int extract(void* dataIn, int* size, int dataInFormat, int* x1, int* x2, void* dataOut, int dataOutFormat);
	int extract(void* dataIn, int* size, int dataInFormat, int x1, int y1, int z1, int x2, int y2, int z2, void* dataOut, int dataOutFormat);
};



#endif