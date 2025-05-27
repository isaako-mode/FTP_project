#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "slice_lib.h"
#include "slice.h"

//METHODS FOR SLICE/BUFFER

int slc_str_cmp(Slice slc, const char* str) {
    char* str1 = malloc(sizeof(char) * slc.len + 1);
    memcpy(str1, slc.data, slc.len);

    str1[slc.len] = '\0';
    int result = strcmp(str1, str);
    free(str1);
    return result;

}