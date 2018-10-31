# IUPUI CSCI 503 Fall 2017 Lab 4

## Multiple Producer Multiple Consumer Problem

### Background

The program is a simulation of multiple producer multiple consumer problem.

The program creates the required number of producers and consumers, and
produces the required number of items.

Each producer and consumers sleeps for a random number of seconds specified as
the input.

A ring buffer of the given size is used to store the items produced by the
producers.

### File Description

* **semaphore.h** 			: Header file for semaphore
* **semaphore.c** 			: Semaphore implementation
* **do_thread.h** 			: Header file describing the functions of producer and consumer threads
* **do_thread.c** 			: Implementation of the producer and consumer thread functions
* **producer_consumer.h** 	: Header file required for initialization of producer consumer problem
* **producer_consumer.c** 	: Implementation of methods for initialization of producer consumer problem
* **ring_buffer.h** 		: Header file for ring buffer
* **ring_buffer.h** 		: Implementation of the ring buffer
* **main.c** 	  			: Main file that runs producer consumer problem
* **text_matrix** 			: Executable file generated after compilation

### Usage

* Executing `text_producer_consumer`:
	`test_producer_consumer <num_producers> <num_consumers> <max_sleep_secs> <num_items_produce> <buffer_size>`
* Run `make` command to create the *shm_ipc* executable file
* Run `make clean` command to clear all object and executable files