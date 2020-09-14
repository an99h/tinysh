#include "tsh_main/tsh_config.h"
#include "csl/cs_object_pool.h"
#include "csl/cs_memwriter.h"

#if (TSH_SUPPORT_RTOS == 1)
#include "kmq/kmq.h"
#else
#include "kmq/kmq_noos.h"
extern cs_list_t msg_send;
#endif

#include "tinysh.h"
#include "cli_cmd.h"

#define LF                  0x0a
#define CR                  0x0d
#define TSH_NAME_LEN        10
u8 tsh_login = !(TSH_PASSWORD_ON);
char tsh_name[TSH_NAME_LEN] = {'T', 'S', 'H'};
void* msg_pool;

#if (TSH_SUPPORT_RTOS == 1)
kmq_t tsh_uart_q;
#endif

static void _tsh_get_opt(void* str, int len, CLI_ARG* arg);
static void _tsh_do_cmd(void* msg, int msg_len);
static void _tsh_set_history(char* cmd, int len);
static int  _tsh_get_history(unsigned char key_direction, char* cmd);
static bool _tsh_check_is_login(CLI_ARG* arg);
static void _tsh_set_user_name(char* name);
extern void shell_add_cmd(void);

int tsh_init(char *uname)
{
    tsh_uart_init();

#if (TSH_SUPPORT_RTOS == 1)
    kmq_init(&tsh_uart_q, TSH_MSG_COUNT);
#else
    MsgQ_Init();
#endif

    msg_pool = cs_object_pool_init(TSH_MSG_COUNT, TSH_MSG_SIZE, NULL);
    if (msg_pool <= 0)
    {
        tsh_print("object pool init fail\r\n");
        return -1;
    }

    if (uname != NULL)
    {
        _tsh_set_user_name(uname);
    }

    shell_add_cmd();

    return 0;
}


#if (TSH_SUPPORT_RTOS == 1)

void tsh_cli_msg_procedure(const kmq_t* q, const kmq_msg_t* msg, void* param)
{
    if (!q || !msg)
    {
        return;
    }

    switch (msg->id)
    {
    case TSH_MSG_ID_CLI:
        _tsh_do_cmd(msg->arg.pointer_value, msg->len);
    default:
        break;
    }

    tsh_disable_interrupt();
    cs_object_pool_recycle(msg->arg.pointer_value);
    tsh_enable_interrupt();
}

void tsh_uart_task(void* param)
{
    int ret = 0;

    tsh_print("start tsh run loop\r\n");

    ret = kmq_run_loop(&tsh_uart_q, tsh_cli_msg_procedure, 0);
    if (ret)
    {
        tsh_print("run tsh queue loop error\r\n");
    }

    kmq_uninit(&tsh_uart_q);

    if (msg_pool)
    {
        cs_object_pool_uninit(msg_pool);
        msg_pool = NULL;
    }
}
#else
void tsh_cli_msg_procedure(void)
{
    ListNode* node = NULL;

    node = MsgQ_Recv();
    if (node == NULL)
    {
        return;
    }

    switch (node->msg_type)
    {
    case TSH_MSG_ID_CLI:
        _tsh_do_cmd(node->data, node->data_len);
    default:
        break;
    }

    if (node->data)
    {
        cs_object_pool_recycle(node->data);
    }

    tsh_disable_interrupt();
    cs_list_push_back(&msg_send, node);
    tsh_enable_interrupt();
}
#endif

static void _tsh_do_cmd(void* msg, int msg_len)
{
    CLI_ARG arg;

    if (!msg)
    {
        return;
    }

    if (tsh_login)
    {
        _tsh_set_history((char*)msg, msg_len);
    }

    _tsh_get_opt(msg, msg_len, &arg);

    if (!_tsh_check_is_login(&arg))
    {
        goto EXIT;
    }

    if (msg_len != 0 && tsh_cmd_despatcher((char*)arg.argv[0], &arg) != 0)
    {
        tsh_print("tsh: command not found: %s\r\n", arg.argv[0]);
    }
EXIT:
    tsh_print("%s:)", tsh_name);
    //fflush(stdout);
}

static void _tsh_get_opt(void* str, int len, CLI_ARG* arg)
{
    int str_len = 0;
    char* ptr = NULL;
    char* retptr = NULL;

    memset(arg, 0, sizeof(CLI_ARG));

    if (len == 0)
    {
        return;
    }

    ptr = str;

    while ((retptr = strtok(ptr, " ")) != NULL)
    {
        str_len = strlen(retptr);
        memcpy(arg->argv[arg->argc++], retptr, str_len);
        if (arg->argc == TSH_MSG_ARGC + 1)
        {
            arg->argc = TSH_MSG_ARGC;
            tsh_print("warning:input argv number > %d\r\n", TSH_MSG_ARGC);
            return;
        }
        ptr = NULL;
    }
}

static void _tsh_set_user_name(char* name)
{
    int len = strlen(name);
    if (len > TSH_NAME_LEN)
    {
        len = TSH_NAME_LEN;
    }
    memcpy(tsh_name, name, len);
}

bool _tsh_check_is_login(CLI_ARG* arg)
{
    if ((tsh_login == true) && (strcmp(arg->argv[0], "logout") == 0 && arg->argc == 1))
    {
        tsh_login = false;
        tsh_print("logout success!\r\n");
        return false;
    }

    if (tsh_login == false)
    {
        if (strcmp(arg->argv[0], TSH_PASSWORD) == 0 && arg->argc == 1)
        {
            tsh_login = true;
            tsh_print("\r\nlogin success!\r\n");
        }
        else if (strlen(arg->argv[0]) == 0)
        {
            tsh_print("login!\r\n");
        }
        else
        {
            tsh_print("\r\nlogin error!!!\r\n");
        }
        return false;
    }

    return true;
}


