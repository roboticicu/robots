#include <stdio.h>
#include <string.h>


void set_line(char *display, char *mess, int line) {
	char *dest = display + (20*(line-1));
	memcpy(dest, mess, 20);
}

void print_display(char *display) {
	int x;
	printf("Display:");
	for (x = 0; x < 80; x++) {
		if ( x % 20 == 0)
			printf("\n");
		printf("%c", display[x]);
	}
	printf("\n");
	return;
}

void main() { 
	char display_screen[80];

	memset(display_screen, '1', 80);
	set_line(display_screen, "This is a test.", 1);
	set_line(display_screen, "Second test is this.", 3);
	set_line(display_screen, "0123456789", 4);
	print_display(display_screen);
}