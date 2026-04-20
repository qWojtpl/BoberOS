#ifndef COMMS
#define COMMS

unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void outb(unsigned short port, unsigned char value);
void outw(unsigned short port, unsigned short value);

#endif