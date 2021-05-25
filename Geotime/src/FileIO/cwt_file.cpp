
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include <wpc.h>

#include <cwt_file.h>


#define FREE(ptr) { if ( ptr != NULL ) {free(ptr); ptr = NULL; }}
#define CUDAFREE(ptr) { if ( ptr != NULL ) {cudaFree(ptr); ptr = NULL; }}

static void endian_short_swap(short* data, long size)
{
    for (long add = 0; add < size; add++)
    {
        data[add] = ((data[add] & 0xff) << 8) | ((data[add] & 0xff00) >> 8);
    }
}

#ifndef SAWP
#define SWAP(_a_, _b_, _temp_) { \
	_temp_ = _b_; \
	_b_ = _a_; \
	_a_ = _temp_; }
#endif

static void endianSwap(void* _data, long size, int length)
{
    long i, j;
    unsigned char temp, * data = (unsigned char*)_data;

    for (i = 0; i < size; i++)
        for (j = 0; j < length / 2; j++)
            SWAP(data[i * length + j], data[i * length + (length - 1 - j)], temp)
}


template<typename Tin, typename Tout> void data_transfert(void* _in, void* _out, long size)
{
    Tin* in = (Tin*)_in;
    Tout* out = (Tout*)_out;
    for (long add = 0; add < size; add++)
        out[add] = in[add];
}


CWT_FILE::CWT_FILE()
{
    this->inline_size = NULL;
    this->inline_offset = NULL;
    this->code = NULL;
    this->d_code = NULL;
    this->uncompressed_data = NULL;
    this->d_uncompressed_data = NULL;
    this->memory_code = NULL;
    this->pFile = 0;
    set_wpc_param_block(WPC_DEFAULT_BLOCK);
    set_wpc_param_levels(WPC_DEFAULT_LEVELS);
    set_wpc_param_encoder(WPC_DEFAULT_ENCODER);
    set_wpc_param_waveid(WPC_DEFAULT_WAVEID);
    set_wpc_param_target(WPC_DEFAULT_TARGET);
    set_wpc_param_error(WPC_DEFAULT_ERROR);
    set_wpc_param_sigma(WPC_DEFAULT_SIGMA);
    set_wpc_param_std(WPC_DEFAULT_STD);
}


CWT_FILE::~CWT_FILE()
{
    if (this->rw == 1)
    {
        header_write();
    }
    FREE(this->inline_size)
        FREE(this->inline_offset)
        FREE(this->code)
        FREE(this->uncompressed_data)
        CUDAFREE(d_code)
        CUDAFREE(d_uncompressed_data)
        FREE(this->memory_code)
        if (this->pFile) fclose(this->pFile);
}

void CWT_FILE::openForRead(const char* filename)
{
#ifdef CWTENABLE
    this->pFile = fopen(filename, "r");
    this->rw = 0;
    header_read(1);

    this->inline_offset = (long*)calloc(this->dimz, sizeof(long));
    for (long z = 1; z < this->dimz; z++)
    {
        this->inline_offset[z] = this->inline_offset[z - 1] + this->inline_size[z - 1];
    }

    long stride1 = this->dimx;
    long stride2 = 0;
    int err = wpc_create(&this->wpc);

    // this->wpc_param.error = 0.01;

    long datasize = (long)dimx * dimy;
    size_t codesize = datasize;
    this->uncompressed_data = (float*)calloc((long)dimx * dimy, sizeof(float));
    this->code = (unsigned char*)calloc(codesize * 4, sizeof(unsigned char));
    if (this->wpc_param.target == WPC_ARCH_GPU)
    {
        cudaError_t ret1 = cudaSuccess;
        cudaError_t ret2 = cudaSuccess;
        ret1 = cudaMalloc((void**)&this->d_uncompressed_data, (long)dimx * dimy * sizeof(float));
        ret2 = cudaMalloc((void**)&this->d_code, (long)codesize * 4 * sizeof(unsigned char));
        if (ret1 != cudaSuccess || ret2 != cudaSuccess)
        {
            CUDAFREE(this->d_uncompressed_data)
                CUDAFREE(this->d_code)
                this->wpc_param.target = WPC_ARCH_CPU;
        }
    }

    err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
        this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
        this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);

    if (err == -1 && this->wpc_param.target == WPC_ARCH_GPU)
    {
        if (this->d_uncompressed_data != NULL) { cudaFree(this->d_uncompressed_data); d_uncompressed_data = NULL; }
        if (this->d_code != NULL) { cudaFree(this->d_code); this->d_code = NULL; }
        set_wpc_param_target(WPC_ARCH_CPU);
        err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
            this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
            this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);
    }

    fseek(this->pFile, (long)this->offset2, SEEK_SET);
