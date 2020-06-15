#ifndef __KMQ_NOOS_H__
#define __KMQ_NOOS_H__
#include "tsh_main/tsh_config.h"

#if (TSH_SUPPORT_RTOS == 0)

#include "csl/cs_list.h"

typedef union
{
    char          char_value;
    unsigned char uchar_value;
    int           int_value;
    unsigned int  uint_value;
    long          long_value;
    unsigned long ulong_value;
    void* pointer_value;
    double        double_value;
    float         float_value;
} MSG_VALUE_T;

typedef struct
{
    CS_LIST_NODE_OBJECT;
    unsigned char   msg_type;
    unsigned char   data_len;
    char* data;
    MSG_VALUE_T arg;
} ListNode;

int MsgQ_Init(void);
bool MsgQ_SendOneNode(int msg_type, char* data, unsigned char len);
ListNode* MsgQ_Recv(void);
#endif

#endif
