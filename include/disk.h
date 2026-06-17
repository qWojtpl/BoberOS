#ifndef DISK
#define DISK

void write_sector(unsigned int lba, unsigned short* buffer);
void read_sector(unsigned int lba, unsigned short* buffer);
void buffer_to_string(unsigned short* buffer, char* output);
void string_to_buffer(char* input, unsigned short* output);

#endif