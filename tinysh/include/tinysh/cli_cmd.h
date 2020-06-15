#ifndef __CLI_CMD_H__
#define __CLI_CMD_H__
#include "tsh_main/tsh_config.h"

typedef struct
{
    int  argc;
    char argv[TSH_MSG_ARGC][TSH_MSG_SIZE];
} CLI_ARG;

typedef void(*cli_func)(CLI_ARG* arg);

typedef struct
{
    char* name;         // shell ÃüÁîÃû³Æ
    cli_func func;      // shell ÃüÁîº¯Êı
    char* desc;         // shell ÃüÁîÃèÊö
} CLI_CMD;


void tsh_cmd_iterator(void);
int  tsh_cmd_despatcher(char* cmd, CLI_ARG* arg);
void tsh_cmd_add(char *cmd, cli_func func, char *desc);

#endif