#endif
}

void CWT_FILE::openForRead(const char* filename, FORMAT requestedFormat)
{
#ifdef CWTENABLE
    this->pFile = fopen(filename, "r");
    this->rw = 0;
    header_read(1);
    this->format = requestedFormat;

    this->inline_offset = (long*)calloc(this->dimz, sizeof(long));
    for (long z = 1; z < this->dimz; z++)
    {
        this->inline_offset[z] = this->inline_offset[z - 1] + this->inline_size[z - 1];
    }

    long stride1 = this->dimx;
    long stride2 = 0;
    int err = wpc_create(&this->wpc);

    // this->wpc_param.error = 0.01;

    long datasize = (long)dimx * dimy;
    size_t codesize = datasize;
    this->uncompressed_data = (float*)calloc((long)dimx * dimy, sizeof(float));
    this->code = (unsigned char*)calloc(codesize * 4, sizeof(unsigned char));
    if (this->wpc_param.target == WPC_ARCH_GPU)
    {
        cudaError_t ret1 = cudaSuccess;
        cudaError_t ret2 = cudaSuccess;
        ret1 = cudaMalloc((void**)&this->d_uncompressed_data, (long)dimx * dimy * sizeof(float));
        ret2 = cudaMalloc((void**)&this->d_code, (long)codesize * 4 * sizeof(unsigned char));
        if (ret1 != cudaSuccess || ret2 != cudaSuccess)
        {
            CUDAFREE(this->d_uncompressed_data)
                CUDAFREE(this->d_code)
                this->wpc_param.target = WPC_ARCH_CPU;
        }
    }

    err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
        this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
        this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);

    if (err == -1 && this->wpc_param.target == WPC_ARCH_GPU)
    {
        if (this->d_uncompressed_data != NULL) { cudaFree(this->d_uncompressed_data); d_uncompressed_data = NULL; }
        if (this->d_code != NULL) { cudaFree(this->d_code); this->d_code = NULL; }
        set_wpc_param_target(WPC_ARCH_CPU);
        err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
            this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
            this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);
    }

    fseek(this->pFile, (long)this->offset2, SEEK_SET);
#endif
}

void CWT_FILE::openForWrite(const char* filename, float error)
{
#ifdef CWTENABLE
    this->pFile = fopen(filename, "r+"); // !!!!!
    this->rw = 1;
    header_read(1);

    long stride1 = this->dimx;
    long stride2 = 0;
    int err = wpc_create(&this->wpc);

    this->wpc_param.error = error;

    long datasize = (long)dimx * dimy;
    size_t codesize = datasize;
    this->uncompressed_data = (float*)calloc((long)dimx * dimy, sizeof(float));
    this->code = (unsigned char*)calloc(codesize * 4, sizeof(unsigned char));
    if (this->wpc_param.target == WPC_ARCH_GPU)
    {
        cudaError_t ret1 = cudaSuccess;
        cudaError_t ret2 = cudaSuccess;
        ret1 = cudaMalloc((void**)&this->d_uncompressed_data, (long)dimx * dimy * sizeof(float));
        ret2 = cudaMalloc((void**)&this->d_code, (long)codesize * 4 * sizeof(unsigned char));
        if (ret1 != cudaSuccess || ret2 != cudaSuccess)
        {
            CUDAFREE(this->d_uncompressed_data)
                CUDAFREE(this->d_code)
                this->wpc_param.target = WPC_ARCH_CPU;
        }
    }
    err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
        this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
        this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);

    if (err == -1 && this->wpc_param.target == WPC_ARCH_GPU)
    {
        if (this->d_uncompressed_data != NULL) { cudaFree(this->d_uncompressed_data); d_uncompressed_data = NULL; }
        if (this->d_code != NULL) { cudaFree(this->d_code); this->d_code = NULL; }
        set_wpc_param_target(WPC_ARCH_CPU);
        err = wpc_init(this->wpc, 0, 0, 0, this->dimx, this->dimy, 1, stride1, stride2, 20,
            this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error,
            this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);
    }

    fseek(this->pFile, (long)this->offset2, SEEK_SET);
