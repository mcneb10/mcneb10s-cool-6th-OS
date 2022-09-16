#include "utils.h"


void hang() {
	tty_print("The operating system will now hang.\n");
	__asm__("hang_loop:jmp hang_loop");
}
