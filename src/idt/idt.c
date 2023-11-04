#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"

struct  idt_desc idt_descriptors[PEACH_OS_TOTAL_INTERRUPT];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc * ptr);

void idt_zero()
{
    write("divide by zero error \n",1,1);
}

void idt_set(int interrupt_no, void* address){
    struct idt_desc * desc = &idt_descriptors[interrupt_no];
    desc-> offset_1 = (uint32_t) address & 0x0000ffff;
    desc-> selector = KERNEL_CODE_SELECTOR;
    desc-> zero = 0; 
    desc-> type_attr = 0xEE; // set many bitwiser values, include ring 3 = 0b11
    desc-> offset_2 = (uint32_t) address >> 16; 
    //the higher part of the offset
    
}
void idt_init(){
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;    

    idt_set(0, idt_zero);

    idt_load(&idtr_descriptor);

}
