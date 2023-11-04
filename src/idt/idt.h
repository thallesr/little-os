#ifndef IDT_H
#define IDT_H
#include <stdint.h>


void idt_init();


struct idt_desc
{
    uint16_t offset_1; //offset bits 0-15
    uint16_t selector; // selector that of GDT
    uint8_t zero; // will be unused for now , set 0
    uint8_t type_attr; // descriptor type and attr
    uint16_t offset_2; // offset bits 16-31

} __attribute__((packed)); // apparently blocks from changing order on compiler ?

struct idtr_desc
{
    uint16_t limit; // last address of descriptor table
    uint16_t base; //base address, start of interrupt descript table
} __attribute__((packed)); // apparently blocks from changing order on compiler ?


#endif