#endif
}


void CWT_FILE::header_write()
{
    int tmp;
    if (this->pFile != nullptr) {
        fseek(this->pFile, 4, SEEK_SET);
        tmp = dimx; endianSwap(&tmp, 1, sizeof(int)); int a = fwrite((void*)&tmp, sizeof(int), 1, this->pFile);
        tmp = dimy; endianSwap(&tmp, 1, sizeof(int)); fwrite((void*)&tmp, sizeof(int), 1, pFile);
        tmp = dimz; endianSwap(&tmp, 1, sizeof(int)); fwrite((void*)&tmp, sizeof(int), 1, pFile);
        fseek(pFile, 32, SEEK_SET);
        tmp = format; endianSwap(&tmp, 1, sizeof(int)); fwrite((void*)&tmp, sizeof(int), 1, pFile);
        tmp = SIZEOF[format]; endianSwap(&tmp, 1, sizeof(int)); fwrite((void*)&tmp, sizeof(int), 1, pFile);
        fseek(this->pFile, (long)this->offset, SEEK_SET);
        fwrite((void*)&this->wpc_param.block, sizeof(int), 1, this->pFile);
        fwrite((void*)&this->wpc_param.levels, sizeof(int), 1, this->pFile);
        fwrite((void*)&this->wpc_param.encoder, sizeof(int), 1, this->pFile);
        fwrite((void*)&this->wpc_param.waveid, sizeof(int), 1, this->pFile);
        // fwrite((void*)&this->wpc_param.target, sizeof(int), 1, this->pFile);
        tmp = 1; fwrite((void*)&tmp, sizeof(int), 1, this->pFile);
        fwrite((void*)&this->wpc_param.error, sizeof(float), 1, this->pFile);
        // float tmp_f = 0.01f; fwrite((void*)&tmp_f, sizeof(float), 1, this->pFile);
        fwrite((void*)&this->wpc_param.sigma, sizeof(float), 1, this->pFile);
        if (inline_size && array_std)
        {
            fwrite((void*)inline_size, sizeof(long), (long)dimz, this->pFile);
            fwrite((void*)array_std, sizeof(float), (long)dimz, this->pFile);
        }
    }
}



void CWT_FILE::header_read(int all)
{
    int tmp = 1;

    fseek((FILE*)this->pFile, 4, SEEK_SET);
    fread((void*)&this->dimx, sizeof(int), 1, this->pFile); endianSwap(&this->dimx, 1, sizeof(int));
    fread((void*)&this->dimy, sizeof(int), 1, this->pFile); endianSwap(&this->dimy, 1, sizeof(int));
    fread((void*)&this->dimz, sizeof(int), 1, this->pFile); endianSwap(&this->dimz, 1, sizeof(int));
    fseek((FILE*)this->pFile, 32, SEEK_SET);
    fread((void*)&this->format, sizeof(int), 1, this->pFile); endianSwap(&this->format, 1, sizeof(int));
    fread((void*)&this->_sizeof, sizeof(int), 1, this->pFile); endianSwap(&this->_sizeof, 1, sizeof(int));
    fseek((FILE*)this->pFile, 72, SEEK_SET);
    fread((void*)&this->offset, sizeof(int), 1, this->pFile); endianSwap(&this->offset, 1, sizeof(int));
    if (all)
    {
        fseek(this->pFile, (long)this->offset, SEEK_SET);
        fread((void*)&this->wpc_param.block, sizeof(int), 1, this->pFile);
        fread((void*)&this->wpc_param.levels, sizeof(int), 1, this->pFile);
        fread((void*)&this->wpc_param.encoder, sizeof(int), 1, this->pFile);
        fread((void*)&this->wpc_param.waveid, sizeof(int), 1, this->pFile);
        // fread((void*)&this->wpc_param.target, sizeof(int), 1, this->pFile);
        tmp = 1; fread((void*)&tmp, sizeof(int), 1, this->pFile);
        fread((void*)&this->wpc_param.error, sizeof(float), 1, this->pFile);
        // float tmp_f = 0.01f; fread((void*)&tmp_f, sizeof(float), 1, this->pFile);
        fread((void*)&this->wpc_param.sigma, sizeof(float), 1, this->pFile);
        this->inline_size = (long*)calloc(this->dimz, sizeof(long));
        this->array_std = (float*)calloc(this->dimz, sizeof(float));
        fread((void*)this->inline_size, sizeof(long), (long)this->dimz, this->pFile);
        fread((void*)this->array_std, sizeof(float), (long)this->dimz, this->pFile);
        this->offset1 = (size_t)this->offset + (size_t)7 * 4;
        this->offset2 = (size_t)this->offset1 + (size_t)dimz * sizeof(long) + (size_t)dimz * sizeof(float);
    }
}

