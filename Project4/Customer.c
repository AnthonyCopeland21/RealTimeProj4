#include "Customer.h"
#include "Teller.h"

static int enter_customer = 0;

/* Purpose: Thread function to sleep until customer needs to be added to queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *add_customer(void *arg){
	// simulated 1 to 4 minutes of wait time for another customer to enter
	usleep((rand() % 180 + 60) * TIMING_SCALE);
	enter_customer = 1;
	pthread_exit(NULL);
}

/* Purpose: Thread function for the customer queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *customer_thread(void *arg){
	// initialize customer arguments
	Customer *queue;
	pthread_t add_cust_id;
	int i = 0;
	int j = 0;
	int customer_count = 0;
	int next_customer = 0;
	int ret = 0;
	struct timeval start, stop;
	// 420 is max number of customer that can be received in a day (60 per hour for 7 hours)
	queue = malloc(sizeof(Customer) * 420);
	memset(queue, 0, sizeof(Customer) * 420);
	// loop to wait for bank to open
	while(!(get_bank_open())){
		usleep(10);
	}
	// loop to wait for bank to close. Adds customers to queue
	ret = pthread_create(&add_cust_id, NULL, &add_customer, (void *)NULL);
	while(get_bank_open()){
		// customer has entered. add him to queue
		if(enter_customer){
			gettimeofday(&start, NULL);
			queue[customer_count].queue_enter_time = start.tv_usec * 1000000 + start.tv_sec;
			queue[customer_count].transaction_time = (rand() % 450 + 30) * TIMING_SCALE;
			printf("Set transaction time to %d for Customer %d\n", queue[customer_count].transaction_time, customer_count);
			customer_count++;

			enter_customer = 0;
			ret = pthread_create(&add_cust_id, NULL, &add_customer, (void *)NULL);
		}
		j = available_teller();
		if (j != -1 && next_customer < customer_count) {
			printf("Teller %d taking customer %d\n", j, next_customer);
			gettimeofday(&stop, NULL);
			queue[customer_count].queue_exit_time = stop.tv_usec * 1000000 + start.tv_sec;
			set_transaction_time(queue[next_customer].transaction_time, j);
			set_available(0, j);
			next_customer++;
		}
	}
	// wait for all customers to be taken care of
	//printf("Customers that came in: %d\nCustomers left: %d\n",customer_count,(customer_count-next_customer));


	// Statistics should go down here
	printf("Calculating statistics for the day...\n\n");
	printf("Total Customers: %d\n", customer_count);
	int max_transaction = max_transaction_time(queue, customer_count);
	printf("Max transaction time: %d minutes %d seconds\n", max_transaction/60, max_transaction%60);
	int max_queue = max_queue_time(queue, customer_count);
	printf("Max queue time: %d minutes %d seconds\n",max_queue/60, max_queue%60);
	for (i = 0; i < 3; i++){
		printf("Teller %d: %d\n", i, get_teller_customer_count(i));
	}
	free(queue);
	pthread_exit(NULL);
}

/* Purpose: Calculate the max transaction time
 * Inputs:  Customer * customers, all customers that have come through
 * 			total, number of customers
 * Output:  None
 */
int max_transaction_time(Customer *customers, int total) {
	int max = 0;
	int i = 0;
	for(i = 0; i < total; i++) {
		if (customers[i].transaction_time > max) {
			max = customers[i].transaction_time;
		}
	}
	return max/TIMING_SCALE;
}

/* Purpose: Calculate the max queue time
 * Inputs:  Customer * customers, all customers that have come through
 * 			total, number of customers
 * Output:  None
 */
int max_queue_time(Customer *customers, int total) {
	int max = 0;
	int i = 0;
	for(i = 0; i < total; i++) {
		if ((customers[i].queue_exit_time - customers[i].queue_enter_time)> max) {
			max = customers[i].queue_exit_time - customers[i].queue_enter_time;
		}
	}
	return max/TIMING_SCALE;
}
