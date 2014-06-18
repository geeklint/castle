#include "mm.h"

struct MMType {
    size_t size;
    struct MMBlock * cache;
};

struct MMBlock {
    struct MMType * type;
    struct MMBlock * next;
};

struct MMType * mm_new(size_t size){
    struct MMType * type;
    
    if ((type = malloc(sizeof(struct MMType)))){
        type->size = size;
        type->cache = NULL;
    }
    
    return type;
}

/*
 * Allocate
 */
void * mm_alloc(struct MMType * type){
    void * block;
    if (type->cache){
        block = type->cache;
        type->cache = type->cache->next;
    } else {
        block = malloc(sizeof(struct MMBlock) + type->size);
        ((struct MMBlock *)block)->type = type;
    }
    return block + sizeof(struct MMBlock);
}

/*
 * Free
 */
void mm_free(void * block){
    struct MMType * type;

    block -= sizeof(struct MMBlock);
    type = ((struct MMBlock *)block)->type;
    ((struct MMBlock *)block)->next = type->cache;
    type->cache = block;
}

/*
 * Clear object cache
 */
void mm_clear(struct MMType * type){
    struct MMBlock * cache;
    while ((cache = type->cache)){
        type->cache = cache->next;
        free(cache);
    }
}

/*
 * Invalidate the type.
 * This function will clear the cache first,
 *     there is no need to explicty do so
 */
void mm_delete(struct MMType * type){
    mm_clear(type);
    free(type);
}