void tsh_uart_irq(uart_dev_t* uart)
{
    static cs_memwriter_t mem;
    static char* cache = NULL;
    char ch = 0;
    int cur_len = 0;
    unsigned char stop_flag = 0;
    static int key_direction = 0;

    if (tsh_uart_recv_byte(uart, &ch) != 0)
    {
        return;
    }
	
    if (!cache)
    {
        cache = (char*)cs_object_pool_obtain(msg_pool);
        if (cache == NULL)
        {
            return;
        }
        cs_memwriter_attach(&mem, cache, TSH_MSG_SIZE);
    }

    cur_len = cs_memwriter_get_cursor(&mem);
    if ((cur_len >= TSH_MSG_SIZE-2) && ch != LF && ch != CR && ch != 0x08)
    {
        return;
    }

    if (ch == 0x1b || key_direction != 0) // ·½Ïò¼ü
    {
        unsigned char i = 0;
        unsigned char old_len = 0;
        char cmd[32] = { 0 };
        key_direction++;
        if (key_direction == 3)
        {
            key_direction = 0;
            if (_tsh_get_history(ch, cmd))
            {
                old_len = cs_memwriter_get_cursor(&mem);
                for (i = 0; i < old_len; i++)
                {
                    tsh_uart_send_data(uart, "\b \b", 3);
                }
                cs_memwriter_seek(&mem, 0);
                cs_memwriter_push_back_binary(&mem, &cmd, strlen(cmd));
                tsh_uart_send_data(uart, cmd, strlen(cmd));
            }
        }
        return;
    }

    if (ch != 0x08)  // !É¾³ý¼ü
    {
		if (ch == 0x09) // !TAP¼ü
        {
            return;
        }
		
        if (tsh_login)
        {
            tsh_uart_send_byte(uart, ch);
            if (ch == LF || ch == CR)
            {
                tsh_uart_send_data(uart, "\r\n", 2);
            }
        }
        else
        {
            if (ch != LF && ch != CR)
            {
                tsh_uart_send_byte(uart, '*');
            }
        }
        cs_memwriter_push_back_binary(&mem, &ch, sizeof(unsigned char));
        cur_len = cs_memwriter_get_cursor(&mem);
    }
    else
    {
        cur_len = cs_memwriter_get_cursor(&mem);
        if (cur_len - 1 >= 0)
        {
            tsh_uart_send_data(uart, "\b \b", 3);
            cs_memwriter_seek(&mem, cur_len - 1);
            cache[cur_len - 1] = 0;
        }
        return;
    }

    if (ch == LF || ch == CR)
    {
        //È¥³ý½áÊø·û \r \n
        cache[cur_len - 1] = 0;
        cur_len -= 1;
        stop_flag = 1;
    }

    if (stop_flag)
    {
#if (TSH_SUPPORT_RTOS == 1)
        kmq_send_q(&tsh_uart_q, TSH_MSG_ID_CLI, 1, cache, cur_len, NULL);
#else
        MsgQ_SendOneNode(TSH_MSG_ID_CLI, cache, cur_len);
#endif
        cs_memwriter_seek(&mem, 0);
        cache = NULL;
    }
}

///////////////////shell history///////////////////////////////
typedef struct
{
    unsigned char index;
    unsigned char counts;
    char shell_history[TSH_HISTORY_COUNT][TSH_MSG_SIZE];
}SHELL_HISTORY_T;

SHELL_HISTORY_T cmd_history =
{
    .index = 0,
    .counts = 0,
    .shell_history = {{0},{0}},
};

static void _tsh_set_history(char* cmd, int len)
{
	cmd_history.index = 0;
	
    if (len <= 0)
    {
        return;
    }
    /* if current cmd is same as last cmd, don't push */
    if (memcmp(cmd_history.shell_history[cmd_history.counts - 1], cmd, len) == 0)
    {
        return;
    }
    
    memcpy(cmd_history.shell_history[cmd_history.counts++], cmd, len);

    if (cmd_history.counts == TSH_HISTORY_COUNT)
    {
        unsigned char i = 0;
        for (; i < TSH_HISTORY_COUNT - 1; i++)
        {
            memset(cmd_history.shell_history[i], 0, TSH_MSG_SIZE);
            memcpy(cmd_history.shell_history[i], cmd_history.shell_history[i + 1],
                strlen(cmd_history.shell_history[i + 1]));
        }

        memset(cmd_history.shell_history[i], 0, TSH_MSG_SIZE);
        cmd_history.counts = TSH_HISTORY_COUNT - 1;
    }
}

static int _tsh_get_history(unsigned char key_direction, char* cmd)
{
    switch (key_direction)
    {
    case 0x41:
        if (cmd_history.counts > 0 && cmd_history.index < cmd_history.counts)
        {
            memcpy(cmd, cmd_history.shell_history[cmd_history.counts - cmd_history.index - 1],
                sizeof(cmd_history.shell_history[cmd_history.counts - cmd_history.index - 1]));
            cmd_history.index++;
            return 1;
        }
        break;
    case 0x42:
        if (cmd_history.counts > 0 && cmd_history.index > 0)
        {
            memcpy(cmd, cmd_history.shell_history[cmd_history.counts - cmd_history.index + 1],
                sizeof(cmd_history.shell_history[cmd_history.counts - cmd_history.index + 1]));
            cmd_history.index--;
            return 1;
        }
        break;
    default:
        break;
    }
    return 0;
}
