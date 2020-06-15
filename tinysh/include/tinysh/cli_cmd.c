#include "tinysh/cli_cmd.h"
#include "csl/cs_linked_hash_map.h"

static int g_is_init = 0;
static cs_linkedhashmap_t g_hashmap;

static cs_linkedhashmap_t* _GetInstance(void)
{
    if (!g_is_init)
    {
        g_is_init = 1;
        cs_linkedhashmap_init(&g_hashmap, TSH_MAX_CMD_COUNT);
    }
    return &g_hashmap;
}

int tsh_cmd_despatcher(char* cmd, CLI_ARG* arg)
{
    cs_linkedhashmap_t* hashMap = _GetInstance();
    CLI_CMD* cli_struct = (CLI_CMD*)cs_linkedhashmap_get(hashMap, (void*)cmd);
    if (cli_struct)
    {
        cli_struct->func(arg);
        return 0;
    }

    return -1;
}

void tsh_cmd_add_impl(char* cmd, CLI_CMD* cli_struct)
{
    cs_linkedhashmap_t* hashMap = _GetInstance();
    cs_linkedhashmap_insert(hashMap, (void*)cmd, (void*)cli_struct);
}


void tsh_cmd_add(char* cmd, cli_func func, char* desc)
{
    CLI_CMD* cli_struct = (CLI_CMD*)cs_malloc(sizeof(CLI_CMD));

    cli_struct->name = cmd;
    cli_struct->func = func;
    cli_struct->desc = desc;

    tsh_cmd_add_impl(cmd, (void*)cli_struct);
}

void tsh_cmd_iterator(void)
{
    unsigned char i = 0;
    unsigned char count  = 0;
    cs_linkedhashmap_t* hashMap = _GetInstance();

    count = cs_linkedhashmap_get_count(hashMap);
    cs_linkedhashmap_bucket_t* bucket = cs_linkedhashmap_begin(hashMap);
    for (i = 0; i < count; i++)
    {
        CLI_CMD* cli_struct = (CLI_CMD*)bucket->val;
        tsh_print("%10s  ---  %s\r\n", cli_struct->name, cli_struct->desc);
        bucket = cs_linkedhashmap_next(bucket);
    }
}
