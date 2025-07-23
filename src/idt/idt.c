#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "io/io.h"
struct idt_desc idt_descriptors[PEACHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void int21h();

extern void int20h();

extern void no_interrupt();

extern void idt_load(struct idtr_desc* ptr);

void int21h_handler()
{
    print("keyboard pressed");
    outb(0x20,0x20); //acknoledge
}
void int20h_handler()
{
    print("timer interrupt");
    outb(0x20,0x20); //acknoledge
}

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address){
    struct idt_desc * desc = &idt_descriptors[interrupt_no];
    desc-> offset_1 = (uint32_t) address & 0x0000ffff;
    desc-> selector = KERNEL_CODE_SELECTOR;
    desc-> zero = 0; 
    desc-> type_attr = 0xEE; // set many bitwise values, include ring 3 = 0b11
    desc-> offset_2 = (uint32_t) address >> 16; 
    //the higher part of the offset
    
}
void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;
    //we need to set a hanlder because if they happen and there is no handler there it will crash
    for (int i =0; i< PEACHOS_TOTAL_INTERRUPTS; i++){
        idt_set(i,no_interrupt);
    }
    idt_set(0x20,int20h);
    idt_set(0x21,int21h);

   
    //remove these 2 to test other heap things
    //idt_set(0, idt_zero);
    //idt_set(0x20,int21h);

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}
void no_interrupt_handler(){
    outb(0x20, 0x20);
    //ack interrupt
    //for key pressed we still need to read the key pressed
}