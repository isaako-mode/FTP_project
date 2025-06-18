#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
#include<fcntl.h> 
#include "slice_lib.h"
#include "parser.h"
#include "slice.h"
#include "buffer.h"
#include "command_names.h"
#include "slice_lib.h"
#include "user_cmd.h"

#define OK "200"
#define BAD_REQUEST "400"

int build_list_rsp(Message* message, Buffer* buffer) {
	// \n200.\r\n\r\nFILES\r\n
		for(size_t i=0; i< buffer->buf_len; i++) {
	// message->file_data.offset
			printf("%c", buffer->data[i]);
			if(buffer->data[i] == '\0' && buffer->data[i+1] == '\0') {
				buffer->data[i] = '\r';
				buffer->data[i+1] = '\n';
		
		}	
	}
	return 200;
}


int configure_output_buff(Message* message, Buffer* buffer, user_cmd input_cmd) {

	printf("%s", message->command.data);
	
	memset(message->command.data, '\0', message->command.len);
	memcpy(message->command.data, OK, strlen(OK));
	// Setup surrounding newlines for output
	buffer->data[0] = '\n';
	buffer->data[buffer->buf_len] = '\n';

	// increment to skip over the newline
	buffer->data += 1;

	// Copy 200 OK response to the cmd position of the buffer
	// memcpy(buffer->data, OK, strlen(OK));

	buffer->data[strlen(OK) + 1] = '\0';

	switch(input_cmd) {
	case CMD_LIST:
		build_list_rsp(message, buffer);
	}


    for (size_t i = 0; i < buffer->buf_len; i++) {
        printf("IN THE FUNC [%02zu] = 0x%02x (%c)\n", i,
        (unsigned char)buffer->data[i],
        buffer->data[i] >= 32 &&
        buffer->data[i] <= 126 ?
        buffer->data[i] : '.');
    }
	return 200;
}