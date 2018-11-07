#include "Bank.h"

void bank_is_open(){
	open_bank_timer(25200);
}

void prep_for_open(){
	printf("Welcome to the Bank!\n");
	int ret = 0;

	pthread_t queue_id;

	// teller threads created
	create_teller_threads();

	// customer queue created
	ret = pthread_create(&queue_id, NULL, &customer_thread, (void *)NULL);

	// set timer, open the bank
	bank_is_open();


	pthread_exit(NULL);

}
