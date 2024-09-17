#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

// The implementation is adapted for 32 and 64 bit systems

typedef size_t block_index_t;
#define BLOCK_SIZE 16 //Size in byte. Must be a multiple of 8
#define NUM_BLOCKS 10 
#define COUNT_CELL_IN_BLOCK BLOCK_SIZE / sizeof(void*)
#define POOL_SIZE COUNT_CELL_IN_BLOCK * NUM_BLOCKS

typedef struct {
    block_index_t pool[POOL_SIZE];
    bool map_used_blocks[NUM_BLOCKS];
    pthread_mutex_t *mutex;
} BlockAllocator;

BlockAllocator *block_allocator();

int block_allocator_destroy(BlockAllocator* allocator);

int get_free_block_number_allocator(BlockAllocator* allocator);

void* block_allocator_allocate(BlockAllocator* allocator);

int block_allocator_free(BlockAllocator* allocator, void* block);

#endif // BLOCK_ALLOCATOR_H
