#include "Timing.h"

static int bank_open = 0;

void open_bank_timer(int seconds_open){
	struct sigevent interupt;
	struct itimerspec timer;
	struct timespec end;
	struct timespec restart;

	timer_t timer_id;
	interupt.sigev_notify = SIGEV_SIGNAL_CODE;
	interupt.sigev_signo = 6;
	interupt.sigev_code = 666;
	interupt.sigev_priority = 5;

	end.tv_sec = (long)seconds_open;
	end.tv_nsec = (long)(seconds_open * 1000000000);
	restart.tv_nsec = 0;
	restart.tv_sec = 0;

	bank_open = 1;
	timer_create(CLOCK_REALTIME, &interupt, &timer_id);
	timer_settime(timer_id, 0, &end, &restart);
}





void teller_sleep(int seconds_sleep){
}
