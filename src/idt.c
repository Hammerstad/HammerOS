#include <stdint.h>

#include "display.h"
#include "idt.h"
#include "util.h"

static void idt_set_gate(uint8_t,uint32_t,uint16_t,uint8_t);
extern void idt_flush(uint32_t);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void init_idt()
{
    terminal_writestring("Initiating IDT...          ");
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (uint32_t)&idt_entries;
    
    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
    
    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10 , 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11 , 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12 , 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13 , 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14 , 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15 , 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16 , 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17 , 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18 , 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19 , 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20 , 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21 , 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22 , 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23 , 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24 , 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25 , 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26 , 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27 , 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28 , 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29 , 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30 , 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31 , 0x08, 0x8E);
    
    idt_flush((uint32_t)&idt_ptr);
    terminal_writestring("DONE\n");
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    
    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(uint32_t eax, registers_t regs)
{
    /*
        u32int ds;                  // Data segment selector
        u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
        u32int int_no, err_code;    // Interrupt number and error code (if applicable)
        u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
   */
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
}
void isr_handler2(uint32_t arg1,uint32_t arg2,uint32_t arg3,uint32_t arg4,uint32_t arg5)
{
    terminal_writedec(arg1);
    terminal_writestring("\n");
    terminal_writedec(arg2);
    terminal_writestring("\n");
    terminal_writedec(arg3);
    terminal_writestring("\n");
    terminal_writedec(arg4);
    terminal_writestring("\n");
    terminal_writedec(arg5);
    terminal_writestring("\n");
}
void write_register(uint32_t regval){
    terminal_writedec(regval);
    terminal_writestring("\n");
}