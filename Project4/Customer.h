#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Timing.h"

#define TIMING_SCALE (1400)

typedef struct{
	int queue_enter_time;
	int queue_exit_time;
	int transaction_time;
} Customer;

// PROTOTYPES
void *add_customer(void *arg);
void *customer_thread(void *arg);
int max_customer_queue_time(Customer *customers, int total);
int max_transaction_time(Customer *customers, int total);
int max_queue_time(Customer *customers, int total);

#endif /* CUSTOMER_H_ */
