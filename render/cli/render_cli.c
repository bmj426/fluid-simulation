#include <stdio.h>
#include <string.h>
#include "ansi.h"

void cli_draw_border(int W, int H) {
	for (int j = 1; j <= H; ++j) {
		for (int i = 1; i <= W; ++i) {
			if (j == 1 || j == H || i == 1 || i == W) putchar('#');
			else putchar(' ');
		}
		putchar('\n');
	}
	fflush(stdout);
}
