#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser.h"
#include "slice.h"

char* exec_command(Message* message, Buffer* message_buffer);
void list(Message* message);
int eval_cmd(Slice cmd);

#endif