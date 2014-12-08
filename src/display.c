#include "display.h"
#include "string.h"
#include "io.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color) {
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

size_t cursor_y;
size_t cursor_x;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void terminal_initialize() {
    cursor_y = 0;
    cursor_x = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    terminal_clear();
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void move_cursor(void) {
    unsigned temp = cursor_y * VGA_WIDTH + cursor_x;

    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void terminal_scroll() {
    if (cursor_y < 24) {
        return;
    }
    // Move all characters up one row
    size_t index;
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            index = y * VGA_WIDTH + x;
            terminal_buffer[index - VGA_WIDTH] = terminal_buffer[index];
        }
    }
    //Clear bottom row
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_WIDTH * VGA_HEIGHT) - (x + 1)] = make_vgaentry(' ', terminal_color);
    }
    cursor_y--;
}

void terminal_putchar(char c) {
    if (c == 0x08 && cursor_x > 0) {
        cursor_x--;
    } else if (c == 0x09) {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c >= ' ') {
        terminal_putentryat(c, terminal_color, cursor_x, cursor_y);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    terminal_scroll();
    move_cursor();
}

void terminal_writestring(const char* data) {
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++) {
        terminal_putchar(data[i]);
    }
}
