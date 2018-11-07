#include "Teller.h"

// GLOBALS
static Teller tellers[3];

/* Purpose: Provide next available teller
 * Inputs: tellers - array of all tellers
 * Output: 1->length of tellers for available teller
 * 		   0 for no available teller
 */
int available_teller(Teller *tellers) {
	int i = 0;
	// loop runs through tellers that
	for (i = 0; i < (sizeof(tellers)/sizeof(tellers[0])); i++){
		if (tellers[i].available) return (i + 1);
	}
	return 0;
}

/* Purpose: Creates threads for tellers
 * Inputs:  None
 * Outputs: None
 */
void create_teller_threads(void){
	int ret = 0;
	int i;
	// loop to create 3 teller threads
	for(i = 0; i < 3; i++) {
		ret = pthread_create(&tellers[i].id, NULL, &teller_thread, (void *)i);
		if (ret != 0){
			printf("ERROR: pthread_create() failed\n");
			exit(EXIT_FAILURE);
		}
	}
}

/* Purpose: Thread function for the teller
 * Inputs:  None
 * Output:  None
 */
void *teller_thread(void *arg){
	int i = (int)arg;
	// initialize teller arguments
	tellers[i].available = 0;
	tellers[i].break_number = 0;
	tellers[i].break_time = 0;
	tellers[i].customer_transaction_time = 0;
	tellers[i].total_customer_count = 0;

	printf("Creation of teller %d\n", i+1);
	printf("Bank open: %d\n", get_bank_open());
	// loop to wait for bank to open
	while(!get_bank_open()){
		usleep(10);
	}
	// loop to wait for bank to close and customer queue to be 0
	while(get_bank_open()){
		if(!teller[i].available){
		}
	}

	pthread_exit(NULL);
}


