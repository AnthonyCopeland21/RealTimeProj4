#include "Bank.h"

/* Purpose: Opens the bank and prints when the bank closes
 * Inputs:  None
 * Outputs: None
 */
void bank_is_open(void){
	// This function call below controls the number of real-time seconds the bank is open
	open_bank_timer(42);
	// loop that spins until the bank closes and all customers are accounted for (overtime)
	while (get_bank_open() && get_overtime()){
		usleep(10);
	}
	printf("Bank is closed!\n");
}

/* Purpose: Preparation for bank to open. Creates customer queue and bank teller threads
 * Inputs:  None
 * Outputs: None
 */
void prep_for_open(void){
	printf("Welcome to the Bank!\n");
	int ret = 0;
	pthread_t queue_id;

	// this function call creates the teller threads
	create_teller_threads();

	// customer queue thread created with this call
	ret = pthread_create(&queue_id, NULL, &customer_thread, (void *)NULL);
	printf("Bank is currently closed %d\n", get_bank_open());
	
	// this function call opens the bank and initializes the timer
	bank_is_open();

	pthread_exit(NULL);
}
