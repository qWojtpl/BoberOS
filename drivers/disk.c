#include "disk.h"
#include "comms.h"

void write_sector(unsigned int lba, unsigned short* buffer) {
    outb(0x1F2, 1); // number of sectors
    outb(0x1F3, (unsigned char) lba);
    outb(0x1F4, (unsigned char) (lba >> 8));
    outb(0x1F5, (unsigned char) (lba >> 16));

    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F)); // write to master

    outb(0x1F7, 0x30); // write

    while((inb(0x1F7) & 0x80)); // wait - BUSY
    while(!(inb(0x1F7) & 0x08)); // DRQ - Data request must be available

    for(int i = 0; i < 256; i++) { // 256 x 16 bits
        outw(0x1F0, buffer[i]);
    }

    outb(0x1F7, 0xE7); // flush
}