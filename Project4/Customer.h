#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include "Timing.h"

// CONSTANTS 
#define TIMING_SCALE (1500)  // this is used to scale timing for the usleeps

// STRUCTS
typedef struct{
	double queue_enter_time; 
	double queue_exit_time;
	int transaction_time;
} Customer;

// PROTOTYPES
void *add_customer(void *arg);
int average_customer_transaction_time(Customer *customers, int total);
double average_queue_time(Customer *customers, int total);
void *customer_thread(void *arg);
int max_customer_queue_time(Customer *customers, int total);
int max_transaction_time(Customer *customers, int total);
double max_queue_time(Customer *customers, int total);

#endif /* CUSTOMER_H_ */
