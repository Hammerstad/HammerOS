#ifndef INTERRUPTS_COMMON_H
#define	INTERRUPTS_COMMON_H

#include <stdint.h>

typedef struct registers
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t gs, fs, es, ds;      /* pushed the segs last */
    uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} registers_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
/*
void debug_register(registers_t regs){
    terminal_writestring("INTERRUPT RECEIVED:\n");
    terminal_writestring("Interrupt number: ");
    terminal_writedec(regs.int_no);
    terminal_writestring("\nInterrupt error code: ");
    terminal_writedec(regs.err_code);
    
    terminal_writestring("\nEDI:");
    terminal_writedec(regs.edi);
    terminal_writestring(" ESI:");
    terminal_writedec(regs.esi);
    terminal_writestring(" EBP:");
    terminal_writedec(regs.ebp);
    terminal_writestring(" ESP:");
    terminal_writedec(regs.esp);
    terminal_writestring(" EAX:");
    terminal_writedec(regs.eax);
    terminal_writestring(" EBX:");
    terminal_writedec(regs.ebx);
    terminal_writestring(" ECX:");
    terminal_writedec(regs.ecx);
    terminal_writestring(" EDX:");
    terminal_writedec(regs.edx);
    
    terminal_writestring("\nDS:");
    terminal_writedec(regs.ds);
    terminal_writestring(" ES:");
    terminal_writedec(regs.es);
    terminal_writestring(" FS:");
    terminal_writedec(regs.fs);
    terminal_writestring(" GS:");
    terminal_writedec(regs.gs);
    
    terminal_writestring("\nEIP:");
    terminal_writedec(regs.eip);
    terminal_writestring(" CS:");
    terminal_writedec(regs.cs);
    terminal_writestring(" EFLAGS:");
    terminal_writedec(regs.eflags);
    terminal_writestring(" USERESP:");
    terminal_writedec(regs.useresp);
    terminal_writestring(" SS:");
    terminal_writedec(regs.ss);
    
    terminal_writestring("\n");
}*/

#endif	/* INTERRUPTS_COMMON_H */