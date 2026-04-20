

#include "print.h"
#include "keyboard.h"
#include "disk.h"

void kernel_main() {

	print(0, 'B');
	print(1, 'O');
	print(2, 'B');
	print(3, 'E');
	print(4, 'R');

	int index = 4;

	short data[256];

	read_sector(50, data);

	for(int i = 0; i < 256; i++) {
		print(++index, data[i]);
        data[i] = 0x4142;
    }

	write_sector(50, data);

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