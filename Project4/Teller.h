#ifndef TELLER_H_
#define TELLER_H_

#include "Timing.h"

// STRUCTS
typedef struct {
	int available;
	int break_number;
	int break_time;
	int customer_transaction_time;
	int total_customer_count;
	pthread_t id;
} Teller;

// PROTOTYPES
int available_teller(void);
void create_teller_threads();
int get_teller_customer_count(int teller_num);
void set_transaction_time(int trans_time, int teller);
void set_available(int available, int teller);
void *teller_thread(void *arg);


#endif /* TELLER_H_ */
