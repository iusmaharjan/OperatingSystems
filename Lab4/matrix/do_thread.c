/*
 * do_thread.c - Implementation of the thread functions
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>
#include <stdio.h>

#include "do_thread.h"

/*
 * Function: create_gv
 * ---------------------------
 * Creates global variable to be used by all threads.
 *
 * Argument:
 * firstMatrix: First Square Matrix
 * secondMatrix: Second Square Matrix
 * matrixSize: Size of the matrix
 *
 * Returns: Instance of the global variable
 */
GV *create_gv(int **firstMatrix, int **secondMatrix, int matrixSize) {
	GV *gv;
	int **resultMatrix;
	int i, j;

	gv = (GV *) malloc(sizeof(GV));
	if (gv == NULL) {
		printf("GV: Failed to allocate memory\n");
		exit(1);
	}

	gv->i = 0;
	gv->j = 0;
	gv->firstMatrix = firstMatrix;
	gv->secondMatrix = secondMatrix;
	gv->matrixSize = matrixSize;
	gv-> mutex = create_semaphore(1);

	resultMatrix = (int **) malloc(sizeof(int *) * matrixSize);
	for (i = 0; i < matrixSize; ++i) {
		resultMatrix[i] = (int *) malloc(sizeof(int) * matrixSize);
		for (j = 0; j < matrixSize; ++j) {
			resultMatrix[i][j] = 0;
		}
	}

	gv->resultMatrix = resultMatrix;

	return gv;
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
LV *create_lv(int threadId, GV *gv) {
	LV *lv = (LV *) malloc(sizeof(LV *));
	lv->threadId = threadId;
	lv->gv = gv;
	return lv;
}

/*
 * Function: free_gv
 * ---------------------------
 * Frees up space allocated by the global variable
 *
 * Argument:
 * gv: Instance of global variable to be freed
 */
void free_gv(GV *gv) {
	free_semaphore(gv->mutex);
	free(gv);
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
 * Function: do_thread
 * ---------------------------
 * Function executed by the multiplier thread. It picks up an element of the
 * matrix to multiply, and writes the result to the resultant matrix
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *do_thread(void *arg) {
	int i, j, k;
	LV *lv;
	GV *gv;

	lv = arg;
	gv = lv->gv;

	while (1) {
		// Lock acquired
		P(gv->mutex);

		// If every element is calculated, exit the thread
		if (gv->i == gv->matrixSize && gv->j == gv->matrixSize) {
			V(gv->mutex);
			free_lv(lv);
			return NULL;
		}

		// Increment the position to be calculated
		i = gv->i++;
		j = gv->j;
		if (gv->i == gv->matrixSize) {
			if (gv->j != gv->matrixSize-1) {
				gv->i = 0;
			}
			gv->j++;
		}

		// Lock released
		V(gv->mutex);

		// Perform Calculation
		for (k = 0; k < gv->matrixSize; ++k) {
			gv->resultMatrix[i][j] += gv->firstMatrix[i][k] * gv->secondMatrix[k][j];
		}

		// Uncomment the following line to see the calculation performed by the thread
		// printf("Thread %d calculated [%d][%d] as %d\n", lv->threadId, i, j, gv->resultMatrix[i][j]);
	}

	free_lv(lv);
	return NULL;
}
