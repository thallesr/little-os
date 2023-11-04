section .asm

global idt_load
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