#include "pparser.h"
#include "string/string.h"
#include "kernel.h"
#include "memory/memory.h"

static int path_valid_format(const char* filename)
{
    int len = strnlen(filename,PEACH_OS_MAX_PATH);
    return (len >=3 && is_digit(filename[0]) && memcmp((void*) &filename[1],":/",2)==0);
}