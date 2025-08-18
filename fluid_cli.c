#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>

void enable_raw_mode(void* orig) {
	struct termios* raw;
	raw = (struct termios *)orig;
	raw->c_lflag &= ~(ICANON | ECHO);
	raw->c_cc[VMIN]	= 0;
	raw->c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, raw);
}

void disable_raw_mode(void* orig) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, (struct termios *)orig);
}
	

int main(void) {
	struct termios orig;
	tcgetattr(STDIN_FILENO, &orig);
	enable_raw_mode((void*) &orig);

	printf("\x1b[?25l\x1b[2J");

	const int W = 80;
	const int H = 24;
	float x = W/2.0f, y = H/2.0f;
	float vx = 20.0f, vy = -12.5f;
	const float frame_rate = 120.0f;
	const float dt = 1.0f/frame_rate;
	const float damping = 0.98f;

	while(1) {
		x += vx * dt;
		y += vy * dt;

		vy += 20.0f * dt;

		if (x < 2) 	{ x = 2; 	vx = -vx * damping; }
		if (x > W-1){ x = W-1;	vx = -vx * damping; }
		if (y < 2) 	{ y = 2; 	vy = -vy * damping; }
		if (y > H-1){ y = H-1; 	vy = -vy * damping; }

		printf("\x1b[H");
		printf("Particle Sim - (Ctrl+C : force quit)\n");

		for (int j = 1; j <=H; ++j) {
			for (int i = 1; i <= W; ++i) {
				if (j==1 || j==H || i ==1 || i==W) putchar('#');
				else if (i == (int)(x+0.5f) && j == (int)(y+0.5f)) putchar('o');
				else putchar(' ');
			}
			putchar('\n');
		}

		fflush(stdout);

		usleep((int)(dt * 1e6));
	}

	disable_raw_mode((void *) &orig);

	printf("\x1b[?25h");
	return 0;
}
