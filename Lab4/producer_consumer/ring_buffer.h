/*
 * ring_buffer.h - Defines the structure and methods required for implementation of ring buffer
 *
 * CSCI503(Fall 2017) Operating Systems: Lab 4
 * Submitted by: Ayush Maharjan
 */

/*
 * Structure: RingBuffer
 * ---------------------------
 * Structure required to implement ring buffer
 *
 * data - pointer to the data
 * size - buffer size
 * head - head index of the buffer
 * tail - tail index of the buffer
 */
typedef struct {
    int *data;
    int size;
    int head;
    int tail;
} RingBuffer;

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
RingBuffer *create_ring_buffer(int size);

/*
 * Function: free_ring_buffer
 * --------------------------------
 * Frees up space allocated by the ring buffer
 *
 * Argument:
 * ringBuffer: Pointer to ring buffer to be freed
 */
void free_ring_buffer(RingBuffer *ringBuffer);

/*
 * Function: write_to_buffer
 * --------------------------------
 * Writes data to the buffer
 *
 * Argument:
 * buffer: Pointer to ring buffer to be written in
 * data: Data to be inserted
 */
void write_to_buffer(RingBuffer *buffer, int data);

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
int read_from_buffer(RingBuffer *ringBuffer);