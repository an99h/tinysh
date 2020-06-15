#ifndef CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47
#define CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47

#define CSL_VERSION 0x030000
#define CSL_VERSION_STRING "3.0.0"

#include "cs_base_types.h"

#if defined(_WIN32)
#include <assert.h>
#include <string.h>
#if defined(WIN_DRIVER) || defined(_NTDDK_) || defined(_WDF_H_) || defined(WDFAPI)
#include <wdm.h>
#define cs_malloc(size) ExAllocatePoolWithTag(NonPagedPool, (size), 'cshm')
#define cs_free(ptr) ExFreePoolWithTag(ptr, 'cshm')
#else
#include <stdlib.h>
#define cs_malloc malloc
#define cs_free free
#endif
#elif defined(__VXWORKS__)
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#define cs_malloc malloc
#define cs_free free
#elif defined(LINUX_DRIVER) || defined(module_init)
#include<linux/slab.h>
#define assert(cond)                                              \
{                                                                 \
    if (!(cond))                                                  \
       printk(KERN_ALERT "ASSERT: %s(%u)\n", __FILE__, __LINE__); \
}
#define cs_malloc(size) kmalloc((size), GFP_KERNEL)
#define cs_free kfree
#elif defined(SYLIXOS)
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define cs_malloc malloc
#define cs_free free
#elif defined(TINYSH_RTOS)
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#define cs_malloc pvPortMalloc
#define cs_free vPortFree
#elif defined(TINYSH_NOOS)
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#define cs_malloc malloc
#define cs_free free
#else
#error "csl lib does not support on your platform"
#endif

/* 供谷歌单元测试框架使用 */
#ifdef _USE_GOOGLE_UNIT_TESTING
extern void* _test_malloc(const size_t size, const char* file, const int line);
extern void* _test_calloc(const size_t number_of_elements, const size_t size, const char* file, const int line);
extern void _test_free(void* const ptr, const char* file, const int line);
#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
#endif

#endif
