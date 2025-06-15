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

#define OK "200"
#define BAD_REQUEST "400"

int build_list_rsp(Message* message, Buffer*, buffer) {
	
}


int configure_output_buff(Message* message, Buffer* buffer, user_cmd input_cmd) {

	// Setup surrounding newlines for output
	buffer->data[0] = '\n';
	buffer->data[buffer->buf_len] = '\n';

	// increment to skip over the newline
	buffer->data += 1;

	// Copy 200 OK response to the cmd position of the buffer
	memcpy(buffer->data, OK, strlen(OK));

	buffer->data[strlen(OK) + 1] = '\0';

	switch(input_cmd) {
	case LIST:
		build_list_rsp(message, buffer);
	}
}