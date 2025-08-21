#pragma once
#define _POSIX_C_SOURCE 200112L
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	FR_FIXED_60,
	FR_FIXED_120,
	FR_INF,
} FrMode;


typedef struct {
	FrMode 	mode;
	double 	target_fps;
	struct 	timespec next_deadline;
	int		deadline_valid;

	struct	timespec last_time;
	int		last_valid;

	double 	ema_dt_sec;
	double	ema_alpha;
} FrameRate;


void 	fr_init(FrameRate* fr, FrMode mode);
void 	fr_set_mode(FrameRate* fr, FrMode mode);
double 	fr_begin(FrameRate* fr);
void 	fr_end(FrameRate* fr);
double	fr_fps(const FrameRate* fr);
const char* fr_mode_name(const FrameRate* fr);

#ifdef __cplusplus
}
#endif
