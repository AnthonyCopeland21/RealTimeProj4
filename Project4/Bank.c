#include "Bank.h"

void bank_is_open(){
	open_bank_timer(25200);
	while (get_bank_open()){
		usleep(10);
		printf("Bank is open! \n");
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

	printf("Bank should now be closed\n");

	pthread_exit(NULL);
}
