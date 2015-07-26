#include "display.h"
#include "idt.h"
#include "io.h"
#include "isr.h"
#include "util.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

const char *exception_messages[] = 
{
    "Division By Zero", // 0
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds", // 5
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment",
    "Bad TSS", // 10
    "Segment Not Present",
    "Stack Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt", // 15
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check", // 18
    
    "Reserved", // 19
    "Reserved", // 20
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved", // 25
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved", // 30
    "Reserved"  // 31
};

void isr_handler(registers_t regs)
{
    terminal_writestring("Received interrupt ");
    terminal_writedec(regs.int_no);
    terminal_writestring("\n");
    if (regs.int_no < 32)
    {
        terminal_writestring(exception_messages[regs.int_no]);
        terminal_writestring(" Exception. System Halted!\n");
        for (;;);
        //debug_register(regs);
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
   
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= IRQ8)
   {
       // Send reset signal to slave.
       outportb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outportb(0x20, 0x20);
}
