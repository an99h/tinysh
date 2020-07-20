#include "cli_cmd.h"
#include "csl/cs_linked_hash_map.h"

static int g_is_init = 0;
static cs_linkedhashmap_t g_hashmap;
static void _tsh_set_get_g_var(CLI_ARG* arg, int* var);

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
        switch (cli_struct->tsh_type)
        {
        case TSH_TYPE_FUNC:
            cli_struct->func(arg);
            break;
        case TSH_TYPE_VAR:
            _tsh_set_get_g_var(arg, cli_struct->g_var);
            break;
        default:
            break;
        }

        return 0;
    }

    return -1;
}

void tsh_cmd_add_impl(char* cmd, CLI_CMD* cli_struct)
{
    cs_linkedhashmap_t* hashMap = _GetInstance();
    cs_linkedhashmap_insert(hashMap, (void*)cmd, (void*)cli_struct);
}


void tsh_cmd_add(int tsh_type, char* cmd, cli_func func, int* g_var, char* desc)
{
    CLI_CMD* cli_struct = (CLI_CMD*)cs_malloc(sizeof(CLI_CMD));

    cli_struct->tsh_type = tsh_type;
    cli_struct->name = cmd;
    cli_struct->func = func;
    cli_struct->g_var = g_var;
    cli_struct->desc = desc;

    tsh_cmd_add_impl(cmd, (void*)cli_struct);
}

void tsh_cmd_iterator(int type)
{
    unsigned char i = 0;
    unsigned char count  = 0;
    cs_linkedhashmap_t* hashMap = _GetInstance();

    count = cs_linkedhashmap_get_count(hashMap);
    cs_linkedhashmap_bucket_t* bucket = cs_linkedhashmap_begin(hashMap);
    for (i = 0; i < count; i++)
    {
        CLI_CMD* cli_struct = (CLI_CMD*)bucket->val;
        char c = 0;
        switch (type)
        {
        case TSH_TYPE_ALL:
        {
            switch (cli_struct->tsh_type)
            {
            case TSH_TYPE_FUNC:
                c = 'c';
                break;
            case TSH_TYPE_VAR:
                c = 'v';
                break;
            default:
                break;
            }

            tsh_print("-%c- %-15s---  %s\r\n", c, cli_struct->name, cli_struct->desc);
            break;
        }
        case TSH_TYPE_VAR:
            if (cli_struct->tsh_type == TSH_TYPE_VAR)
            {
                tsh_print("%s = %d\r\n", cli_struct->name, *cli_struct->g_var);
            }
            break;
        default:
            break;
        }


        bucket = cs_linkedhashmap_next(bucket);
    }
}


static void _tsh_set_get_g_var(CLI_ARG* arg, int* var)
{
    if (arg->argc == 1)
    {
        tsh_print("%s = %d\r\n", arg->argv[0], *var);
    }
    else if (arg->argc == 3 && arg->argv[1][0] == '=' && strlen(arg->argv[1]) == 1)
    {
        int t = (int)atoi(arg->argv[2]);
        *var = t;
    }
    else
    {
        tsh_print("error parameter, eg: var = 1\r\n");
    }
}

