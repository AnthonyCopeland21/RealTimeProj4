#include "Customer.h"

/* Purpose: Thread function for the customer queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *customer_thread(void *arg){
	// initialize customer arguments
	Customer *queue;
	int count = 0;
	queue = malloc(sizeof(Customer));
	queue[count].queue_enter_time = 0;
	queue[count].queue_exit_time = 0;
	queue[count].transaction_time = rand() % 450 + 30;
	count++;
	printf("Creation of customer queue: %d\n", get_bank_open());
	// loop to wait for bank to open
	while(!(get_bank_open())){
		usleep(10);
	}
	int counter = 1;
	int j = 0;
	// loop to wait for bank to close. Adds customers to queue
	while(get_bank_open()){
		// access current time.
		// wait for time to elapse over randomized time
		// put the wait in a loop, so that it can constantly check
		// for available tellers
		j = available_teller();
		if (j != -1) teller[j].transaction_time = queue[count].transaction_time;


		// waiting for customer to arrive
		// you cant sleep because if there's a line in the queue and 
		// the teller finishes with acustomer, the teller needs to take
		// the next customer in line, and cannot do that when sleeping
		usleep((rand() % 180 + 60) * 100);
		printf("In Customer thread: %d\n", counter);
	}
	// for each new customer, realloc(queue, sizeof(Customer))

	// Statistics should go down here
	printf("Calculating statistics for the day...\n");

	free(queue);
	pthread_exit(NULL);
}
