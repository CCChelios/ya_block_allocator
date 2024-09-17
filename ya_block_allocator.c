#include "ya_block_allocator.h"

BlockAllocator *block_allocator(){
    BlockAllocator *allocator = (BlockAllocator *)malloc(sizeof(BlockAllocator));
    if (allocator == NULL){
        return NULL;
    }
    allocator->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (allocator->mutex == NULL){
        return NULL;
    }
    pthread_mutex_init(allocator->mutex, NULL);
    for (int i = 0; i < NUM_BLOCKS; i++) {
        allocator->map_used_blocks[i] = false;
    }
    return allocator;
}

int block_allocator_destroy(BlockAllocator* allocator) {
    if (allocator == NULL){
        return 1;
    }
    if (pthread_mutex_destroy(allocator->mutex)){
        return 1;
    }
    free(allocator->mutex);
    free(allocator);
    return 0;
}

void* block_allocator_allocate(BlockAllocator* allocator) {
    pthread_mutex_lock(allocator->mutex);
    int issuing_block_number = get_free_block_number_allocator(allocator);
    if (issuing_block_number == -1) {
        pthread_mutex_unlock(allocator->mutex);
        return NULL;
    }
    allocator->map_used_blocks[issuing_block_number] = 1;
    pthread_mutex_unlock(allocator->mutex);
    return &allocator->pool[issuing_block_number * COUNT_CELL_IN_BLOCK];
}

int block_allocator_free(BlockAllocator *allocator, void *block) {
    if (block == NULL) {
        return 0;
    }
    else if ((block < &allocator->pool) || (block > &allocator->pool[POOL_SIZE]))
    {
        return 0;
    }
    block_index_t block_index = ((uintptr_t)block - (uintptr_t)allocator->pool);
    block_index /= BLOCK_SIZE;
    pthread_mutex_lock(allocator->mutex);
    allocator->map_used_blocks[block_index] = false;
    pthread_mutex_unlock(allocator->mutex);
    return 1;
}

int get_free_block_number_allocator(BlockAllocator* allocator){
    for(int i = 0; i < NUM_BLOCKS; i++){
        if (!allocator->map_used_blocks[i]){
            return i;
        }
    }
    return -1;
}