void CWT_FILE::createNew(const char* filename_src, const char* filename_dst)
{
    int offset0;
    FILE* pfile_src = fopen(filename_src, "r");
    fseek(pfile_src, 72, SEEK_SET);
    fread(&offset0, sizeof(int), 1, pfile_src);
    endianSwap(&offset0, 1, sizeof(int));
    char* h = (char*)calloc(offset0, sizeof(char));
    fseek(pfile_src, 0, SEEK_SET);
    fread(h, sizeof(char), offset0, pfile_src);
    fclose(pfile_src);
    this->pFile = fopen(filename_dst, "w+");
    fwrite(h, sizeof(char), offset0, this->pFile);
    header_read(0);
    header_write();
    free(h);
    this->rw = 0;
}

void CWT_FILE::createNew(const char* filename_src, const char* filename_dst, int dimx, int dimy, int dimz, int format)
{
    createNew(filename_src, filename_dst);
    this->dimx = dimx;
    this->dimy = dimy;
    this->dimz = dimz;
    this->format = format;
    header_write();
}

int CWT_FILE::get_dimx()
{
    return this->dimx;
}

int CWT_FILE::get_dimy()
{
    return this->dimy;
}

int CWT_FILE::get_dimz()
{
    return this->dimz;
}

int CWT_FILE::get_format()
{
    return this->format;
}

int CWT_FILE::get_sizeof()
{
    return this->_sizeof;
}



float CWT_FILE::inlineRead(long no, void* data)
{
#ifdef CWTENABLE
    void* custream = NULL;
    long off = (long)this->offset2 + this->inline_offset[no];
    fseek(this->pFile, off, SEEK_SET);
    fread(this->code, sizeof(unsigned char), this->inline_size[no], this->pFile);
    int err = -1;

    float* tabResult = this->uncompressed_data;
    if (this->format == FORMAT::FLOAT) {
        tabResult = static_cast<float*>(data);
    }

    if (this->wpc_param.target == WPC_ARCH_CPU || this->d_uncompressed_data == NULL || this->d_code == NULL)
    {
        err = wpc_decompress(this->wpc, tabResult, this->code, this->inline_size[no], this->array_std[no], 0, (intptr_t)custream);
    }
    else if (this->wpc_param.target == WPC_ARCH_GPU && this->d_uncompressed_data && this->d_code)
    {
        cudaMemcpy(this->d_code, this->code, this->inline_size[no] * sizeof(unsigned char), cudaMemcpyHostToDevice);
        err = wpc_decompress(this->wpc, this->d_uncompressed_data, this->d_code, this->inline_size[no], this->array_std[no], 0, (intptr_t)custream);
        cudaMemcpy(tabResult, this->d_uncompressed_data, (long)this->dimx * this->dimy * sizeof(float), cudaMemcpyDeviceToHost);
    }
    if (this->format == FORMAT::INT16) data_transfert<float, short>(this->uncompressed_data, data, (long)this->dimx * this->dimy);
    float ret = (float)(dimx * dimy * sizeof(short)) / (float)this->inline_size[no];
    return ret;
#else
    return 0.0f;
#endif
}

float CWT_FILE::inlineManyRead(int no, int nbre, void* data)
{
    float ret = 0.0;
    for (int z = 0; z < nbre; z++)
    {
        ret += inlineRead(no + z, &((short*)data)[(size_t)z * this->dimx * this->dimy]);
    }
    return ret / (float)nbre;
}


