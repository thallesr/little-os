section .asm
extern int21h_handler
extern no_interrupt_handler

global idt_load
global int21h
global no_interrupt
idt_load:
    push ebp
    mov ebp, esp

    ;boiler code to be c compatible


    ;just ebp , top of the stack
    ;ebp +4 return address for caller of this
    ;ebp +8 first arg
    mov ebx, [ebp+8]
    lidt [ebx]

    ;


    pop ebp
    ret

int21h:
    pushad ; push all general purpose register
    cli
    call int21h_handler



    popad ; pop all general purpose register
    sti
    iret

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret