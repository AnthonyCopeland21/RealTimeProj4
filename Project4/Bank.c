#include "Bank.h"

void bank_is_open(){
	// This function call below controls the number of real-time seconds the bank is open
	open_bank_timer(42);
	while (get_bank_open() && get_overtime()){
		usleep(10);
	}
	printf("Bank is closed!\n");
}

void prep_for_open(){
	printf("Welcome to the Bank!\n");

	int ret = 0;

	pthread_t queue_id;

	// teller threads created
	create_teller_threads();

	// customer queue created
	ret = pthread_create(&queue_id, NULL, &customer_thread, (void *)NULL);

	printf("Bank is currently closed %d\n", get_bank_open());
	// set timer, open the bank
	bank_is_open();

	pthread_exit(NULL);
}
