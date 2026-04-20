

#include "print.h"
#include "keyboard.h"


void kernel_main() {

	print(0, 'B');
	print(2, 'O');
	print(4, 'B');
	print(6, 'E');
	print(8, 'R');

	int index = 8;

	while(1) {
		char c = keyboard_read_ascii();
		index += 2;
		print(index, c);
	}

}

__attribute__((section(".text._start")))
void _start() {
	kernel_main();
}
