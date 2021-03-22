#include "cs_memwriter.h"

#ifdef __cplusplus
extern "C" {
#endif

int cs_memwriter_attach(cs_memwriter_t* mem, void* buffer, const int bytes)
{
    if (mem && buffer && bytes >= 0)
    {
        mem->_data = (char*)buffer;
        mem->_size = bytes;
        mem->_cur_idx = 0;
        return 0;
    }
    return -1;
}

int cs_memwriter_seek(cs_memwriter_t* mem, const int index)
{
    if (mem && (index >=0) && (index < mem->_size))
    {
        mem->_cur_idx = index;
        return 0;
    }
    return -1;
}

int cs_memwriter_skip(cs_memwriter_t* mem, const int step)
{
    int new_idx = -1;

    if (!mem)
    {
        return -1;
    }

    new_idx = mem->_cur_idx + step;
    if ((new_idx < 0) || (new_idx >= mem->_size))
    {
        return -2;
    }

    mem->_cur_idx = new_idx;
    return 0;
}

int cs_memwriter_get_cursor(cs_memwriter_t* mem)
{
    return mem ? mem->_cur_idx : 0;
}

int cs_memwriter_push_back_binary(cs_memwriter_t* mem, const void* buffer, const int bytes)
{
    int new_idx = -1;

    if (!mem || !buffer || bytes <= 0)
    {
        return -1;
    }

    new_idx = mem->_cur_idx + bytes;
    if (new_idx > mem->_size)
    {
        return -2;
    }

    memcpy(&mem->_data[mem->_cur_idx], buffer, bytes);
    mem->_cur_idx = new_idx;
    return 0;
}

void* cs_memwriter_pop_front(cs_memwriter_t* mem, const int bytes)
{
    char* p = NULL;
    int new_idx = -1;

    if (!mem || bytes <= 0)
    {
        return NULL;
    }

    new_idx = mem->_cur_idx + bytes;
    if (new_idx > mem->_size)
    {
        return NULL;
    }

    p = &mem->_data[mem->_cur_idx];
    mem->_cur_idx = new_idx;
    return p;
}

void* cs_memwriter_pop_back(cs_memwriter_t* mem, const int bytes)
{
    char* p = NULL;
    int new_idx = -1;

    if (!mem || bytes <= 0)
    {
        return NULL;
    }

    new_idx = mem->_cur_idx - bytes;
    if (new_idx < 0)
    {
        return NULL;
    }

    mem->_cur_idx = new_idx;
    p = &mem->_data[mem->_cur_idx];
    return p;
}

void* cs_memwriter_get_buffer(cs_memwriter_t* mem, size_t* size)
{
    if (mem)
    {
        if (size)
        {
            *size = mem->_size;
        }
        return mem->_data;
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif
