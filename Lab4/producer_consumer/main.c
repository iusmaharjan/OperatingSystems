#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#include "do_thread.h"

#define NUM_ARGUMENTS 6

/*
 * Function: validate_arguments
 * ----------------------------
 * Validates the arguments provided to the program
 *
 * Arguments:
 * argc: Argument count
 * argv: pointer to list of arguments
 */
void validate_arguments(int argc, char **argv) {
	int i, j, length;
	if (argc < NUM_ARGUMENTS) {
		printf("Not enough arguments\n");
		exit(1);
	} else {
		for (i = 1; i < NUM_ARGUMENTS; ++i) {
			length = strlen(argv[i]);
			for (j = 0; j < length; ++j) {
				if (!isdigit(argv[i][j])) {
					printf("arg[%d] should be an integer greater than 0\n", i);
					exit(1);
				}
			}

			if (atoi(argv[i]) == 0 && i != 3) {
					printf("arg[%d] should be an integer greater than 0\n", i);
					exit(1);
			}
		}
	}
}

/*
 * Function: create_producer_thread
 * --------------------------------
 * Creates producer thread with given thread id and gv
 *
 * Arguments:
 * thread: pointer to pthread_t where thread is to be created
 * threadId: unique identifier for the thread
 * gv: pointer to the global variable
 */
void create_producer_thread(pthread_t *thread, int threadId, GV *gv) {
	LV *lv;
	lv = create_lv(threadId, gv);
	if (pthread_create(thread, NULL, producer_thread_function, lv)) {
		perror("pthread_create");
		exit(1);
	}
}

/*
 * Function: create_consumer_thread
 * --------------------------------
 * Creates consumer thread with given thread id and gv
 *
 * Arguments:
 * thread: pointer to pthread_t where thread is to be created
 * threadId: unique identifier for the thread
 * gv: pointer to the global variable
 */
void create_consumer_thread(pthread_t *thread, int threadId, GV *gv) {
	LV *lv;
	lv = create_lv(threadId, gv);
	if (pthread_create(thread, NULL, consumer_thread_function, lv)) {
		perror("pthread_create");
		exit(1);
	}
}

/*
 * Function: main
 * -----------------------------
 * Main function takes 5 extra arguments:
 * number of producers
 * number of consumers
 * number of seconds to sleep
 * number of items to produces
 * size of the ring buffer
 *
 * It initialize global variable. It runs producer and consumer threads and
 * waits for them to finish.
 */
int main(int argc, char **argv) {
	int i;
	GV *gv;
	pthread_t *producerThreads;
	pthread_t *consumerThreads;
	void *retValue;

	srand(time(NULL));

	validate_arguments(argc, argv);

	int numberOfProducers = atoi(argv[1]);
	int numberOfConsumers = atoi(argv[2]);
	int maxSleepSeconds = atoi(argv[3]);
	int totalItems = atoi(argv[4]);
	int ringBufferSize = atoi(argv[5]);

	// Uncomment to print the arguments
	// printf("Number of producers: %d\n", numberOfProducers);
	// printf("Number of consumers: %d\n", numberOfConsumers);
	// printf("Max sleep seconds: %d\n", maxSleepSeconds);
	// printf("Total items to produce: %d\n", totalItems);
	// printf("Ring buffer size: %d\n", ringBufferSize);

	// Initialize semaphores and other variables
	gv = (GV *) malloc(sizeof(GV));
	if (gv == NULL) {
		printf("GV: Failed to allocate memory\n");
		exit(1);
	}
	gv->maxSleepSeconds = maxSleepSeconds;
	gv->totalCount = totalItems;
	gv->producedCount = 0;
	gv->consumedCount = 0;
	gv->producerConsumer = create_producer_consumer(ringBufferSize);

	producerThreads = (pthread_t *) malloc(sizeof(pthread_t) * numberOfProducers);
	if (producerThreads == NULL) {
		printf("Producer Threads: Failed to allocate memory\n");
		exit(1);
	}

	consumerThreads = (pthread_t *) malloc(sizeof(pthread_t) * numberOfConsumers);
	if (consumerThreads == NULL) {
		printf("Consumer Threads: Failed to allocate memory\n");
		exit(1);
	}

	// Create producers
	for (i = 0; i < numberOfProducers; ++i) {
		create_producer_thread(&producerThreads[i], i, gv);
	}

	// Create consumers
	for (i = 0; i < numberOfConsumers; ++i) {
		create_consumer_thread(&consumerThreads[i], i, gv);
	}

	// Join producers
	for (i = 0; i < numberOfProducers; ++i) {
		pthread_join(producerThreads[i], &retValue);
	}

	// Join consumers
	for (i = 0; i < numberOfConsumers; ++i) {
		pthread_join(consumerThreads[i], &retValue);
	}

	// Free variables
	free(producerThreads);
	free(consumerThreads);
	free(gv);

	return 0;
}