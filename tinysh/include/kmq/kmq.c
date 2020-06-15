#include "kmq.h"

#if (TSH_SUPPORT_RTOS == 1)

BaseType_t kmq_init(kmq_t* q, int count)
{
    if (!q || count <= 0)
    {
        return -1;
    }

    q->_qid = xQueueCreate(count, sizeof(kmq_msg_t));
    if (!q->_qid)
    {
        return -2;
    }

    return 0;
}

void kmq_uninit(kmq_t* q)
{
    if (q && q->_qid)
    {
        vQueueDelete(q->_qid);
    }
}


BaseType_t kmq_send_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout)
{
    BaseType_t xHigherPriorityTaskWoken = 0;

    if (!q || !msg)
    {
        return -1;
    }

    if (!msg->isr)
    {
        if (xQueueSend(q->_qid, (char*)msg, timeout) != pdPASS)
        {
            return -2;
        }
    }
    else
    {
        if (xQueueSendFromISR(q->_qid, (char*)msg, &xHigherPriorityTaskWoken) != pdPASS)
        {
            return xHigherPriorityTaskWoken;
        }
    }

    return xHigherPriorityTaskWoken;
}


BaseType_t kmq_recv_msg(kmq_t* q, kmq_msg_t* msg, unsigned int timeout)
{
    if (!q || !msg)
    {
        return -1;
    }

    if (xQueueReceive(q->_qid, (char*)msg, timeout) != pdPASS)
    {
        return -2;
    }

    return 0;
}


BaseType_t kmq_quit(kmq_t* q, int isr, unsigned int timeout)
{
    kmq_msg_t msg;
    msg.id = KMQ_MSG_QUIT;
    msg.isr = isr;
    return kmq_send_msg(q, &msg, timeout);
}

BaseType_t kmq_run_loop(kmq_t* q, on_kmq_msg_procedure procedure, void* param)
{
    kmq_msg_t msg;
    if (!q || !procedure)
    {
        return -1;
    }

    while (0 == kmq_recv_msg(q, &msg, KMQ_WAIT_FOREVER))
    {
        if (msg.id == KMQ_MSG_QUIT)
        {
            break;
        }

        if (msg.callback && msg.callback(q, &msg, param))
        {
            continue;
        }

        procedure(q, &msg, param);
    }
    return 0;
}

void kmq_send_q(kmq_t* q, int msg_id, int isr, char* buf, int len, on_kmq_msg_callback_t callback)
{
    BaseType_t woken;
    kmq_msg_t msg;
    msg.isr = isr;
    msg.id = msg_id;
    msg.callback = callback;
    msg.len = len;
    msg.arg.pointer_value = buf;
    woken = kmq_send_msg(q, &msg, 0);
    portYIELD_FROM_ISR(woken);
}

#endif
