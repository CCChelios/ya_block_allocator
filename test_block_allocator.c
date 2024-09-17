#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <time.h> 

#include "ya_block_allocator.h"

void test_block_allocator_create() {
    BlockAllocator *allocator = block_allocator();
    assert(allocator != NULL);
    for (int i = 0; i < NUM_BLOCKS; i++) {
        assert(allocator->map_used_blocks[i] == 0);
    }
    printf("passed test_block_allocator_craete.\n");
}

void test_block_allocator_destroy(){
    BlockAllocator *allocator = block_allocator();
    assert(block_allocator_destroy(allocator) == 0);
    printf("passed test_block_allocator_destroy.\n");
}

void test_block_allocator_allocate() {
    BlockAllocator *allocator = block_allocator();
    void *blocks[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = block_allocator_allocate(allocator);
        void *start_adr = allocator->pool;
        void *end_adr = allocator->pool + BLOCK_SIZE * sizeof(size_t) * NUM_BLOCKS;
        assert((blocks[i] >= start_adr) && (blocks[i] < end_adr));
    }
    // Check that the blocks are over
    void* block = block_allocator_allocate(allocator);
    assert(block == NULL);
    block_allocator_destroy(allocator);
    printf("passed test_block_allocator_allocate.\n");
}

void test_block_allocator_free() {
    BlockAllocator *allocator = block_allocator();
    void* blocks[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = block_allocator_allocate(allocator);
    }

    // We release the blocks in reverse order and check
    for (int i = NUM_BLOCKS-1; i >= 0; i--) {
        assert(block_allocator_free(allocator, blocks[i]));
        assert(allocator->map_used_blocks[i] == false);
    }

    // Checking that all blocks can be allocated again
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = block_allocator_allocate(allocator);
        assert(blocks[i] != NULL);
    }
    block_allocator_destroy(allocator);
    printf("passed test_block_allocator_free.\n");
}

void test_block_allocator_write_read(){
    srand(time(0));
    int size_cell = sizeof(void*);
    BlockAllocator *allocator = block_allocator();
    void *blocks[NUM_BLOCKS];
    block_index_t *array_for_test[NUM_BLOCKS][COUNT_CELL_IN_BLOCK];
    int *array_for_test_alias = (int *)array_for_test;
    array_for_test[0];
    // fill allocated memory and test array
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = block_allocator_allocate(allocator);
        for (int j = 0; j < (BLOCK_SIZE / sizeof(int)); j++){
            ((int *)blocks[i])[j] = ((int *)array_for_test[i])[j] = rand();
        }
    }
    // compare allocated memory and test array
    for (int i = 0; i < NUM_BLOCKS; i++){
        for (int j = 0; j < (BLOCK_SIZE / sizeof(int)); j++){
            assert(((int *)blocks[i])[j] == ((int *)array_for_test[i])[j]);
        }
    }

    block_allocator_destroy(allocator);
    printf("passed test_block_allocator_write_read.\n");
}

int main() {
    test_block_allocator_create();
    test_block_allocator_destroy();
    test_block_allocator_allocate();
    test_block_allocator_free();
    test_block_allocator_write_read();

    printf("All tests passed.\n");
    return 0;
}
