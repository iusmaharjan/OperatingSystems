/*
 * do_thread.h - Header file describing the functions of threads
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include "semaphore.h"

/*
 * Structure: GV
 * ---------------------------
 * Global variable holds the information required by all threads. It contains
 * the following members
 *
 * i - next row number to be calculated
 * j - next column number to be calculated
 * matrixSize - Size of the matrix
 */
typedef struct gv_t {
	int i;
	int j;

	int matrixSize;

	int **firstMatrix;
	int **secondMatrix;
	int **resultMatrix;

	Semaphore *mutex;
} GV;

/*
 * Structure: LV
 * ---------------------------
 * Local variable holds the information unique to each thread. It contains
 * the following members
 *
 * threadId - ID to recognize the thread
 * gv - Pointer to global data
 */
typedef struct lv_t {
	int threadId;
	GV *gv;
} LV;

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
GV *create_gv(int **firstMatrix, int **secondMatrix, int matrixSize);

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
LV *create_lv(int threadId, GV *gv);

/*
 * Function: free_gv
 * ---------------------------
 * Frees up space allocated by the global variable
 *
 * Argument:
 * gv: Instance of global variable to be freed
 */
void free_gv(GV *gv);

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
 * Function: do_thread
 * ---------------------------
 * Function executed by the multiplier thread
 *
 * Argument:
 * arg: Argument should be a pointer to the local variable lv
 */
void *do_thread(void *arg);
