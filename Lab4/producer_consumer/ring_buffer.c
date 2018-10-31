/*
 * ring_buffer.c - Implementation of the functions to initialize structure required for ring buffer
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>
#include <stdio.h>

#include "ring_buffer.h"

/*
 * Function: create_ring_buffer
 * ----------------------------------
 * Creates ring buffer of the given size
 *
 * Argument:
 * size: Size of the buffer
 *
 * Returns: Pointer to instance of the buffer
 */
RingBuffer *create_ring_buffer(int size) {
	RingBuffer *ringBuffer = (RingBuffer *) malloc(sizeof(RingBuffer));
	if (ringBuffer == NULL) {
		printf("RingBuffer: Failed to allocate memory\n");
		return NULL;
	}

	ringBuffer->data = (int *) malloc(sizeof(int) * size);
	if (ringBuffer->data == NULL) {
		printf("RingBuffer: Failed to allocate memory for size %d\n", size);
		free(ringBuffer);
		return NULL;
	}

	ringBuffer->head = 0;
	ringBuffer->tail = 0;
	ringBuffer->size = size;
	return ringBuffer;
}

/*
 * Function: free_ring_buffer
 * --------------------------------
 * Frees up space allocated by the ring buffer
 *
 * Argument:
 * ringBuffer: Pointer to ring buffer to be freed
 */
void free_ring_buffer(RingBuffer *ringBuffer) {
	if (ringBuffer == NULL) {
		return;
	}
	free(ringBuffer->data);
	free(ringBuffer);
}

/*
 * Function: write_to_buffer
 * --------------------------------
 * Writes data to the buffer
 *
 * Argument:
 * buffer: Pointer to ring buffer to be written in
 * data: Data to be inserted
 */
void write_to_buffer(RingBuffer *buffer, int data) {
	unsigned index;

	if (buffer->head - buffer->tail == buffer->size) {
		printf("Buffer full\n");
		return;
	}

    // Write to the proper index
    index = buffer->head % buffer->size;
    buffer->data[index] = data;

    // Increment head using atomic operation
    buffer->head++;
}

/*
 * Function: read_from_buffer
 * --------------------------------
 * Frees up space allocated by the ring buffer
 *
 * Argument:
 * buffer: Pointer to ring buffer to be read from
 *
 * Returns: read data
 */
int read_from_buffer(RingBuffer *buffer) {
	unsigned index;
	int data;

	if (buffer->head - buffer->tail == 0) {
		printf("Buffer empty\n");
		return 0;
	}

    // Write to the proper index
    index = buffer->tail % buffer->size;
    data = buffer->data[index];

    // Increment head using atomic operation
    buffer->tail++;

    return data;
}

/* Code for testing purposes
void print_buffer(RingBuffer *buffer) {
	for (int i = buffer->tail; buffer->head != i; ++i) {
		printf("%d\t", buffer->data[i]);
	}
	printf("\n");
}

void test_zero_size_buffer() {
	printf("For zero buffer size:\n");
	RingBuffer *ringBuffer = create_ring_buffer(0);
	free(ringBuffer);
	printf("---------------------------------------------------\n");
}

void test_negative_size_buffer() {
	printf("For negative buffer size:\n");
	RingBuffer *ringBuffer = create_ring_buffer(-1);
	free(ringBuffer);
	printf("---------------------------------------------------\n");
}

void test_buffer() {
	printf("Normal Test\n");
	RingBuffer *ringBuffer = create_ring_buffer(4);

	read_from_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 1);
	print_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 2);
	print_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 3);
	print_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 4);
	print_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 5);
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	write_to_buffer(ringBuffer, 5);
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	printf("Read: %d\n", read_from_buffer(ringBuffer));
	print_buffer(ringBuffer);

	free(ringBuffer);
	printf("---------------------------------------------------\n");
}

int main() {
	test_zero_size_buffer();
	test_negative_size_buffer();
	test_buffer();
}
*/