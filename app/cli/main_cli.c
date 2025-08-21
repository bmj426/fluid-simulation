#include <stdio.h>
#include "ansi.h"
#include "render_cli.h"
#include "frame_rate.h"


int main(void) {
	struct termios orig;
	if (ansi_init(&orig) != 0) { perror("ansi_init"); return 1; }

	const int W = 80, H = 24;

	FrameRate fr;
	fr_init(&fr, FR_FIXED_120);

	while(1) {
		double dt = fr_begin(&fr);

		ansi_clear();
		ansi_home();
		printf("mode=%s fps=%.1f dt=%.3f sec\n", fr_mode_name(&fr), fr_fps(&fr), dt);
		cli_draw_border(W, H);

		fr_end(&fr);
	}

	ansi_shutdown(&orig);
	return 0;
}
