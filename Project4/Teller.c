#include "Teller.h"

// GLOBALS
static Teller tellers[3];
static int activate_breaks = 0;
static int next_teller_to_break = 0;

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
	next_teller_to_break = next_teller_break();
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
	int j = 0;
	int start_wait = 1;
	int start_teller_wait = 1;
	struct timeval teller_start, start, stop, current;
	// initialize teller arguments
	tellers[i].available = 0;
	if (i == next_teller_to_break) tellers[i].next_to_break = 1;
	else tellers[i].next_to_break = 0;
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
	while(get_bank_open() || get_overtime()){
		if(activate_breaks){
			if (start_wait && tellers[i].next_to_break) {
				tellers[i].break_time = (rand() % 1800 + 1800);
				gettimeofday(&start, NULL);
				tellers[i].break_count_start = start.tv_usec * 0.000001 + start.tv_sec;
				start_wait = 0;
			}
			gettimeofday(&current, NULL);
			tellers[i].break_count_current = current.tv_usec * 0.000001 + current.tv_sec;
			tellers[i].time_for_break = tellers[i].break_count_current - tellers[i].break_count_start;

			if (tellers[i].next_to_break && tellers[i].available && (tellers[i].time_for_break * 1000) >= tellers[i].break_time){
				tellers[i].available = 0;
				tellers[i].break_waits[j] = (rand() % 180 + 60);
				usleep(tellers[i].break_waits[j] * TIMING_SCALE);
				tellers[i].available = 1;
				tellers[i].break_number++;
				start_wait = 1;
				j++;
				tellers[i].next_to_break = 0;
				next_teller_to_break = next_teller_break();
				tellers[next_teller_to_break].next_to_break = 1;
			}
		}
		if (start_teller_wait && tellers[i].available){
			gettimeofday(&teller_start, NULL);
			tellers[i].start_wait_time = teller_start.tv_sec + teller_start.tv_usec * 0.000001;
			start_teller_wait = 0;
		}
		// this is only set to 0 in customer thread
		if(!tellers[i].available){
			gettimeofday(&stop, NULL);
			tellers[i].end_wait_time = stop.tv_usec * 0.000001 + stop.tv_sec;
			tellers[i].all_waits[tellers[i].total_customer_count] = tellers[i].end_wait_time - tellers[i].start_wait_time;
			//printf("Customer serviced. Teller %d waited %f", i, tellers[i].all_waits[tellers[i].total_customer_count]);
			usleep(tellers[i].customer_transaction_time);
			tellers[i].total_customer_count++;
			tellers[i].available = 1;
			start_teller_wait = 1;
		}
	}


	pthread_exit(NULL);
}

int get_teller_customer_count(int teller_num){
	return tellers[teller_num].total_customer_count;
}

int get_teller_break_count(int teller_num_break){
	return tellers[teller_num_break].break_number;
}

int get_activate_breaks(void){
	return activate_breaks;
}

double max_wait_time(){
	double max = 0;
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; i++){
		for(j = 0; j < tellers[i].total_customer_count; j++){
			if (max < tellers[i].all_waits[j]) max = tellers[i].all_waits[j];
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

double average_break_time(int teller_num){
	double average_break = 0;
	int i = 0;
	int total = tellers[teller_num].break_number;

	for (i = 0; i < tellers[teller_num].break_number; i++){
		average_break += tellers[teller_num].break_waits[i]/total;
	}
	return average_break;
}

double max_break_time(int teller_num){
	double max_break = 0;
	int i = 0;
	for (i = 0; i < tellers[teller_num].break_number; i++){
		if (max_break < tellers[teller_num].break_waits[i]) max_break = tellers[teller_num].break_waits[i]; //Check this
	}
	return max_break;
}

double min_break_time(int teller_num){
	double min_break = tellers[teller_num].break_waits[0];
	int i = 0;
	for (i = 0; i < tellers[teller_num].break_number; i++){
		if (min_break > tellers[teller_num].break_waits[i]) min_break = tellers[teller_num].break_waits[i]; //Check this
	}
	return min_break;
}

int next_teller_break(void){
	return (rand() % 3);
}
