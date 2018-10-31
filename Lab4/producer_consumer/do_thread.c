/*
 * do_thread.c - Implementation of the producer and consumer thread functions
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "do_thread.h"

/*
 * Function: generate_random_number
 * ---------------------------
 * Generates a random number less than or equal to the given size
 *
 * Arguments:
 * max: maximum value of the random number
 *
 * Returns: An random number less than the given size
 */
int generate_random_number(int max) {
	return rand() % (max + 1);
}

/*
 * Function: create_lv
 * ---------------------------
 * Creates local variable unique to each thread.
 *
 * Argument:
 * threadId: ID of the thread
 * gv: An instance of global variable
 *
 * Returns: Instance of the local variable
 */
LV *create_lv(int threadId, void *gv) {
	LV *lv = (LV *) malloc(sizeof(LV));
	if (lv == NULL) {
		printf("LV: Failed to allocate memory\n");
		exit(1);
	}
	lv->threadId = threadId;
	lv->processedCount = 0;
	lv->gv = gv;
	return lv;
}

/*
 * Function: free_lv
 * ---------------------------
 * Frees up space allocated by the local variable
 *
 * Argument:
 * lv: Instance of local variable to be freed
 */
void free_lv(LV *lv) {
	free(lv);
}

/*
 * Function: producer_thread_function
 * ----------------------------------
 * Function executed by the producer thread
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *producer_thread_function(void *arg) {
	LV *lv = arg;
	int sleepTime;
	int numberOfItemsProduced = 0;

	while (1) {

		// Generate and sleep for a random number of seconds
		sleepTime = generate_random_number(lv->gv->maxSleepSeconds);
		sleep(sleepTime);

		P(lv->gv->producerConsumer->emptyCount);

		P(lv->gv->producerConsumer->mutexLock);

		// If total of items is produced, unlock empty and exit the loop
		if (lv->gv->producedCount == lv->gv->totalCount) {
			V(lv->gv->producerConsumer->mutexLock);
			V(lv->gv->producerConsumer->emptyCount);
			break;
		}

		write_to_buffer(lv->gv->producerConsumer->buffer, sleepTime);
		// Uncomment to print the value written to the buffer
		// printf("Producer %d added %d to buffer\n", lv->threadId, sleepTime);
		lv->gv->producedCount++;
		numberOfItemsProduced++;

		V(lv->gv->producerConsumer->mutexLock);

		V(lv->gv->producerConsumer->fullCount);
	}

	// Print number of items produced
	printf("Producer %d finished: %d items produced\n", lv->threadId, numberOfItemsProduced);

	free_lv(lv);
	return NULL;
}

/*
 * Function: consumer_thread_function
 * ----------------------------------
 * Function executed by the consumer thread
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *consumer_thread_function(void *arg) {
	LV *lv = arg;
	int sleepTime;
	int item;
	int numberOfItemsConsumed = 0;

	while (1) {

		P(lv->gv->producerConsumer->fullCount);

		P(lv->gv->producerConsumer->mutexLock);

		// If required number of items is already produced, unlock the locked full count and exit the loop
		if (lv->gv->totalCount == lv->gv->consumedCount) {
			V(lv->gv->producerConsumer->mutexLock);
			V(lv->gv->producerConsumer->fullCount);
			break;
		}

		item = read_from_buffer(lv->gv->producerConsumer->buffer);
		// Uncomment to print the value read
		// printf("Consumer %d read %d from buffer\n", lv->threadId, item);
		lv->gv->consumedCount++;
		numberOfItemsConsumed++;

		// If required number of items is produced in this loop, unlock the an extra fullCount and other variables before exiting
		if (lv->gv->totalCount == lv->gv->consumedCount) {
			V(lv->gv->producerConsumer->mutexLock);
			V(lv->gv->producerConsumer->fullCount);
			V(lv->gv->producerConsumer->emptyCount);
			break;
		}

		V(lv->gv->producerConsumer->mutexLock);

		V(lv->gv->producerConsumer->emptyCount);

		// Sleep for random seconds
		sleepTime = generate_random_number(lv->gv->maxSleepSeconds);
		sleep(sleepTime);
	}

	// Print number of items consumed
	printf("Consumer %d finished: %d items consumed\n", lv->threadId, numberOfItemsConsumed);

	free_lv(lv);
	return NULL;
}