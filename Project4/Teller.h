#ifndef TELLER_H_
#define TELLER_H_

#include "Timing.h"

// STRUCTS
typedef struct {
	int available;
	int break_number;
	int break_time;
	pthread_t id;
} Teller;

// PROTOTYPES
int available_teller(Teller *tellers);
void create_teller_threads();
void *teller_thread(void *arg);


#endif /* TELLER_H_ */
