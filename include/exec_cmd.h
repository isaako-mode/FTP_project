#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser.h"
#include "slice.h"

char* exec_command(Message* message);
char* list(Slice current_dir);
int eval_cmd(Slice cmd);

#endif