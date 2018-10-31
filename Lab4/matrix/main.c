/*
 * main.c - Main program that carries out multiplication and verifies correctness
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "do_thread.h"

#define NUM_ARGUMENTS 3
#define MAX_RAND 10

/*
 * Function: generate_random_number
 * ---------------------------
 * Generates a random number less than MAX_RAND
 *
 * Returns: An random number less than MAX_RAND
 */
int generate_random_number() {
	return rand() % MAX_RAND;
}

/*
 * Function: create_thread
 * ---------------------------
 * Creates thread with given thread id and gv
 *
 * Arguments:
 * thread: pointer to pthread_t where thread is to be created
 * threadId: unique identifier for the thread
 * gv: pointer to the global variable
 */
void create_thread(pthread_t *thread, int threadId, GV *gv) {
	LV *lv = create_lv(threadId, gv);

	if (pthread_create(thread, NULL, do_thread, lv)) {
		perror("pthread_create");
		exit(1);
	}
}

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

			if (atoi(argv[i]) == 0) {
					printf("arg[%d] should be an integer greater than 0\n", i);
					exit(1);
			}
		}
	}
}

/*
 * Function: print_matrix
 * ----------------------------
 * Prints a given square matrix
 *
 * Arguments:
 * matrix: Pointer to the matrix
 * matrixSize: number of rows or columns of the square matrix
 */
void print_matrix(int **matrix, int matrixSize) {
	int i, j;

	for (i = 0; i < matrixSize; ++i) {
		for (j = 0; j < matrixSize; ++j) {
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}

/*
 * Function: multiply_sequential
 * -----------------------------
 * Performs sequential multiplication of two square matrices
 *
 * Arguments:
 * firstMatrix: Pointer to the first matrix to be multiplied
 * secondMatrix: Pointer to the second matrix to be multiplied
 * matrixSize: number of rows or columns of the square matrices
 *
 * Returns: pointer to the resultant matrix
 */
int **multiply_sequential(int **firstMatrix, int **secondMatrix, int matrixSize) {
	int i, j, k;
	int **resultMatrix;

	resultMatrix = (int **) malloc(sizeof(int *) * matrixSize);


	for (i = 0; i < matrixSize; ++i) {
		resultMatrix[i] = (int *) malloc(sizeof(int) * matrixSize);
		for (j = 0; j < matrixSize; ++j) {
			resultMatrix[i][j] = 0;
			for (k = 0; k < matrixSize; ++k) {
				resultMatrix[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}

	return resultMatrix;
}

/*
 * Function: multiply_sequential
 * -----------------------------
 * Performs parallel multiplication of two square matrices. It starts the
 * required number of thread to perform the multiplication.
 *
 * Arguments:
 * firstMatrix: Pointer to the first matrix to be multiplied
 * secondMatrix: Pointer to the second matrix to be multiplied
 * matrixSize: number of rows or columns of the square matrices
 * numberOfThreads: number of thread that will perform the multiplication.
 *
 * Returns: pointer to the resultant matrix
 */
int **multiply_parallel(int **firstMatrix, int **secondMatrix, int matrixSize, int numberOfThreads) {
	int **result;
	int i;
	void *retValue;
	pthread_t *threads;
	GV *gv;

	gv = create_gv(firstMatrix, secondMatrix, matrixSize);

	threads = (pthread_t *) malloc(sizeof(pthread_t) * numberOfThreads);
	if (threads == NULL) {
		printf("Thread: Failed to allocate memory\n");
		exit(1);
	}

	for (i = 0; i < numberOfThreads; ++i) {
		create_thread(&threads[i], i, gv);
	}

	for (i = 0; i < numberOfThreads; ++i) {
		pthread_join(threads[i], &retValue);
	}

	result = gv->resultMatrix;

	// Free Global Variable
	free_gv(gv);

	return result;
}

/*
 * Function: compare_matrices
 * -----------------------------
 * Compares two square matrices and prints 'Success' if they are equal or 'Error'
 * if they are not
 *
 * Arguments:
 * firstMatrix: Pointer to the first matrix to be multiplied
 * secondMatrix: Pointer to the second matrix to be multiplied
 * matrixSize: number of rows or columns of the square matrices
 */
void compare_matrices(int **firstMatrix, int ** secondMatrix, int matrixSize) {
	int i, j;
	int equals = 1;

	for (i = 0; i < matrixSize; ++i) {
		for (j = 0; j < matrixSize; ++j) {
			if (firstMatrix[i][j] != secondMatrix[i][j]) {
				equals = 0;
			}
		}
	}

	printf("%s\n", equals ? "Success" : "Error");
}

/*
 * Function: create_matrix
 * -----------------------------
 * Creates a square matrix of given size with random numbers
 *
 * Arguments:
 * matrixSize: number of rows or columns of the square matrix
 *
 * Returns: pointer to the created matrix
 */
int **create_matrix(int matrixSize) {
	int i, j;

	int **matrix = (int **) malloc(sizeof(int *) * matrixSize);

	for (i = 0; i < matrixSize; ++i) {
		matrix[i] = (int *) malloc(sizeof(int) * matrixSize);
		for (j = 0; j < matrixSize; ++j) {
			matrix[i][j] = generate_random_number();
		}
	}

	return matrix;
}

/*
 * Function: main
 * -----------------------------
 * Main function takes 2 extra arguments: matrix size and number of threads
 * It initializes two matrices of the given size.It performs the multiplication
 * (sequential and parallel) and verify their results are equal.
 */
int main(int argc, char **argv) {
	int matrixSize;
	int numberOfThreads;
	int **firstMatrix;
	int **secondMatrix;
	int **singleThreadResultMatrix;
	int **parallelResultMatrix;

	validate_arguments(argc, argv);

	matrixSize = atoi(argv[1]);
	numberOfThreads = atoi(argv[2]);

	srand(time(NULL));

	firstMatrix = create_matrix(matrixSize);
	secondMatrix = create_matrix(matrixSize);

	singleThreadResultMatrix = multiply_sequential(firstMatrix, secondMatrix, matrixSize);

	parallelResultMatrix = multiply_parallel(firstMatrix, secondMatrix, matrixSize, numberOfThreads);

	// Uncomment following lines to print the matrices
	// printf("First Matrix\n");
	// print_matrix(firstMatrix, matrixSize);
	// printf("\n");
	// printf("Second Matrix\n");
	// print_matrix(secondMatrix, matrixSize);
	// printf("\n");
	// printf("sequential Multiplication Result Matrix\n");
	// print_matrix(singleThreadResultMatrix, matrixSize);
	// printf("\n");
	// printf("Parallel Multiplication Result Matrix\n");
	// print_matrix(parallelResultMatrix, matrixSize);
	// printf("\n");

	compare_matrices(singleThreadResultMatrix, parallelResultMatrix, matrixSize);

	return 0;
}