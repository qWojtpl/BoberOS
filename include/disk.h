#ifndef DISK
#define DISK

void write_sector(unsigned int lba, unsigned short* buffer);
void read_sector(unsigned int lba, unsigned short* buffer);

#endif