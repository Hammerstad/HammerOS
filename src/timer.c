#include "display.h"
#include "io.h"
#include "interrupts/isr.h"
#include "timer.h"

uint32_t tick = 0;

static void timer_callback(__attribute__((unused)) registers_t regs)
{
    tick++;
    terminal_writestring("Tick: ");
    terminal_writedec(tick);
    terminal_writestring("\n");
}

void init_timer(uint32_t frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);
    
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;
    
    // Send the command byte.
    outportb(0x43, 0x36);
    
    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)( divisor & 0xFF );
    uint8_t h = (uint8_t)( (divisor>>8));
    
    // Send the frequency divisor.
    outportb(0x40, l);
    outportb(0x40, h);
}
