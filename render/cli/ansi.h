#pragma once
#include <termios.h>

int ansi_init(struct termios* out_orig);
void ansi_shutdown(const struct termios* orig);
void ansi_clear(void);
void ansi_home(void);
