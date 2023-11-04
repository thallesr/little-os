#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <idt.h>

#define INITIAL_VIDEO_MEMORY 0xB8000 
#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();

void terminal_initialize();

void write(char * content, int lineStart, int columnStart);


uint16_t terminal_make_char(char character,char colour);

#endif
