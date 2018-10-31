# IUPUI CSCI 503 Fall 2017 Lab 1

## Aligned Memory Block Allocator

### Background

Most often developers use the C functions of malloc() and free() to allocate/free various-size memory blocks dynamically. Although it is flexible, it is not the fastest way to allocate/release memories. In certain system-level runtimes and OS kernels, there are only a fixed number of data types being used. Hence, an optimized approach is to pre-allocate a pool of free blocks in advance so that a free block can be picked up and returned to the user quickly. Its speed is so fast that an alloc or free operation only involves a couple of assignments. The outcome of the lab is that you will have a much faster memory allocator than the standard libraries provided by existing computer systems.

### File Description

* **memblockallocator_ayush_os_lab1.h** : defines the structures and routines required for the memory block allocator
* **memblockallocator_ayush_os_lab1.c** : Implementation of the routines required for the memory block allocator
* **main_ayush_os_lab1.c** : Provides the main function to demonstrate the usage of the memory block allocator (*Note: You can make changes in this file to test the implementation of the memory block allocator*)

### Implementation Details

* The list of free nodes are created using a single linked list
* The structure **block** has only one member, a pointer which points to the address of the next block. There is no 'data' field because the data is overwritten over the block. The size of the free blocks are calculated keeping in mind the requirements from the **block** structure and the *block_size*.

### Usage

* Run `make` command to create the *memblockallocator_ayush_os_lab1.o* object file
* Run `make test` command to create the `test` executable that outputs the result of the demo program in *main_ayush_os_lab1.c*
* Run `make clean` command to clear all object and executable files
