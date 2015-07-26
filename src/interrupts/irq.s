.macro IRQ irqnr intnr
  .global irq\irqnr
  irq\irqnr:
    cli
    push $0
    push $\intnr
    jmp irq_common_stub
.endm

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

.extern irq_handler
# This is our common IRQ stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame. 
irq_common_stub:
    push %ds
    push %es
    push %fs
    push %gs
    
    mov %ds, %ax
    pusha
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    call irq_handler
    popa
    
    pop %gs
    pop %fs
    pop %es
    pop %ds
    
    addl $0x8, %esp
    sti
    iret
  