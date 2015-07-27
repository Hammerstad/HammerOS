#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include "display.h"
#include "gdt.h"
#include "interrupts/idt.h"
#include "keyboard.h"
#include "timer.h"

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main()
{
	terminal_initialize();
	terminal_putlogo();
	terminal_writestring("\n");
	
	init_gdt();
	init_idt();
	init_keyboard();
	
	// Allow interrupts
	asm volatile ("sti");
	
	//init_timer(50);
}