#define DEBUG0 fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
float CWT_FILE::inlineWrite(int no, void* data)
{
#ifdef CWTENABLE
    void* custream = NULL;
    if (this->format == FORMAT::INT16) data_transfert<short, float>(data, this->uncompressed_data, (long)this->dimx * this->dimy);
    float std = this->wpc_param.std;
    size_t codesize = (long)this->dimx * this->dimy * sizeof(float);
    int err = -1;
    if (this->wpc_param.target == WPC_ARCH_CPU || this->d_uncompressed_data == NULL || this->d_code == NULL)
    {
        err = wpc_compress(this->wpc, this->uncompressed_data, this->code, &codesize, &std, 0, (intptr_t)NULL);
    }
    else if (this->wpc_param.target == WPC_ARCH_GPU && this->d_uncompressed_data && this->d_code)
    {
        cudaMemcpy(this->d_uncompressed_data, this->uncompressed_data, (long)this->dimx * this->dimy * sizeof(float), cudaMemcpyHostToDevice);
        err = wpc_compress(this->wpc, this->d_uncompressed_data, this->d_code, &codesize, &std, 0, (intptr_t)custream);
        cudaMemcpy(this->code, this->d_code, codesize * sizeof(char), cudaMemcpyDeviceToHost);
    }
    this->inline_size[no] = codesize;
    this->array_std[no] = std;
    fwrite((void*)code, sizeof(unsigned char), codesize, this->pFile);
    fflush(this->pFile);
    float ret = (float)(dimx * dimy * sizeof(short)) / (float)codesize;
    return ret;
#else
    return 0.0f;
#endif
}

float CWT_FILE::inlineManyWrite(int no, int nbre, void* data)
{
    float ret = 0.0f;
    for (int z = 0; z < nbre; z++)
    {
        ret += inlineWrite(no + z, &((short*)data)[(size_t)z * this->dimx * this->dimy]);
    }
    return ret / (float)nbre;
}



// long CWT_FILE::total_data_size()
// {
//     long size = 0;
//     for ( int z = 0; z< this->dimz; z++)
//         size += this->inline_size[z];
//     return size;
// }



// void CWT_FILE::block_new(char *filename, int format, int dimx, int dimy, int dimz)
// {
//     this->format = format;
//     this->dimx = dimx;
//     this->dimy = dimy;
//     this->dimz = dimz;

//     this->inline_offset = (long*)calloc(dimz, sizeof(long));
//     this->inline_size = (long*)calloc(dimz, sizeof(long));
//     this->array_std = (float*)calloc(dimz, sizeof(float));

//     long stride1 = dimx;
//     long stride2 = 0;
//     int err = wpc_create(&this->wpc);
//     err = wpc_init(this->wpc, 0, 0, 0, dimx, dimy, 1, stride1, stride2, 20, 
//         this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error, 
//         this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);

//     long datasize = (long)dimx * dimy;
//     size_t codesize = datasize;
//     this->uncompressed_data = (float*)calloc((long)dimx*dimy, sizeof(float));
//     this->code = (unsigned char *) calloc(codesize*4, sizeof(unsigned char));

//     this->_pFile = fopen(filename, "wb");
//     this->offset_data0 = ARRAY_INDEX_OFFSET + (long)dimz * sizeof(long) + (long)dimz * sizeof(float);
//     fseek((FILE*)this->_pFile, this->offset_data0, SEEK_SET);
// }

// float CWT_FILE::inline_insert(void *data, int no)
// {
//     // short *d = (short*)data;
//     // for (long add=0; add<(long)this->dimx*this->dimy; add++)
//     //    this->uncompressed_data[add] = d[add];

//     if ( this->format == FORMAT::SHORT ) data_transfert<short, float>(data, this->uncompressed_data, (long)this->dimx*this->dimy);
//     // data_transfert_to_float<short>(data, this->uncompressed_data, (long)this->dimx*this->dimy);

//     float std = this->wpc_param.std;
//     size_t codesize;

//     int err = wpc_compress(this->wpc, this->uncompressed_data, this->code, &codesize, &std, 0, NULL);    
//     this->inline_size[no] = codesize;
//     array_std[no] = std;
//     fwrite((void*)code, sizeof(unsigned char), codesize, (FILE*)this->_pFile);
//     fflush((FILE*)this->_pFile);
//     float ret = (float)(dimx*dimy*sizeof(short)) / (float)codesize;
//     return ret;
// }

