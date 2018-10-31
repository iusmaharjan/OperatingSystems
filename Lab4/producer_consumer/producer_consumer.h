/*
 * producer_consumer.h - Defines the structure and methods required for implementation of producer consumer problem
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include "semaphore.h"
#include "ring_buffer.h"

/*
 * Structure: ProducerConsumer
 * ---------------------------
 * Structure required to implement producer consumer problem
 *
 * mutexLock - binary lock for critical section
 * emptyCount - semaphore for counting empty spaces in buffer
 * fullCount - semaphore for counting full spaces in buffer
 * buffer - ring buffer that holds data
 */
typedef struct {
	Semaphore *mutexLock;
	Semaphore *emptyCount;
	Semaphore *fullCount;
	RingBuffer *buffer;
} ProducerConsumer;

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
ProducerConsumer *create_producer_consumer(int bufferSize);

/*
 * Function: free_producer_consumer
 * --------------------------------
 * Frees up space allocated by the producer consumer variable
 *
 * Argument:
 * producerConsumer: Instance of producer consumer variable to be freed
 */
void free_producer_consumer(ProducerConsumer *producerConsumer);