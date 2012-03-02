#ifndef QT_HAZARDPTRS_H
#define QT_HAZARDPTRS_H

#include "qt_visibility.h"

#define FREELIST_DEPTH 8
#define HAZARD_PTRS_PER_SHEP 3

typedef struct {
    void (*free)(void *);
    void *ptr;
} hazard_freelist_entry_t;

typedef struct {
    hazard_freelist_entry_t freelist[FREELIST_DEPTH];
    unsigned int count;
} hazard_freelist_t;

void INTERNAL initialize_hazardptrs(void);
void INTERNAL hazardous_ptr(unsigned int which, uintptr_t ptr);
void INTERNAL hazardous_release_node(void (*freefunc)(void*), void *ptr);

#endif
