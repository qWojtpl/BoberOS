

#include "print.h"


void kernel_main() {

	print(0, 'B');
	print(2, 'O');
	print(4, 'B');
	print(6, 'E');
	print(8, 'R');

	while(1) { }

}

__attribute__((section(".text._start")))
void _start() {
	kernel_main();
}
