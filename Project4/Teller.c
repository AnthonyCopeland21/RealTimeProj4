#include "Teller.h"

// GLOBALS
static Teller tellers[3];
static int activate_breaks = 0;

/* Purpose: Provide next available teller
 * Inputs: tellers - array of all tellers
 * Output: 1->length of tellers for available teller
 * 		   -1 for no available teller
 */
int available_teller(void) {
	int i = 0;
	// loop runs through tellers that
	for (i = 0; i < (sizeof(tellers)/sizeof(tellers[0])); i++){
		if (tellers[i].available) return i;
	}
	return -1;
}

/* Purpose: Creates threads for tellers
 * Inputs:  None
 * Outputs: None
 */
void create_teller_threads(){
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

/* Purpose: Set transaction time for available teller
 * Inputs:  trans_time - time for transaction
 * 			teller - 0->2 for available teller
 * Output:  None
 * */
void set_transaction_time(int trans_time, int teller){
	tellers[teller].customer_transaction_time = trans_time;
}

/* Purpose: Set availability for teller
 * Inputs:  available - teller availability
 * 			teller - 0->2 for available teller
 * Output:  None
 * */
void set_available(int available, int teller){
	tellers[teller].available = available;
}

/* Purpose: Thread function for the teller
 * Inputs:  None
 * Output:  None
 */
void *teller_thread(void *arg){
	int i = (int)arg;
	int start_wait = 0;
	struct timeval start, stop;
	// initialize teller arguments
	tellers[i].available = 0;
	tellers[i].break_number = 0;
	tellers[i].break_time = 0;
	tellers[i].customer_transaction_time = 0;
	tellers[i].total_customer_count = 0;
	tellers[i].end_wait_time = 0;
	tellers[i].start_wait_time = 0;

	printf("Creation of teller %d\n", i+1);
	// loop to wait for bank to open
	while(!get_bank_open()){
		usleep(10);
	}
	tellers[i].available = 1;
	// loop to wait for bank to close and customer queue to be 0
	while(get_bank_open()){
		if(activate_breaks){
			// this means that breaks are activated
			// I would use the struct timeval and the function gettimeofday(&VARIABLE, NULL) for both time until a break
			// and duration of breaks. See how I used them for the Customer queue_enter_time and queue_exit time
			// as well as the Teller start_wait_time and Teller stop_wait_time
			//
			// You could create a random wait time using rand() and wait until the current time is greater than
			// the struct timeval variable + random wait time, then you can set the availability of the teller to 0 and use
			// usleep to sleep a random duration/break
			//
			// The next thing would be creating functions to return the average, longest, and shortest break times for
			// each teller. You can call the functions from Customer.c in customer_thread() with all the other stats
		}
		if (start_wait == 0){
			gettimeofday(&start, NULL);
			tellers[i].start_wait_time = start.tv_usec * 0.000001 + start.tv_sec;
		}
		// this is only set to 0 in customer thread
		if(!tellers[i].available){
			gettimeofday(&stop, NULL);
			tellers[i].end_wait_time = stop.tv_usec * 0.000001 + stop.tv_sec;
			tellers[i].all_waits[tellers[i].total_customer_count] = tellers[i].end_wait_time - tellers[i].start_wait_time;
			tellers[i].total_customer_count += 1;
			usleep(tellers[i].customer_transaction_time);
			tellers[i].available = 1;
		}
	}

	pthread_exit(NULL);
}

int get_teller_customer_count(int teller_num){
	return tellers[teller_num].total_customer_count;
}

int get_activate_breaks(void){
	return (int)get_activate_breaks;
}

double max_wait_time(){
	double max = 0;
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; i++){
		for(j = 0; j < tellers[i].total_customer_count; j++){
			if (max < tellers[i].all_waits[i]) max = tellers[i].all_waits[j];
		}
	}
	return max;
}

double average_wait_time(){
	double average = 0;
	int i = 0;
	int j = 0;
	int total = tellers[0].total_customer_count + tellers[1].total_customer_count + tellers[2].total_customer_count;
	for (i = 0; i < 3; i++){
		for(j = 0; j < tellers[i].total_customer_count; j++){
			average += tellers[i].all_waits[j]/total;
		}
	}
	return average;
}

