#include "random.h" 
/* Mersenne twiser (code adapted from wikipedia) */

#include <string.h>

#include "mm.h"

static struct MMType * size = NULL;

/* Object state */
struct Random{
    int index;
    uint32_t MT[624];
};

/* Constructor */
struct Random * random_new(uint32_t seed){
    struct Random * self;
    
    if (!size){
        size = mm_new(sizeof(struct Random));
    }
    
    if ((self = mm_alloc(size))){
        self->index = 0;
        self->MT[0] = seed;
        for (int i = 1; i < 624; ++i){
            self->MT[i] = i + 0x6c078965
                * (self->MT[i-1] ^ (self->MT[i-1] >> 30));
        }
    }
    return self;
}

/* Repopulate array with next set of numbers */
static void generate_numbers(struct Random * self){
    int y;
    
    for (int i = 0; i < 624; ++i){
        y = (self->MT[i] & 0x80000000) + (self->MT[(i+1) % 624] & 0x7fffffff);
        self->MT[i] = self->MT[(i + 397) & 624] ^ (y >> 1);
        if (y & 1){ // y is odd
             self->MT[i] = self->MT[i] ^ 0x9908b0df;
        }
    }
}

/* Get random number */
uint32_t random_random(struct Random * self){
    int y;
    
    if (!self->index){
        generate_numbers(self);
    }
    y = self->MT[self->index];
    y ^= (y >> 11);
    y ^= ((y << 7) & 0x9d2c5680);
    y ^= ((y << 15) & 0xefc60000);
    y ^= (y >> 18);
    self->index = (self->index + 1) % 624;
    return y;
}

/* Get number in range */
int random_range(struct Random * self, int min, int max){
    int diff = max - min;
    int choice;
    
    choice = (int) (((double) random_random(self)) / UINT32_MAX * diff);
    return choice + min;
}

/* Static size shuffles */
static void random_shuffle_8(
        struct Random * self, uint8_t * array, size_t count){
    uint8_t temp;
    int index;

    while (--count){
        index = random_range(self, 0, count);
        temp = array[count];
        array[count] = array[index];
        array[index] = temp;
    }
}

static void random_shuffle_16(
        struct Random * self, uint16_t * array, size_t count){
    uint16_t temp;
    int index;
    
    while (--count){
        index = random_range(self, 0, count);
        temp = array[count];
        array[count] = array[index];
        array[index] = temp;
    }
}

static void random_shuffle_32(
        struct Random * self, uint32_t * array, size_t count){
    uint32_t temp;
    int index;

    while (--count){
        index = random_range(self, 0, count);
        temp = array[count];
        array[count] = array[index];
        array[index] = temp;
    }
}



/* Shuffle array */
void random_shuffle(
        struct Random * self, void * array, size_t block, size_t count){
	void * temp;
	int index;

    switch (block){
    	case 1:
    		random_shuffle_8(self, array, count);
    		return;
        case 2:
            random_shuffle_16(self, array, count);
            return;
        case 4:
        	random_shuffle_32(self, array, count);
        	return;
        default:
            break;
    }
    temp = malloc(block);
    while (--count){
    	index = random_range(self, 0, count);
    	memcpy(temp, block * count + array, block);
    	memcpy(block * count + array, block * index + array, block);
    	memcpy(block * index + array, temp, block);
    }
    free(temp);
}

/* Deinitialize generator */
void random_del(struct Random * self){
    mm_free(self);
}

