#include "utils.h"


void hang() {
	sti();
	tty_print("The operating system will now hang.\n");
	__asm__("hangloop: jmp hangloop");
}
