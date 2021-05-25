
#ifndef __CWT_FILE__
#define __CWT_FILE__

#ifdef CWTENABLE
#include <wpc.h>
#endif

typedef struct _WPC_PARAM
{
    int stride1,
        stride2,
        block,
        levels,
        encoder,
        waveid,
        target;
    float error,
        sigma,
        std;

}WPC_PARAM;

class CWT_FILE
{
public:
    enum FORMAT { CHAR, UCHAR, INT16, UINT16, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE };
    const int SIZEOF[10] = { 1,1,16,16,32,32,64,64,32,64 };

    enum ACCESS { CREATE, READ, READ_FULL_MEMORY };
    enum TYPE { DISK, MEMORY };

private:
    const int WPC_DEFAULT_BLOCK = 64;
    const int WPC_DEFAULT_LEVELS = 3;
    const int WPC_DEFAULT_ENCODER = 0;
    const int WPC_DEFAULT_WAVEID = 12;
#ifdef CWTENABLE
    const int WPC_DEFAULT_TARGET = WPC_ARCH_CPU; // WPC_ARCH_CPU;
#else
    const int WPC_DEFAULT_TARGET = 0; // WPC_ARCH_CPU;
#endif
    const float WPC_DEFAULT_ERROR = 0.01f;
    const float WPC_DEFAULT_SIGMA = 5.0f;
    const float WPC_DEFAULT_STD = -1.0f;
    const int HEADER0 = 10;
    // const long ARRAY_INDEX_OFFSET = 100*4;


private:
    // int dimx, dimy, dimz, _header0, type, format;

    int dimx, dimy, dimz, format, _sizeof;
    int offset, rw;
    size_t offset1, offset2;
    FILE* pFile;

    long* inline_offset, * inline_size;
    unsigned char* code, * memory_code, * d_code;
    float* uncompressed_data, * d_uncompressed_data;
#ifdef CWTENABLE
    wpc_t* wpc = NULL;
#endif
    float* array_std;
    WPC_PARAM wpc_param;

    void header_write();
    void header_read(int all);
    long total_data_size();
    // static long format_sizeof(int format);

public:
    CWT_FILE();
    ~CWT_FILE();
    void openForRead(const char* filename);
    void openForRead(const char* filename, FORMAT format);
    void openForWrite(const char* filename, float error = 0.01f);
    void createNew(const char* filename_src, const char* filename_dst);
    void createNew(const char* filename_src, const char* filename_dst, int dimx, int dimy, int dimz, int format);

    int get_dimx();
    int get_dimy();
    int get_dimz();
    int get_format();
    int get_sizeof();
    float inlineRead(long no, void* data);
    float inlineManyRead(int no, int nbre, void* data);
    float inlineWrite(int no, void* data);
    float inlineManyWrite(int no, int nbre, void* data);

    void set_wpc_param_stride1(int val);
    void set_wpc_param_stride2(int val);
    void set_wpc_param_block(int val);
    void set_wpc_param_levels(int val);
    void set_wpc_param_encoder(int val);
    void set_wpc_param_waveid(int val);
    void set_wpc_param_target(int val);
    void set_wpc_param_error(float val);
    void set_wpc_param_sigma(float val);
    void set_wpc_param_std(float val);
};

#endif