// void CWT_FILE::open_for_read(char *filename, int type)
// {
//     this->_pFile = fopen(filename, "rb");
//     header_read();
//     this->inline_offset = (long*)calloc(this->dimz, sizeof(long));
//     for (long z=1; z<this->dimz; z++)
//     {
//         this->inline_offset[z] = this->inline_offset[z-1] + this->inline_size[z-1];
//     }
//     long datasize = (long)dimx * dimy;
//     size_t codesize = datasize;
//     this->uncompressed_data = (float*)calloc((long)dimx*dimy, sizeof(float));
//     this->code = (unsigned char *) calloc(codesize*4, sizeof(unsigned char));   

//     if ( type == DISK )
//     {        
//         this->memory_code = NULL;
//     }   
//     else if ( type == MEMORY )
//     {
//         long total_size = total_data_size();  
//         this->memory_code = (unsigned char*)calloc(total_size, sizeof(unsigned char));
//         long off = ARRAY_INDEX_OFFSET + (long)dimz * sizeof(long) + (long)dimz * sizeof(float);
//         fseek((FILE*)this->_pFile, off, SEEK_SET);
//         fread(this->memory_code, sizeof(unsigned char), total_size, (FILE*)this->_pFile);        
//     }

//     long stride1 = dimx;
//     long stride2 = 0;
//     int err = wpc_create(&this->wpc);
//     err = wpc_init(this->wpc, 0, 0, 0, dimx, dimy, 1, stride1, stride2, 20, 
//         this->wpc_param.block, this->wpc_param.levels, this->wpc_param.waveid, this->wpc_param.error, 
//         this->wpc_param.sigma, this->wpc_param.encoder, this->wpc_param.target);
// }



// float CWT_FILE::inline_read(long no, void *data)
// {
//     if ( this->memory_code == NULL )
//     {
//         long off = ARRAY_INDEX_OFFSET + (long)dimz * sizeof(long) + (long)dimz * sizeof(float) + this->inline_offset[no];
//         fseek((FILE*)this->_pFile, off, SEEK_SET);
//         fread(this->code, sizeof(unsigned char), this->inline_size[no], (FILE*)this->_pFile);
//     }
//     else
//     {
//         long off = this->inline_offset[no];
//         for (int add=0; add<this->inline_size[no]; add++)
//             this->code[add] = this->memory_code[off+add];        
//     }
//     int err = wpc_decompress(this->wpc, this->uncompressed_data, this->code, this->inline_size[no], this->array_std[no], 0, NULL);

//     if ( this->format == FORMAT::SHORT ) data_transfert<float, short>(this->uncompressed_data, data, (long)this->dimx*this->dimy);
//     // for (long add=0; add<(long)this->dimx*this->dimy; add++) ((short*)data)[add] = (short)this->uncompressed_data[add];
//     float ret = (float)(dimx*dimy*sizeof(short)) / (float)this->inline_size[no];
//     return ret;
// }

// CWT_FILE::~CWT_FILE()
// {
//     header_write();
//     FREE(this->inline_size)
//     FREE(this->inline_offset)
//     FREE(this->code)
//     FREE(this->uncompressed_data)    
//     FREE(this->memory_code)
//     fclose((FILE*)this->_pFile);
// }

// long CWT_FILE::get_dimx()
// {
//      return this->dimx;
//  }

// long CWT_FILE::get_dimy()
// // {
// //     return this->dimy;
// // }


// // long CWT_FILE::get_dimz()
// // {
// //     return this->dimz;
// // }




void CWT_FILE::set_wpc_param_stride1(int val)
{
    this->wpc_param.stride1 = val;
}

void CWT_FILE::set_wpc_param_stride2(int val)
{
    this->wpc_param.stride2 = val;
}

void CWT_FILE::set_wpc_param_block(int val)
{
    this->wpc_param.block = val;
}

void CWT_FILE::set_wpc_param_levels(int val)
{
    this->wpc_param.levels = val;
}

void CWT_FILE::set_wpc_param_encoder(int val)
{
    this->wpc_param.encoder = val;
}

void CWT_FILE::set_wpc_param_waveid(int val)
{
    this->wpc_param.waveid = val;
}

void CWT_FILE::set_wpc_param_target(int val)
{
    this->wpc_param.target = val;
}

void CWT_FILE::set_wpc_param_error(float val)
{
    this->wpc_param.error = val;
}

void CWT_FILE::set_wpc_param_sigma(float val)
{
    this->wpc_param.sigma = val;
}

void CWT_FILE::set_wpc_param_std(float val)
{
    this->wpc_param.std = val;
}
