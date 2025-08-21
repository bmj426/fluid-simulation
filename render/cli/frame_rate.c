#define _POSIX_C_SOURCE 200112L
#include "frame_rate.h"
#include <errno.h>

static inline long sec_to_ns(double s) { return (long)(s * 1e9); }
static inline double ts_diff_sec(struct timespec a, struct timespec b) {
	return (double)(a.tv_sec - b.tv_sec) + (double)(a.tv_nsec - b.tv_nsec)/1e9;
}

static inline void ts_add_ns(struct timespec* t, long ns) {
	t->tv_nsec += ns;
	while (t->tv_nsec >= 1000000000L) { t->tv_nsec -= 1000000000L; t->tv_sec++; }
}

static inline void get_now(struct timespec* t) {
	clock_gettime(CLOCK_MONOTONIC, t);
}

static double mode_to_fps(FrMode m) {
	switch (m) {
		case FR_FIXED_60: return 60.0;
		case FR_FIXED_120: return 120.0;
		case FR_INF: return 0.0; // 무제한
		default: return 60.0;
	}
	return 60.0;
}


void fr_init(FrameRate* fr, FrMode mode) {
	fr->mode = mode;
	fr->target_fps = mode_to_fps(mode);
	fr->deadline_valid = 0;
	fr->last_valid = 0;
	fr->ema_dt_sec = 0.0;
	fr->ema_alpha = 0.1;
}

void fr_set_mode(FrameRate* fr, FrMode mode) {
	fr->mode = mode;
	fr->target_fps = mode_to_fps(mode);
	fr->deadline_valid = 0;
}

double fr_begin(FrameRate* fr) {
	struct timespec now;
	get_now(&now);

	double dt = 0.0;
	if (fr->last_valid) {
		dt = ts_diff_sec(now, fr->last_time);

		if (fr->ema_dt_sec <= 0.0) fr->ema_dt_sec = dt;
		else fr->ema_dt_sec = fr->ema_alpha*dt + (1.0 - fr->ema_alpha)*fr->ema_dt_sec;
	}

	fr->last_time = now;
	fr->last_valid = 1;

	if (!fr->deadline_valid) {
		fr->next_deadline = now;
		fr->deadline_valid = 1;
	}

	return dt;
}

void fr_end(FrameRate* fr) {
	if (fr->target_fps <= 0.0) return;

	const long interval_ns = sec_to_ns(1.0 / fr->target_fps);
	ts_add_ns(&fr->next_deadline, interval_ns);

	int rc;
	do {
		rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &fr->next_deadline, NULL);
	} while (rc == EINTR);

	struct timespec now;
	get_now(&now);
	if (ts_diff_sec(fr->next_deadline, now) < -0.1) {
		fr->next_deadline = now;
	}
}

double fr_fps(const FrameRate* fr) {
	if (fr->ema_dt_sec <= 0.0) return 0.0;
	return 1.0 / fr->ema_dt_sec;
}

const char* fr_mode_name(const FrameRate* fr) {
	switch (fr->mode) {
		case FR_FIXED_60: return "fixed-60";
		case FR_FIXED_120: return "fixed-120";
		case FR_INF: return "infinite";
		default: return "unknown";
	}
	return "unknown";
}
