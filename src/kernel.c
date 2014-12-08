#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "display.h"
#include "string.h"

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
	terminal_writestring("AHello, kernel World!\n");
	terminal_setcolor(1);
	terminal_writestring("BHello, kernel World!\n");
	terminal_setcolor(2);
	terminal_writestring("CHello, kernel World!\n");
	terminal_setcolor(3);
	terminal_writestring("DHello, kernel World!\n");
	terminal_setcolor(4);
	terminal_writestring("EHello, kernel World!\n");
	terminal_setcolor(5);
	terminal_writestring("FHello, kernel World!\n");
	terminal_setcolor(6);
	terminal_writestring("GHello, kernel World!\n");
	terminal_setcolor(7);
	terminal_writestring("HHello, kernel World!\n");
	terminal_setcolor(8);
	terminal_writestring("IHello, kernel World!\n");
	terminal_setcolor(9);
	terminal_writestring("JHello, kernel World!\n");
	terminal_setcolor(10);
	terminal_writestring("KHello, kernel World!\n");
	terminal_setcolor(11);
	terminal_writestring("LHello, kernel World!\n");
	terminal_setcolor(12);


	terminal_writestring("HHello, kernel World!\n");
	terminal_writestring("IHello, kernel World!\n");
	terminal_writestring("JHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_writestring("JAJAHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_setcolor(5);
	terminal_writestring("A LINE!\n");
	terminal_writestring("TEST!\n");
	terminal_writestring("LOREM!\n");
	terminal_writestring("IPSUM!\n");
	terminal_writestring("BSDHello, kernel World!\n");
	terminal_writestring("KHello, kernel World!\n");
	terminal_setcolor(10);
	terminal_writestring("WEEE!\n");
	terminal_writestring("JAAA!\n");
}
