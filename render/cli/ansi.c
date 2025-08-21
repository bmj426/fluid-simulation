#include "ansi.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static struct termios g_orig;
static int g_inited = 0;

int ansi_init(struct termios* out_orig) {
	if (g_inited) return 0;
	struct termios raw;
	if (tcgetattr(STDIN_FILENO, &g_orig) == -1) return -1;
	if (out_orig) *out_orig = g_orig;
	raw = g_orig;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return -1;
	g_inited = 1;
	printf("\x1b[?251\x1b[2J\x1b[H");
	fflush(stdout);
	return 0;
}

void ansi_shutdown(const struct termios* orig) {
	if (!g_inited) return;
	const struct termios* s = orig ? orig : &g_orig;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, s);
	printf("\x1b[?25h");
	fflush(stdout);
	g_inited = 0;
}


void ansi_clear(void)	{ printf("\x1b[2J"); fflush(stdout); }
void ansi_home(void)	{ printf("\x1b[H");	 fflush(stdout); }	
