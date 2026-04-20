

#include "print.h"
#include "keyboard.h"
#include "disk.h"

void kernel_main() {

	print(0, 'B');
	print(1, 'O');
	print(2, 'B');
	print(3, 'E');
	print(4, 'R');

	short data[256];

	for(int i = 0; i < 256; i++) {
        data[i] = 0x4241;
    }

	write_sector(50, data);

	int index = 4;

	while(1) {
		char c = keyboard_read_ascii();
		if(c == 0) {
			continue;
		}
		print(++index, c);
	}

}

__attribute__((section(".text._start")))
void _start() {
	kernel_main();
}