#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser.h"
#include "slice.h"
#include "user_cmd.h"

user_cmd exec_command(Slice* dest, Message* message);
int list(Message* message);
int eval_cmd(Slice cmd);

#endif