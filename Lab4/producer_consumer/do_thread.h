/*
 * do_thread.h - Header file describing the producer and consumer functions of threads
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include "producer_consumer.h"

/*
 * Structure: GV
 * ---------------------------
 * Global variable holds the information required by all threads. It contains
 * the following members
 *
 * maxSleepSeconds - maximum seconds each thread sleeps
 * totalCount - count of total items to be produced
 * producedCount - count of the items produced
 * consumedCount - count of the items consumed
 * producerConsumer - pointer to structure containing semaphores for producer consumer
 */
typedef struct gv_t {
	int maxSleepSeconds;
	int totalCount;
	int producedCount;
	int consumedCount;
	ProducerConsumer *producerConsumer;
} GV;

/*
 * Structure: LV
 * ---------------------------
 * Local variable holds the information unique to each thread. It contains
 * the following members
 *
 * threadId - ID to recognize the thread
 * processedCount - Number of items processed
 * gv - Pointer to global data
 */
typedef struct lv_t {
	int threadId;
	int processedCount;
	GV *gv;
} LV;

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
LV *create_lv(int threadId, void *gv);

/*
 * Function: free_lv
 * ---------------------------
 * Frees up space allocated by the local variable
 *
 * Argument:
 * lv: Instance of local variable to be freed
 */
void free_lv(LV *lv);

/*
 * Function: producer_thread_function
 * ----------------------------------
 * Function executed by the producer thread
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *producer_thread_function(void *lv);

/*
 * Function: consumer_thread_function
 * ----------------------------------
 * Function executed by the consumer thread
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *consumer_thread_function(void *lv);

