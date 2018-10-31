/*
 * semaphore.h - Header file for the semaphore
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <pthread.h>

typedef struct {
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t cv;
} Semaphore;

/*
 * Function: create_semaphore
 * ---------------------------
 * Creates a semaphore of given size and returns it.
 *
 * Argument:
 * count: size of the semaphore
 *
 * Returns: Instance of the semaphore
 */
Semaphore *create_semaphore(int count);

/*
 * Function: free_semaphore
 * ---------------------------
 * Frees up space occupied by the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void free_semaphore(Semaphore *s);

/*
 * Function: P
 * ---------------------------
 * Proberen or wait function for the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void P(Semaphore *s);

/*
 * Function: V
 * ---------------------------
 * Verhogen or signal function for the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void V(Semaphore *s);