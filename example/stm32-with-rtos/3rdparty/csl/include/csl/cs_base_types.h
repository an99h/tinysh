/**
 * @file
 * @brief 常用数据类型
 * @author yaoyuliang(765495939@qq.com)
 * @copyright SAIZHI.inc
 * @date 2019-03-29
 */

#ifndef CS_BASE_TYPES_H_B6379C10_D27B_457E_84AF_FF4A3248BF20
#define CS_BASE_TYPES_H_B6379C10_D27B_457E_84AF_FF4A3248BF20

#if defined(LINUX_DRIVER) || defined(module_init)
#include <linux/types.h>
#ifndef BASE_TYPES_7F564792_D784_414E_9F0E_36F1221AD653
#define BASE_TYPES_7F564792_D784_414E_9F0E_36F1221AD653
typedef char                CHAR;
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;
typedef unsigned long long  UINT64;
typedef long long           INT64;
#endif
#endif

#ifndef __cplusplus
#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
#endif

#if defined(_WIN32)
#include <stddef.h>
#elif defined(__VXWORKS__)
#include <vxworks.h>
#include <string.h>
#endif

/* 一些常用的数据类型, 如UINT32, size_t, bool等的头文件 */
#endif
