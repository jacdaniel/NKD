
#ifndef __FILEIO2__
#define __FILEIO2__

#include <stdio.h>

#include <cwt_file.h>
#include <xt_file.h>

class FILEIO2
{
    enum EXTENSION { XT, CWT, ISO };
    enum TARGET { NONE, CPU, GPU };

public:
    FILEIO2();
    ~FILEIO2();

    void set_target(int _target);
    void set_compress_error(float _error);

    void openForRead(char* filename);
    void openForWrite(char* filename, float cwt_error = 0.01f);
    void createNew(char* filename_src, char* filename_dst);
    void createNew(char* filename_src, char* filename_dst, int dimx, int dimy, int dimz, int format);

    int get_dimx();
    int get_dimy();
    int get_dimz();
    int get_format();
    int get_sizeof();
    float inlineRead(long no, void* data);
    float inlineManyRead(int no, int nbre, void* data);
    float inlineManyReadShort(int no, int nbre, void* data);
    float inlineWrite(int no, void* data);
    float inlineManyWrite(int no, int nbre, void* data);
    float inlineIncrementalWrite(int no, void* data);
    static int exist(char* filename);

    int blockRead(int *x1, int *x2, void *data);


private:
    int get_fileformat(char* filename);
    XT_FILE* p_xtfile;
    CWT_FILE* p_cwtfile;
    int extension;
    float compress_error;
    int target;
};



#endif