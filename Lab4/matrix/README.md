# IUPUI CSCI 503 Fall 2017 Lab 4

## Parallel Matrix Multiplication

### Background

The program perform matrix multiplication using n number of threads, in which
each thread selects a random matrix position and performs the multiplication.

The output is verified against a sequential matrix multiplication and returns
`True` if they are equal. Else, it returns `False`.

### File Description

* **semaphore.h** : Header file for semaphore
* **semaphore.c** : Semaphore implementation
* **main.c** 	  : Main file that reads input and performs multiplication
* **do_thread.h** : Header file describing the functions of threads
* **do_thread.c** : Implementation of the thread functions
* **text_matrix** : Executable file generated after compilation

### Usage

* Executing `text_matrix`: `test_matrix <matrix_size> <num_threads>`
* Run `make` command to create the *shm_ipc* executable file
* Run `make clean` command to clear all object and executable files