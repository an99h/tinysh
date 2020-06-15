#include "kmq_noos.h"

#if (TSH_SUPPORT_RTOS == 0)

cs_list_t msg_send;
cs_list_t msg_recv;

int  MsgQ_Init(void)
{
    unsigned char i = 0;

    cs_list_init(&msg_send);
    cs_list_init(&msg_recv);

    for (; i < TSH_MSG_COUNT; i++)
    {
        ListNode* node = (ListNode*)cs_list_malloc_node(sizeof(ListNode));
        if (node == NULL)
        {
            tsh_print("create list error\r\n");
            return -1;
        }
        cs_list_push_back(&msg_send, node);
    }

    return 0;
}


bool MsgQ_Send(ListNode* send_node)
{
    unsigned char i = 100;
    ListNode* node = NULL;

    node = (ListNode*)cs_list_peek_front(&msg_send);
    while (node == NULL && --i > 0)
    {
        node = (ListNode*)cs_list_peek_front(&msg_send);
    }

    if (i != 0)
    {
        node = cs_list_pop_front(&msg_send);
        memcpy(node, send_node, sizeof(ListNode));
        cs_list_push_back(&msg_recv, node);
        return true;
    }

    return false;
}


bool MsgQ_SendOneNode(int msg_type, char* data, unsigned char len)
{
    ListNode send_node;

    send_node.data = data;
    send_node.data_len = len;
    send_node.msg_type = msg_type;

    return MsgQ_Send(&send_node);
}

ListNode* MsgQ_Recv(void)
{
    ListNode* node = NULL;

    node = (ListNode*)cs_list_peek_front(&msg_recv);
    if (node == NULL)
    {
        return NULL;
    }

    return cs_list_pop_front(&msg_recv);
}

#endif
