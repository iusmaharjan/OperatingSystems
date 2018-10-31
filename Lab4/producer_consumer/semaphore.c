/*
 * semaphore.c - Implementation of the semaphore
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>
#include <stdio.h>
#include "semaphore.h"

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
Semaphore *create_semaphore(int count) {
	Semaphore *semaphore = (Semaphore *) malloc(sizeof(Semaphore));
	if (semaphore == NULL) {
		printf("Semaphore: Failed to allocate memory\n");
		return NULL;
	}

	semaphore->count = count;

	// Initialize mutex
	if (pthread_mutex_init(&semaphore->mutex, NULL) != 0) {
		perror("pthread_mutex_init");
		exit(1);
	}

	// Initialize context variable
	if (pthread_cond_init(&semaphore->cv, NULL) != 0) {
		perror("pthread_cond_init");
		exit(1);
	}

	return semaphore;
}

/*
 * Function: free_semaphore
 * ---------------------------
 * Frees up space occupied by the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void free_semaphore(Semaphore *s) {
	free(s);
}

/*
 * Function: P
 * ---------------------------
 * Proberen or wait function for the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void P(Semaphore *s) {
	// Acquire lock
	if (pthread_mutex_lock(&s->mutex) != 0) {
		perror("pthread_mutex_lock");
		exit(1);
	}

	// Wait for the lock if count is 0
	while (s->count == 0) {
		if (pthread_cond_wait(&s->cv, &s->mutex) != 0) {
			perror("pthread_cond_wait");
			exit(1);
		}
	}

	// Decrease semaphore value
	s->count--;

	// Release lock
	if (pthread_mutex_unlock(&s->mutex) != 0) {
		perror("pthread_mutex_unlock");
		exit(1);
	}
}

/*
 * Function: V
 * ---------------------------
 * Verhogen or signal function for the semaphore
 *
 * Argument:
 * s: pointer to semaphore
 */
void V(Semaphore *s) {
	// Acquire lock
	if (pthread_mutex_lock(&s->mutex) != 0) {
		perror("pthread_mutex_lock");
		exit(1);
	}

	// Increase count
	s->count++;

	// Release lock
	if (pthread_mutex_unlock(&s->mutex) != 0) {
		perror("pthread_mutex_unlock");
		exit(1);
	}

	// Signal for other waiting semaphore
	if (pthread_cond_signal(&s->cv) != 0) {
		perror("pthread_cond_signal");
		exit(1);
	}
}