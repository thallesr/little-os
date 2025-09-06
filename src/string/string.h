#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
#include <stddef.h>

size_t strlen(const char* str);
size_t strnlen(const char* str,int max);

bool is_digit(char c);
int to_numeric_digit(char c);
char * strcpy(char * dest, const char* src);


#endif