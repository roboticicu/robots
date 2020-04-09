#include <stdio.h>
#include <string.h>

#define MAX_LINE 20
#define NUM_LINES 4
#define DISPLAY_SIZE MAX_LINE*NUM_LINES

int max_length(char *mess, int max_len) {
	int l = strlen(mess);

	if (l > max_len)
		return max_len;
	return l;
}

void set_line_memcpy(char *display, char *mess, int line) {
	char *dest = display + (MAX_LINE*(line-1));
	memcpy(dest, mess, MAX_LINE);
}

void set_line_strncpy(char *display, char *mess, int line) {
	char *dest = display + (MAX_LINE*(line-1));

	strncpy(dest, mess, max_length(mess, MAX_LINE));
}

void print_display(char *display) {
	int x;
	printf("Display:");
	for (x = 0; x < DISPLAY_SIZE; x++) {
		if ( x % MAX_LINE == 0)
			printf("\n");
		printf("%c", display[x]);
	}
	printf("\n");
	return;
}

void main() { 
	char display_screen[80];

	memset(display_screen, '1', MAX_LINE*NUM_LINES);
	set_line_memcpy(display_screen, "This is a test.", 1);
	set_line_memcpy(display_screen, "Second test is this.", 3);
	set_line_memcpy(display_screen, "01234567890123456789", 4);
	print_display(display_screen);
	memset(display_screen, ' ', MAX_LINE*NUM_LINES);
	set_line_strncpy(display_screen, "This is a test.", 1);
	set_line_strncpy(display_screen, "Second test is this.", 3);
	set_line_strncpy(display_screen, "01234567890123456789", 4);
	print_display(display_screen);
}