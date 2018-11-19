#ifndef TELLER_H_
#define TELLER_H_

#include "Timing.h"

#define TIMING_SCALE (1400)

// STRUCTS
typedef struct {
	int available;
	int next_to_break;
	int break_number;		// number of breaks taken
	int break_time;		// how long the break will be
	double break_count_start;
	double break_count_current;
	double break_count_stop;
	double time_for_break;
	int break_waits[50];
	int customer_transaction_time;
	double all_waits[420];
	double start_wait_time;
	double end_wait_time;
	int total_customer_count;
	pthread_t id;
} Teller;

// PROTOTYPES
int available_teller(void);
void create_teller_threads();
int get_teller_customer_count(int teller_num);
int get_teller_break_count(int teller_num);
int get_activate_breaks(void);
double max_wait_time();
double max_break_time(int teller_num);
double min_break_time(int teller_num);
double average_wait_time();
double average_break_time(int teller_num);
void set_transaction_time(int trans_time, int teller);
void set_available(int available, int teller);
void *teller_thread(void *arg);
int next_teller_break(void);


#endif /* TELLER_H_ */
