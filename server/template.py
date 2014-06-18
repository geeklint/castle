#!/usr/bin/env python

import os
import sys

H_TEXT = '''
/* Object state */
struct {title};

/* Initialize new {title} */
struct {title} * {lower}_new();

/* Deinitialize {title} */
void {lower}_del(struct {title} *);
'''

C_TEXT = '''
#include "{lower}.h"

#include "mm.h"

static struct MMType * size = NULL;

/* Object state */
struct {title}{{
    
}};

/* Initialize new {title} */
struct {title} * {lower}_new(){{
    struct {title} * self;
    
    if (!size){{
        size = mm_new(sizeof(struct {title}));
    }}
    
    if ((self = mm_alloc(size))){{
        
    }}
    return self;
}}

/* Deinitialize {title} */
void {lower}_del(struct {title} * self){{
    mm_free(self);
}}
'''

if __name__ == '__main__':
    for name in sys.argv[1:]:
        title = name.title()
        lower = name.lower()
        if os.path.exists(lower + '.h') or os.path.exists(lower + '.c'):
            print name, 'exists, skipping'
            continue
        with open(lower + '.h', 'w') as header:
            header.write(H_TEXT.format(title=title, lower=lower))
        with open(lower + '.c', 'w') as source:
            source.write(C_TEXT.format(title=title, lower=lower))
        
