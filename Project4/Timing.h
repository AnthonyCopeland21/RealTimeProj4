#ifndef TIMING_H_
#define TIMING_H_

#include <malloc.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// GLOBALS
static int bank_open = 0;
static int overtime = 1;

// PROTOTYPES
void open_bank_timer(int seconds_open);
void *close_bank(void *arg);
int get_bank_open(void);
int get_overtime(void);
void set_overtime(int ot);

#endif /* TIMING_H_*/
