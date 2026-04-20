#include "print.h"
#define VGA_MEMORY ((volatile char*)0xB8000)

void print(int position, char character) {

    volatile char* vga = VGA_MEMORY;

    vga[position] = character;

}

