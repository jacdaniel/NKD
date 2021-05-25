
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <fileio2.h>

int FILEIO2::exist(char* filename)
{
    int ret = 0;
    FILE* pFile = fopen(filename, "r");
    if (pFile) { ret = 1; fclose(pFile); }
    return ret;
}

int FILEIO2::get_fileformat(char* filename)
{
    int ret = -1;
    char* ext = strrchr(filename, '.');
    if (strcmp(ext, ".xt") == 0)
    {
        ret = EXTENSION::XT;
    }
    else if (strcmp(ext, ".cwt") == 0)
    {
        ret = EXTENSION::CWT;
    }
    return ret;
}


FILEIO2::FILEIO2()
{
    p_xtfile = NULL;
    p_cwtfile = NULL;
    set_target(TARGET::GPU);
    set_compress_error(0.01f);
}

FILEIO2::~FILEIO2()
{
    if (this->p_xtfile) delete p_xtfile;
    if (this->p_cwtfile) delete p_cwtfile;
}

void FILEIO2::set_target(int _target)
{
    this->target = _target;
}

void FILEIO2::set_compress_error(float _error)
{
    this->compress_error = _error;
}

void FILEIO2::openForRead(char* filename)
{
    this->extension = get_fileformat(filename);
    if (this->extension == EXTENSION::XT)
    {
        this->p_xtfile = new XT_FILE();
        this->p_xtfile->openForRead(filename);
    }
    else if (extension == EXTENSION::CWT)
    {
        this->p_cwtfile = new CWT_FILE();
        this->p_cwtfile->set_wpc_param_target(this->target);
        this->p_cwtfile->openForRead(filename);
    }
}

void FILEIO2::openForWrite(char* filename, float cwt_error)
{
    this->extension = get_fileformat(filename);
    if (this->extension == EXTENSION::XT)
    {
        this->p_xtfile = new XT_FILE();
        this->p_xtfile->openForWrite(filename);
    }
    else if (extension == EXTENSION::CWT)
    {
        this->p_cwtfile = new CWT_FILE();
        this->p_cwtfile->set_wpc_param_error(this->compress_error);
        this->p_cwtfile->set_wpc_param_target(this->target);
        this->p_cwtfile->openForWrite(filename, cwt_error);
    }
}

void FILEIO2::createNew(char* filename_src, char* filename_dst)
{
    this->extension = get_fileformat(filename_dst);
    if (this->extension == EXTENSION::XT)
    {
        this->p_xtfile = new XT_FILE();
        this->p_xtfile->createNew(filename_src, filename_dst);
    }
    else if (extension == EXTENSION::CWT)
    {
        this->p_cwtfile = new CWT_FILE();
        this->p_cwtfile->set_wpc_param_error(this->compress_error);
        this->p_cwtfile->set_wpc_param_target(this->target);
        this->p_cwtfile->createNew(filename_src, filename_dst);
    }
}

void FILEIO2::createNew(char* filename_src, char* filename_dst, int dimx, int dimy, int dimz, int format)
{
    this->extension = get_fileformat(filename_dst);
    if (this->extension == EXTENSION::XT)
    {
        this->p_xtfile = new XT_FILE();
        this->p_xtfile->createNew(filename_src, filename_dst, dimx, dimy, dimz, format);
    }
    else if (extension == EXTENSION::CWT)
    {
        this->p_cwtfile = new CWT_FILE();
        this->p_cwtfile->set_wpc_param_error(this->compress_error);
        this->p_cwtfile->set_wpc_param_target(this->target);
        this->p_cwtfile->createNew(filename_src, filename_dst, dimx, dimy, dimz, format);
    }
}

int FILEIO2::get_dimx()
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->get_dimx();
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->get_dimx();
    }
}

int FILEIO2::get_dimy()
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->get_dimy();
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->get_dimy();
    }

}

int FILEIO2::get_dimz()
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->get_dimz();
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->get_dimz();
    }

}

int FILEIO2::get_format()
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->get_format();
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->get_format();
    }
}

int FILEIO2::get_sizeof()
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->get_sizeof();
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->get_sizeof();
    }
}

float FILEIO2::inlineRead(long no, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->inlineRead(no, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineRead(no, data);
    }
}

float FILEIO2::inlineManyRead(int no, int nbre, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->inlineManyRead(no, nbre, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineManyRead(no, nbre, data);
    }
}


float FILEIO2::inlineManyReadShort(int no, int nbre, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->inlineManyReadShort(no, nbre, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineManyRead(no, nbre, data);
    }
}



float FILEIO2::inlineWrite(int no, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->inlineWrite(no, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineWrite(no, data);
    }
}

float FILEIO2::inlineIncrementalWrite(int no, void* data)
{
    if (this->extension == EXTENSION::XT || this->extension == EXTENSION::ISO)
    {
        return this->p_xtfile->inlineIncrementalWrite(no, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineWrite(no, data); //it's the same
    }
    return -1.0f;
}

float FILEIO2::inlineManyWrite(int no, int nbre, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->inlineManyWrite(no, nbre, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        return this->p_cwtfile->inlineManyWrite(no, nbre, data);
    }
}

int FILEIO2::blockRead(int* x1, int* x2, void* data)
{
    if (this->extension == EXTENSION::XT)
    {
        return this->p_xtfile->blockRead(x1, x2, data);
    }
    else if (extension == EXTENSION::CWT)
    {
        // return this->p_cwtfile->blockRead(x1, x2, data);
    }
    return 0;
}