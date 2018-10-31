/*
 * memblockallocator_ayush_os_lab1.c - Implementation of the routines required for the aligned memory block allocator
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 1
 * Submitted by: Ayush Maharjan
 */

#include <stdlib.h>
#include <stdio.h>
#include "memblockallocator_ayush_os_lab1.h"

/*
 * Function: calculate_padding
 * ---------------------------
 * Calculates the number of padding bytes to be added to the memory block for correct memory alignment
 *
 * block_size: size of the block in bytes
 * alignment: required alignment byte
 *
 * returns: bytes required for padding
 */
int calculate_padding(int block_size, int alignment) {
	int total_block_size = alignment;
	while (block_size > total_block_size) {
		total_block_size += alignment;
	}
	return total_block_size - block_size;
}

/*
 * Function: create_block_pool
 * ---------------------------
 * Creates a pool of free fixed size aligned memory blocks
 *
 * num_blocks: required number of blocks to be allocated
 * block_size: size of the block in bytes
 * alignment: required alignment byte
 *
 * returns: pointer to the created block_pool
 */
block_pool *create_block_pool(int num_blocks, int block_size, int alignment) {

	// Create a block pool
	block_pool *pool = (block_pool *) malloc(sizeof(block_pool));
	if (NULL == pool) {
		printf("Memory for the block pool could not be allocated\n");
		return NULL;
	}

	// Initialize values of number of blocks, alignment and number of blocks
	pool->total_num_blocks = num_blocks;
	pool->alignment_bytes = alignment;
	pool->num_avail_blocks = num_blocks;

	// Calculate the required memory requirements for each block
	int min_block_size = sizeof(block) > block_size ? sizeof(block) : block_size;
	int padding = calculate_padding(min_block_size, alignment);
	int block_size_with_padding = min_block_size + padding;
	pool -> block_size_with_padding = block_size_with_padding;

	// Calculated the memory requirements for all the blocks
	int total_memory = block_size_with_padding * (num_blocks + 1) + alignment - 1;

	// Allocate a pool of memory
	if (NULL == (pool->allocated_memory = malloc(total_memory))) {
		printf("Could not allocate memory");
		return NULL;
	}

	// Align the pool head
	pool->pool_head = pool->allocated_memory;
	while (((unsigned)pool->pool_head) % alignment != 0) {
		pool->pool_head = (block *) (((char*)pool->pool_head) + 1);
	}

	// Print statements used for debugging purposes
	// printf("Padding: %u\n", padding);
	// printf("Block size with padding: %u\n", block_size_with_padding);
	// printf("--------------------------------------------------\n");
	// printf("Starting address: %u\n", (unsigned) pool->allocated_memory);
	// printf("Ending address: %u\n", (unsigned) ((char*) pool->allocated_memory) + total_memory);
	// printf("Head address: %u\n", (unsigned) pool->pool_head);
	// printf("--------------------------------------------------\n");

	// Create a list of free nodes
	block *prev_node = pool->pool_head;
	block *new_node;
	for (int i = 0; i < num_blocks; ++i) {
		new_node = (block *)(((char*)prev_node) + block_size_with_padding);
		prev_node -> next = new_node;
		prev_node = new_node;
		printf("New node created at %u\n", (unsigned) new_node);
	}

	return pool;
}

/*
 * Function: alloc_align_block
 * ---------------------------
 * Gets a block of memory from the free blocks of the pool and returns it
 *
 * pool: block pool from which the memory is to be allocated
 *
 * returns: address where data can be inserted
 */
void *alloc_align_block(struct block_pool *pool) {
	if (pool->num_avail_blocks == 0) {
		printf("No more free blocks available\n");
		return NULL;
	}

	block *free_block = pool->pool_head->next;
	pool->pool_head->next = free_block->next;

	pool->num_avail_blocks--;

	// Print statements used for debugging purposes
	// printf("--------------------------------------------------\n");
	// printf("Number of available blocks : %u\n", (unsigned) pool->num_avail_blocks);
	// printf("Allocated block : %u\n", (unsigned) free_block);
	// printf("Next free block : %u\n", (unsigned) pool->pool_head->next);

	return free_block;
}

/*
 * Function: free_align_block
 * ---------------------------
 * Frees the provided block of memory into the provided block pool
 *
 * pool: block pool where the block belongs to
 * blk: block to be freed
 */
void free_align_block(struct block_pool *pool, void *blk) {
	unsigned pool_head_address = (unsigned)(pool->pool_head);
	unsigned block_address = (unsigned) ((block *)blk);
	unsigned block_size_with_padding = pool->block_size_with_padding;
	unsigned total_num_blocks = pool->total_num_blocks;

	// printf("Block Size with padding: %u\n", total_num_blocks);

	// Check if the block belongs to the provided pool
	if (block_address < (pool_head_address + block_size_with_padding) ||
	    block_address > (pool_head_address + block_size_with_padding * total_num_blocks)) {
		printf("The memory address does not belong to the block pool\n");
		return;
	}

	// Check if the block's alignment is correct
	if ((block_address - pool_head_address) % block_size_with_padding != 0) {
		printf("The memory address is not the aligned memory block\n");
		return;
	}

	// Free the block
	block *current_block = pool->pool_head;
	block *selected_block = (block *) blk;
	while (current_block->next != NULL && (unsigned)current_block->next < block_address) {
		current_block = current_block->next;
	}
	selected_block->next = current_block->next;
	current_block->next = selected_block;

	pool->num_avail_blocks++;

	// Print statements used for debugging purposes
	// printf("--------------------------------------------------\n");
	// printf("Number of available blocks : %u\n", pool->num_avail_blocks);
	// printf("Freed block : %u\n", (unsigned) selected_block);
}

/*
 * Function: print_free_blocks
 * ---------------------------
 * Prints a list of free memory blocks
 *
 * pool: block pool for which the free blocks are to be printed
 */
void print_free_blocks(struct block_pool *pool) {
	block* current_block = pool->pool_head;
	printf("--------------------------------------------------\n");
	printf("Free memory blocks: \n");

	while (current_block->next != NULL) {
		current_block = current_block->next;
		printf("Free block: %u\n", (unsigned) current_block);
	}
	printf("**************************************************\n");

}