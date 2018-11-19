#include "Customer.h"
#include "Teller.h"

// GLOBAL VARIABLES
static int enter_customer = 0;  // status variable for when a customer is entering the bank

/* Purpose: Thread function to sleep until customer needs to be added to queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *add_customer(void *arg){
	// sleeps for 1 to 4 minutes, then sets flag high to allow customer_thread to accept a new customer
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
	Customer *queue;  // Customer struct used to create queue of customers
	pthread_t add_cust_id;
	int i = 0;
	int j = 0;
	int customer_count = 0;
	int max_queue_depth = 0;
	int next_customer = 0;
	int ret = 0;
	struct timeval start, stop;  // timeval structs used for determining queue wait times

	// 420 is max number of customer that can be received in a day (60 per hour for 7 hours)
	queue = malloc(sizeof(Customer) * 420);
	memset(queue, 0, sizeof(Customer) * 420);

	// loop to wait for bank to open
	while(!(get_bank_open())){
		usleep(10);
	}

	// Adds customers to queue
	ret = pthread_create(&add_cust_id, NULL, &add_customer, (void *)NULL);
	// loop to wait for bank to close, customers are added during this time
	while(get_bank_open()){
		// flag that is set high when a customer arrives at the bank (1 to 4 minutes)
		if(enter_customer){
			gettimeofday(&start, NULL); 
			// converts struct timeval to a double value
			queue[customer_count].queue_enter_time = start.tv_usec * 0.000001 + start.tv_sec;
			queue[customer_count].transaction_time = (rand() % 450 + 30) * TIMING_SCALE;
			customer_count++;
			if((customer_count-next_customer) > max_queue_depth) max_queue_depth = customer_count - next_customer;
			enter_customer = 0;  // flag set low until a new customer arrives
			// new customer has already entered, start waiting for the next customer to enter
			ret = pthread_create(&add_cust_id, NULL, &add_customer, (void *)NULL);
		}
		// functional call will inform of the next available teller, or -1 is none is available
		j = available_teller();
		// conditional for next available teller and next customer to not be attended to
		if (j != -1 && next_customer < customer_count) {
			gettimeofday(&stop, NULL);
			// converts struct timeval to a double value
			queue[next_customer].queue_exit_time = stop.tv_usec * 0.000001 + stop.tv_sec;
			// sets the Teller struct in the Teller files to the transaction time to the teller thread is able to sleep
			set_transaction_time(queue[next_customer].transaction_time, j);
			// this is set to 0, which in the Teller thread means the Teller needs to assist a customer
			set_available(0, j);
			next_customer++;
		}
	}
	// conditional for when there are more customers left over after the bank's regular hours are closed
	if (next_customer < customer_count) printf("Overtime required...\n");
	
	// loop for when there are customers that still need to be helped
	while (next_customer < customer_count) {
		j = available_teller();
		if (j != -1){
			gettimeofday(&stop, NULL);
			queue[next_customer].queue_exit_time = stop.tv_usec * 0.000001 + stop.tv_sec;
			set_transaction_time(queue[next_customer].transaction_time, j);
			set_available(0, j);
			next_customer++;
		}
	}

	// All customers are done, and the bank is officially closed
	set_overtime(0);

	printf("Calculating statistics for the day...\n\n");

	// total customer count for the day
	printf("Total Customers: %d\n", customer_count);

	// loop to print out customers per teller
	for (i = 0; i < 3; i++){
		printf("Teller %d: %d\n", i, get_teller_customer_count(i));
	}

	// max time for teller waiting for a customer. timeval struct used for timing
	int max_wait = max_wait_time() * 10000;
	printf("Max teller wait time: %d minutes %d seconds\n", max_wait/10000, (max_wait % 10000)/167);

	// max transaction time for customer at a teller. random int used for timing
	int max_transaction = max_transaction_time(queue, customer_count);
	printf("Max transaction time: %d minutes %d seconds\n", max_transaction/60, max_transaction%60);

	// max time a customer spends in queue. timeval struct used for timing
	int max_queue = max_queue_time(queue, customer_count) * 100000;
	printf("Max queue time: %d minutes %d seconds\n", max_queue/10000, (max_queue % 10000)/167);

	// max queue depth for the day
	printf("Max queue depth: %d\n", max_queue_depth);

	// average transaction time. random int used for timing
	int avg_transaction = average_customer_transaction_time(queue, customer_count);
	printf("Average transaction time: %d minutes and %d seconds\n", avg_transaction/60, avg_transaction%60);

	// average time customer is in queue. timeval struct used for timing
	unsigned int avg_queue = average_queue_time(queue, customer_count) * 100000;
	printf("Average queue time: %d minutes %d seconds\n", avg_queue/10000, (avg_queue % 10000)/167);

	// average time teller waits for customer. timeval struct used for timing
	unsigned int avg_wait = average_wait_time() * 100000;
	printf("Average teller wait time: %d minutes %d seconds\n", avg_wait/10000, (avg_wait % 10000)/167);
	printf("\n");

	if (get_activate_breaks()){
		// loop to go through each teller and print number of breaks for specified teller
		for (i = 0; i < 3; i++){
			printf("Total number of breaks for Teller %d: %d\n", i+1, get_teller_break_count(i));
		}
		printf("\n");

		// loop to go through each teller and print average time for specified teller. random int used for timing
		unsigned int avg_break = 0;
		for (i = 0; i < 3; i++){
			avg_break = average_break_time(i);
			printf("Average Teller %d break time: %d minutes %d seconds\n", i+1, avg_break/60, avg_break % 60);
		}
		printf("\n");

		// loop to go through each teller and print max break time for specified teller. random int used for timing
		int max_break = 0;
		for (i = 0; i < 3; i++){
			max_break = max_break_time(i);
			printf("Max Teller %d break time: %d minutes %d seconds\n", i+1, max_break/60, max_break % 60);
		}
		printf("\n");

		// loop to go through each teller and print min break time for specified teller. random int used for timing
		int min_break = 0;
		for (i = 0; i < 3; i++){
			min_break = min_break_time(i);
			printf("Min Teller %d break time: %d minutes %d seconds\n", i+1, min_break/60, min_break%60);
		}
	}

	// free the memory of queue
	free(queue);
	pthread_exit(NULL);
}

/* Purpose: Calculate the average queue time
 * Inputs:  Customer * customers, all customers that have come through
 * 			total, number of customers
 * Output:  None
 */
