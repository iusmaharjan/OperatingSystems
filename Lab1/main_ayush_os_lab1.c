/*
 * main_ayush_os_lab1.c - Programming demonstrating usage of the memory block
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 1
 * Submitted by: Ayush Maharjan
 */

#include <stdio.h>
#include "memblockallocator_ayush_os_lab1.h"

/*
 * Function: main
 * ---------------------------
 * Main function to demonstrate usage of memory block
 */
int main() {

	printf("Block Pool 1\n");
	printf("--------------------------------------------------\n");

	// First block pool
	block_pool *pool = create_block_pool(3, 9, 5);

	// printf("Total Number of blocks %u: %u\n", (unsigned) &pool->total_num_blocks, pool->total_num_blocks);
	// printf("Alignment              %u: %u\n", (unsigned) &pool->alignment_bytes, pool->alignment_bytes);
	// printf("Available block        %u: %u\n", (unsigned) &pool->num_avail_blocks, pool->num_avail_blocks);
	// printf("Pool head              %u: %u\n", (unsigned) &pool->pool_head, (unsigned) pool->pool_head);
	// printf("--------------------------------------------------\n");

	print_free_blocks(pool);

	int *p = alloc_align_block(pool);
	int *q = alloc_align_block(pool);
	int *r = alloc_align_block(pool);
	int a = 10;
	int *s = alloc_align_block(pool);
	if (s == NULL) {
		printf("Could not allocate memory block for s\n");
	}

	free_align_block(pool, q);
	free_align_block(pool, p);
	free_align_block(pool, r);
	free_align_block(pool, s);
	print_free_blocks(pool);

	printf("\n\n==================================================\n");
	printf("Block Pool 2\n");
	printf("--------------------------------------------------\n");

	// Second block pool
	block_pool *pool2 = create_block_pool(5, 45, 100);

	print_free_blocks(pool2);
	int *p1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
	int *q1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
	int *r1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
	free_align_block(pool2, q1);
	print_free_blocks(pool2);
	q1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
	int *s1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
	int *t1 = alloc_align_block(pool2);
	print_free_blocks(pool2);
}