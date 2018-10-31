#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// STRUCTS
typedef struct {
	int queue_time;
	int transaction_time;
} Customer;

typedef struct {
	int available = 1;
	int break_number = 0;
	int break_time;
	pthread_t id;
} Teller;

// PROTOTYPES
int available_teller(Teller *tellers);
int max_customer_queue_time(Customer *customers);
int max_wait_teller_time(Customer *customers);
int max_transaction_time(Customer *customers);
void *teller_thread();

int main(int argc, char *argv[]) {
	printf("Welcome to the Bank!\n");
	int ret = 0;
	Teller tellers[3];
	// infinite loop for bank
	while (1) {
		for(int i = 0; i < 3; i++) {
			ret = pthread_create(&tellers[i].id, NULL, &teller_thread);
			if (ret != 0){
				printf("ERROR: pthread_create() failed\n");
				exit(EXIT_FAILURE);
			}
		}




		pthread_exit(NULL);
	}
	return EXIT_SUCCESS;
}

/* Purpose: Provide next available teller
 * Inputs: tellers - array of all tellers
 * Output: 1->length of tellers for available teller
 * 		   0 for no available teller
 */
int available_teller(Teller *tellers) {
	for (int i = 0; i < len(tellers); i++){
		if (tellers[i].available) return (i + 1);
	}
	return 0;
}

/* Purpose: Thread function for the teller
 * Inputs:  None
 * Output:  None
 */

