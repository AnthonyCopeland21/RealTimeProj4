#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// STRUCTS
typedef struct {
	int queue_time;
	int transaction_time;
} Customer;

typedef struct {
	int available;
	int break_number;
	int break_time;
	pthread_t id;
} Teller;


// GLOBALS
Teller tellers[3];
Customer customers[420];  // 420 is max number of customers in a day (1 per minute)
int bank_open = 0;


// PROTOTYPES
int available_teller(Teller *tellers);
int max_customer_queue_time(Customer *customers);
int max_wait_teller_time(Customer *customers);
int max_transaction_time(Customer *customers);
// thread function prototypes
void *teller_thread(void *arg);
void *customer_thread(void *arg);

int main(int argc, char *argv[]) {
	printf("Welcome to the Bank!\n");
	int ret = 0;
	int i = 0;
	pthread_t queue_id;

	// loop to create 3 teller threads
	for(i = 0; i < 3; i++) {
		ret = pthread_create(&tellers[i].id, NULL, &teller_thread, (void *)i);
		if (ret != 0){
			printf("ERROR: pthread_create() failed\n");
			exit(EXIT_FAILURE);
		}
	}
	// customer queue created
	ret = pthread_create(&queue_id, NULL, &customer_thread, (void *)NULL);

	pthread_exit(NULL);
	return EXIT_SUCCESS;
}

/* Purpose: Provide next available teller
 * Inputs: tellers - array of all tellers
 * Output: 1->length of tellers for available teller
 * 		   0 for no available teller
 */
int available_teller(Teller *tellers) {
	int i = 0;
	// loop runs through tellers that
	for (i = 0; i < (sizeof(tellers)/sizeof(tellers[0])); i++){
		if (tellers[i].available) return (i + 1);
	}
	return 0;
}

/* Purpose: Thread function for the teller
 * Inputs:  None
 * Output:  None
 */
void *teller_thread(void *arg){
	int i = (int)arg;
	// initialize teller arguments
	tellers[i].available = 1;
	tellers[i].break_number = 0;
	tellers[i].break_time = 0;

	printf("Creation of teller %d\n", i+1);
	// loop for whole day of work
	while(bank_open){
		// nested loop waiting for teller to be unavailable
		while(!tellers[i].available){
			// do teller stuff
		}
	}

	pthread_exit(NULL);
}

/* Purpose: Thread function for the customer queue
 * Inputs:  (void *) arg for arguments added in pthread_create
 * Output:  None
 */
void *customer_thread(void *arg){
	// initialize customer arguments
	Customer *queue;
	int count = 0;

	queue = malloc(sizeof(Customer));
	queue[count].queue_time = 0;  // initialized to first count
	queue[count].transaction_time = rand() % 450 + 30;

	printf("Creation of customer queue\n");

	// for each new customer, realloc(queue, sizeof(Customer))

	free(queue);

	pthread_exit(NULL);
}
