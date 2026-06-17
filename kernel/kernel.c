

#include "print.h"
#include "keyboard.h"
#include "disk.h"
#include "fs.h"
#include <stdbool.h>

void kernel_main() {

	print(0, 'B');
	print(1, 'O');
	print(2, 'B');
	print(3, 'E');
	print(4, 'R');

	int index = 4;
	
	char free = get_free_record();
	print(++index, free);
	
	fcreate("test.txt");

	char data[512];

	read_record(0, data);

	for(int i = 0; i < 512; i++) {
		print(++index, data[i]);
	}


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