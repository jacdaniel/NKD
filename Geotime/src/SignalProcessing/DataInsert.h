#ifndef __DATAINSERT__
#define __DATAINSERT__

class DataInsert
{
public:
	template<typename Tbloc, typename Tdata> static int insert(Tbloc* block, int* blockSize, Tdata* data, int* dataSize, int* x1);

	static int insert(void* block, int* blockSize, int blockDataFormat, void* data, int* dataSize, int dataFormat, int* x1);


};


#endif