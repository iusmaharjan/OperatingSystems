/*
 * memblockallocator_ayush_os_lab1.h - Structures and routines required for the memory block allocator
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 1
 * Submitted by: Ayush Maharjan
 */

/*
 * Structure: block
 * ---------------------------
 * Data Structure to represent a free memory block used in the memory allocator
 *
 * *next: a pointer to the next free block
 */
typedef struct block {
	struct block *next;
} block;

/*
 * Structure: block_pool
 * ---------------------------
 * Data structure to represent a pool of fixed sized aligned memory blocks
 *
 * total_num_blocks: total number of blocks
 * alignment_bytes: bytes in the memory to which the blocks are aligned
 * num_avail_blocks: number of free blocks available
 * *allocated_memory: pointer to the allocated block of memory
 * *pool_head: pointer to the head block of free memory
 * block_size_with_padding: total size required for each block (includes the padding bytes as well)
 */
typedef struct block_pool {
	int total_num_blocks;
	int alignment_bytes;
	int num_avail_blocks;
	void* allocated_memory;
	block* pool_head;
	int block_size_with_padding;
} block_pool;

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
block_pool *create_block_pool(int num_blocks, int block_size, int alignment);

/*
 * Function: alloc_align_block
 * ---------------------------
 * Gets a block of memory from the free blocks of the pool and returns it
 *
 * pool: block pool from which the memory is to be allocated
 *
 * returns: address where data can be inserted
 */
void *alloc_align_block(struct block_pool *pool);

/*
 * Function: free_align_block
 * ---------------------------
 * Frees the provided block of memory into the provided block pool
 *
 * pool: block pool where the block belongs to
 * blk: block to be freed
 */
void free_align_block(struct block_pool* pool, void* blk);

/*
 * Function: print_free_blocks
 * ---------------------------
 * Prints a list of free memory blocks
 *
 * pool: block pool for which the free blocks are to be printed
 */
void print_free_blocks(struct block_pool* pool);