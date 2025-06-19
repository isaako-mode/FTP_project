#ifndef BUILD_OUTPUT_H
#define BUILD_OUTPUT_H

int configure_output_buff(Message* message, Buffer* buffer, user_cmd input_cmd);
void set_buf_len(Message* message, Buffer* buffer);
int build_list_rsp(Message* message, Buffer* buffer); 

#endif