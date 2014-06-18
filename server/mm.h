#ifndef MM_H
#define MM_H

#include <stdlib.h>

struct MMType;

/*
 * Create a new managed type
 */
struct MMType * mm_new(size_t);

/*
 * Allocate
 */
void * mm_alloc(struct MMType *);

/*
 * Free
 */
void mm_free(void *);

/*
 * Clear object cache
 */
void mm_clear(struct MMType *);

/*
 * Invalidate the type.
 * This function will clear the cache first,
 *     there is no need to explicty do so
 */
void mm_delete(struct MMType *);

#endif /* MM_H */
