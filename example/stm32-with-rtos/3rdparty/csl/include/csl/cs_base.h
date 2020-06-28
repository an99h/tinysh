#ifndef CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47
#define CS_BASE_H_718F1719_5495_4643_AC44_A3DA45190E47

#define CSL_VERSION 0x030000
#define CSL_VERSION_STRING "3.0.0"

#include "cs_base_types.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tsh_main/tsh_config.h"
#if (TSH_SUPPORT_RTOS == 1)
#include "FreeRTOS.h"
#define cs_malloc pvPortMalloc
#define cs_free vPortFree
#else
#define cs_malloc malloc
#define cs_free free
#endif

#endif
