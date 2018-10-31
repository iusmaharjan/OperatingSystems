/*
 * producer_consumer.c - Implementation of the functions to initialize structure required for producer consumer problem
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>

#include "producer_consumer.h"

/*
 * Function: create_producer_consumer
 * ----------------------------------
 * Creates structure required for producer consumer problem
 *
 * Argument:
 * bufferSize: Size of the buffer
 *
 * Returns: Instance of the local variable
 */
ProducerConsumer *create_producer_consumer(int bufferSize) {
	ProducerConsumer *producerConsumer = (ProducerConsumer *) malloc(sizeof(ProducerConsumer));
	producerConsumer->mutexLock = create_semaphore(1);
	producerConsumer->emptyCount = create_semaphore(bufferSize);
	producerConsumer->fullCount = create_semaphore(0);
	producerConsumer->buffer = create_ring_buffer(bufferSize);
	return producerConsumer;
}

/*
 * Function: free_producer_consumer
 * --------------------------------
 * Frees up space allocated by the producer consumer variable
 *
 * Argument:
 * producerConsumer: Instance of producer consumer variable to be freed
 */
void free_producer_consumer(ProducerConsumer *producerConsumer) {
	if (producerConsumer == NULL) {
		return;
	}

	free_semaphore(producerConsumer->mutexLock);
	free_semaphore(producerConsumer->emptyCount);
	free_semaphore(producerConsumer->fullCount);
	free_ring_buffer(producerConsumer->buffer);
}