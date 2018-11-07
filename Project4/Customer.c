#include "Customer.h"

/* Purpose: Thread function for the customer queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *customer_thread(void *arg){
	// initialize customer arguments
	Customer *queue;
	int count = 0;
	// int last_customer_count = 0;
	queue = malloc(sizeof(Customer));
	queue[count].queue_enter_time = 0;  // initialized to first count
	queue[count].queue_exit_time = 0;
	queue[count].transaction_time = rand() % 450 + 30;
	count++;
	printf("Creation of customer queue\n");
	// loop for while the queue is not empty and bank is open
	// note: the bank will always be open before customer_thread is started

	// for each new customer, realloc(queue, sizeof(Customer))

	free(queue);
	pthread_exit(NULL);
}