int average_customer_transaction_time(Customer *customers, int total) {
	int sum_time = 0;
	int i = 0;
	// loop for adding up each transaction time to determine average
	for (i = 0; i < total; i++){
		sum_time += customers[i].transaction_time;
	}
	return (sum_time/TIMING_SCALE)/total;
}

/* Purpose: Calculate the average queue time
 * Inputs:  Customer * customers, all customers that have come through
 * 			total, number of customers
 * Output:  None
 */
double average_queue_time(Customer *customers, int total) {
	double average = 0;
	int i = 0;
	// loop for adding up each queue time for customers to determine average
	for(i = 0; i < total; i++) {
		average += (customers[i].queue_exit_time - customers[i].queue_enter_time)/total;
	}
	return average;
}

/* Purpose: Calculate the max transaction time
 * Inputs:  Customer * customers, all customers that have come through
 * 			total, number of customers
 * Output:  None
 */
int max_transaction_time(Customer *customers, int total) {
	int max = 0;
	int i = 0;
	// loop to go through each transaction time and determine max
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
double max_queue_time(Customer *customers, int total) {
	double max = 0;
	int i = 0;
	// loop to go through each customer queue time and determine max
	for(i = 0; i < total; i++) {
		if ((customers[i].queue_exit_time - customers[i].queue_enter_time) > max) {
			max = customers[i].queue_exit_time - customers[i].queue_enter_time;
		}
	}
	return max;
}


