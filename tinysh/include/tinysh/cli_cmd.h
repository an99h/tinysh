#ifndef __CLI_CMD_H__
#define __CLI_CMD_H__
#include "tsh_main/tsh_config.h"

#define  TSH_TYPE_ALL   0
#define  TSH_TYPE_FUNC  1
#define  TSH_TYPE_VAR   2


typedef struct
{
    int  argc;
    char argv[TSH_MSG_ARGC][TSH_MSG_SIZE];
} CLI_ARG;

typedef void(*cli_func)(CLI_ARG* arg);

typedef struct
{
    char* name;         // ��������
    int tsh_type;       // �������� CLI ȫ�ֱ���
    cli_func func;      // cli �������
    int *g_var;         // ȫ�ֱ���ָ��
    char* desc;         // ��������
} CLI_CMD;


void tsh_cmd_iterator(int type);
int  tsh_cmd_despatcher(char* cmd, CLI_ARG* arg);
void tsh_cmd_add(int tsh_type, char *cmd, cli_func func, int *g_var, char *desc);


#define TSH_CMD_ADD_CLI(_cmd, _func, _desc) tsh_cmd_add(TSH_TYPE_FUNC, _cmd, _func, NULL, _desc)

#define TSH_CMD_ADD_VAR_IMPL(_cmd, _g_var, _desc) tsh_cmd_add(TSH_TYPE_VAR, _cmd, NULL, &_g_var, _desc)
#define TSH_CMD_ADD_VAR(_g_var) TSH_CMD_ADD_VAR_IMPL(#_g_var, _g_var, "get/set "#_g_var)

#endif
